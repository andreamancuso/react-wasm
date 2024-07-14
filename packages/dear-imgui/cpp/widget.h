// Dear ImGui: standalone example application for Emscripten, using GLFW + WebGPU
// (Emscripten is a C++-to-javascript compiler, used to publish executables for the web. See https://emscripten.org/)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <optional>
#include <tuple>
#include <cstring>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

#include "shared.h"

using json = nlohmann::json;

#pragma once

class ReactImgui;

struct BaseStyle {
    std::optional<StyleColors> maybeColors;
    std::optional<StyleVars> maybeStyleVars;
    std::optional<int> maybeFontIndex;
    std::optional<float> maybeWidth;
    std::optional<float> maybeHeight;
    std::optional<HorizontalAlignment> maybeHorizontalAlignment;
};

class Widget {
    public:
        int m_id;
        std::string m_type;
        bool m_handlesChildrenWithinRenderMethod;

        // todo: does this belong here?
        inline static OnTextChangedCallback onInputTextChange_;

        Widget(int id) {
            m_id = id;
            m_type = "Unknown";
            m_handlesChildrenWithinRenderMethod = false;
        }

        void HandleChildren(ReactImgui* view);

        virtual void PreRender(ReactImgui* view);

        virtual void Render(ReactImgui* view) = 0;

        virtual void PostRender(ReactImgui* view);

        virtual void Patch(const json& widgetPatchDef, ReactImgui* view);
};

class StyledWidget : public Widget {
    public:
        std::optional<std::unique_ptr<BaseStyle>> m_style;

        static std::optional<BaseStyle> ExtractStyle(const json& widgetDef, ReactImgui* view);

        StyledWidget(int id) : Widget(id) {}

        StyledWidget(int id, std::optional<BaseStyle>& maybeStyle) : Widget(id) {
            if (maybeStyle.has_value()) {
                m_style.emplace(std::make_unique<BaseStyle>(maybeStyle.value()));
            }
        }

        void PreRender(ReactImgui* view);

        void PostRender(ReactImgui* view);

        bool HasCustomStyles();

        bool HasCustomFont(ReactImgui* view);

        bool HasCustomColors();

        virtual bool HasCustomWidth();

        virtual bool HasCustomHeight();

        float GetComputedWidth(ReactImgui* view);

        float GetComputedHeight(ReactImgui* view);

        bool HasCustomStyleVars();

        bool HasCustomHorizontalAlignment();

        bool HasRightHorizontalAlignment();

        void ReplaceStyle(BaseStyle& newStyle);

        virtual void Patch(const json& widgetPatchDef, ReactImgui* view);
};

class Fragment final : public Widget {
    public:
        static std::unique_ptr<Fragment> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                
                return std::make_unique<Fragment>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Fragment(int id) : Widget(id) {
            m_type = "Fragment";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class Group final : public StyledWidget {
    public:
        static std::unique_ptr<Group> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<Group>(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Group(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Group";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class Window final : public StyledWidget {
    public:
        ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
        bool m_open;
        std::string m_title;
        float m_width;
        float m_height;

        static std::unique_ptr<Window> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto title = widgetDef["title"].template get<std::string>();
                auto width = widgetDef["width"].template get<float>();
                auto height = widgetDef["height"].template get<float>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<Window>(id, title, width, height, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Window(int id, std::string title, float width, float height, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Window";
            m_handlesChildrenWithinRenderMethod = true;

            m_title = title;
            m_width = width;
            m_height = height;
            m_open = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef["title"].is_string()) {
                    m_title = widgetPatchDef["title"].template get<std::string>();
                }
                if (widgetPatchDef["width"].is_string()) {
                    m_width = widgetPatchDef["width"].template get<float>();
                }
                if (widgetPatchDef["height"].is_string()) {
                    m_height = widgetPatchDef["height"].template get<float>();
                }
            }
        }
};

class Child final : public StyledWidget {
    public:
        ImGuiChildFlags m_flags = ImGuiChildFlags_None;
        ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_None;
        float m_width;
        float m_height;

        static std::unique_ptr<Child> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                float width = 0;
                auto height = 0;

                if (widgetDef.contains("width") && widgetDef["width"].is_number()) {
                    width = widgetDef["width"].template get<float>();
                }

                if (widgetDef.contains("height") && widgetDef["height"].is_number()) {
                    height = widgetDef["height"].template get<float>();
                }

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<Child>(id, width, height, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Child(int id, float width, float height, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Child";
            m_handlesChildrenWithinRenderMethod = true;

            m_width = width;
            m_height = height;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef["width"].is_string()) {
                    m_width = widgetPatchDef["width"].template get<float>();
                }
                if (widgetPatchDef["height"].is_string()) {
                    m_height = widgetPatchDef["height"].template get<float>();
                }
            }
        }
};

class SameLine final : public Widget {
    public:
        static std::unique_ptr<SameLine> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                
                return std::make_unique<SameLine>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        SameLine(int id) : Widget(id) {
            m_type = "SameLine";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {}
};

class Separator final : public StyledWidget {
    public:
        static std::unique_ptr<Separator> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<Separator>(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Separator(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {}

        void Render(ReactImgui* view);
};

class Indent final : public StyledWidget {
    public:
        static std::unique_ptr<Indent> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<Indent>(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Indent(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Indent";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class SeparatorText final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<SeparatorText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<SeparatorText>(id, label, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        SeparatorText(int id, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "SeparatorText";
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class BulletText final : public StyledWidget {
    public:
        std::string m_text;

        static std::unique_ptr<BulletText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string text = widgetDef["text"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<BulletText>(id, text, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        BulletText(int id, std::string& text, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "BulletText";
            m_text = text;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("text") && widgetPatchDef["text"].is_string()) {
                    m_text = widgetPatchDef["text"].template get<std::string>();
                }
            }
        }
};

class UnformattedText final : public StyledWidget {
    public:
        std::string m_text;

        static std::unique_ptr<UnformattedText> makeWidget(const json& widgetDef, ReactImgui* view);

        UnformattedText(int id, std::string& text, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "UnformattedText";
            m_text = text;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("text") && widgetPatchDef["text"].is_string()) {
                    m_text = widgetPatchDef["text"].template get<std::string>();
                }
            }
        }
};

class DisabledText final : public StyledWidget {
    public:
        std::string m_text;

        static std::unique_ptr<DisabledText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string text = widgetDef["text"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<DisabledText>(id, text, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        DisabledText(int id, std::string& text, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "DisabledText";
            m_text = text;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("text") && widgetPatchDef["text"].is_string()) {
                    m_text = widgetPatchDef["text"].template get<std::string>();
                }
            }
        }
};

class TabBar final : public StyledWidget {
    public:
        static std::unique_ptr<TabBar> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<TabBar>(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabBar(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "TabBar";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class TabItem final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<TabItem> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<TabItem>(id, label, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabItem(int id, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "TabItem";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class CollapsingHeader final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<CollapsingHeader> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<CollapsingHeader>(id, label, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        CollapsingHeader(int id, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "CollapsingHeader";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class TextWrap final : public StyledWidget {
    public:
        double m_width;

        static std::unique_ptr<TextWrap> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                double width = widgetDef["width"].template get<double>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<TextWrap>(id, width, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TextWrap(int id, double& width, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "TextWrap";
            m_handlesChildrenWithinRenderMethod = true;
            m_width = width;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("width") && widgetPatchDef["width"].is_string()) {
                    m_width = widgetPatchDef["width"].template get<double>();
                }
            }
        }
};

class ItemTooltip final : public StyledWidget {
    public:
        static std::unique_ptr<ItemTooltip> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);
                
                return std::make_unique<ItemTooltip>(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        ItemTooltip(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "ItemTooltip";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);
};

class TreeNode final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<TreeNode> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return std::make_unique<TreeNode>(id, label, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TreeNode(int id, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "TreeNode";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class Combo final : public StyledWidget {
    protected:
        Combo(int id, std::string& label, int defaultValue, const json& options, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Combo";
            m_selectedIndex = defaultValue;
            m_label = label;
            m_itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);
        }
        
        Combo(int id, std::string& label, int defaultValue, std::string& optionsList, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Combo";
            m_selectedIndex = defaultValue;
            m_label = label;
            m_itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(optionsList);
        }

        /**
         * Takes array of [{value: number; label: string}] and converts into "label\0label\0label\0" (double NULL character at the end)
         * because this is what ImGui:Combo() expects
        */
        inline static std::unique_ptr<char[]> getItemsSeparatedByZeros(const json& options) {
            auto optionsIterator = options.items();
            int itemsStringLength = options.size() + 1; // Account for one NULL character for each option plus the final NULL character

            for (auto it = optionsIterator.begin(); it != optionsIterator.end(); ++it) {
                itemsStringLength += strlen(it.value()["label"].template get<std::string>().c_str());
            }

            auto itemsSeparatedByZerosUniquePtr = std::make_unique<char[]>(itemsStringLength);
            auto itemsSeparatedByZeros = itemsSeparatedByZerosUniquePtr.get();

            int offset = 0;

            for (auto it = optionsIterator.begin(); it != optionsIterator.end(); ++it) {
                
                auto label = it.value()["label"].template get<std::string>().c_str();

                memcpy(&itemsSeparatedByZeros[offset], label, strlen(label) + 1);

                offset += strlen(label) + 1;
            }

            return itemsSeparatedByZerosUniquePtr;
        }

        /**
         * Takes comma-delimited string of options "label,label,label" and converts into "label\0label\0label\0" (double NULL character at the end)
         * because this is what ImGui:Combo() expects
        */
        inline static std::unique_ptr<char[]> getItemsSeparatedByZeros(std::string& optionsList) {
            std::string delimiter = ",";

            int itemsStringLength = 1;  // Account for final NULL character
            std::vector<std::string> options;
            std::string token;
            std::stringstream input;
            input << optionsList;

            while (std::getline(input, token, ',')) {
                options.push_back(token);
                itemsStringLength += strlen(token.c_str()) + 1; // Account for NULL character
            }

            auto itemsSeparatedByZerosUniquePtr = std::make_unique<char[]>(itemsStringLength);
            auto itemsSeparatedByZeros = itemsSeparatedByZerosUniquePtr.get();
            
            int offset = 0;

            for (auto it = options.begin(); it != options.end(); ++it) {

                auto label = it->c_str();

                memcpy(&itemsSeparatedByZeros[offset], label, strlen(label) + 1);

                offset += strlen(label) + 1;
            }

            return itemsSeparatedByZerosUniquePtr;
        }

    public:
        int m_selectedIndex;
        std::string m_label;
        std::unique_ptr<char[]> m_itemsSeparatedByZeros; // Relevant for 'basic' combo only

        static std::unique_ptr<Combo> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto defaultValue = widgetDef.contains("defaultValue") && widgetDef["defaultValue"].is_number() ? widgetDef["defaultValue"].template get<int>() : 0;
                auto label = widgetDef["label"].template get<std::string>();
                auto optionsList = widgetDef["optionsList"].template get<std::string>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return Combo::makeWidget(id, label, defaultValue, optionsList, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Combo> makeWidget(int id, std::string& label, int defaultValue, const json& options, std::optional<BaseStyle>& style) {
            Combo instance(id, label, defaultValue, options, style);
            return std::make_unique<Combo>(std::move(instance));
        }

        static std::unique_ptr<Combo> makeWidget(int id, std::string& label, int defaultValue, std::string optionsList, std::optional<BaseStyle>& style) {
            Combo instance(id, label, defaultValue, optionsList, style);
            return std::make_unique<Combo>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class InputText final : public StyledWidget {
    protected:
        inline static ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        static int InputTextCb(ImGuiInputTextCallbackData* data);

        InputText(int id, std::string& defaultValue, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "InputText";
            m_bufferPointer = std::make_unique<char[]>(100);
            m_defaultValue = defaultValue;
            m_label = label;

            strncpy(m_bufferPointer.get(), defaultValue.c_str(), 99);
        }

    public:
        std::unique_ptr<char[]> m_bufferPointer;
        std::string m_defaultValue;
        std::string m_label;

        inline static std::unique_ptr<InputText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto defaultValue = widgetDef.contains("defaultValue") && widgetDef["defaultValue"].is_string() ? widgetDef["defaultValue"].template get<std::string>() : "";
                auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return InputText::makeWidget(id, defaultValue, label, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<InputText> makeWidget(int id, std::string& defaultValue, std::string& label, std::optional<BaseStyle>& style) {
            InputText instance(id, defaultValue, label, style);
            return std::make_unique<InputText>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
        
};

class Checkbox final : public StyledWidget {
    protected:
        Checkbox(int id, std::string& label, bool defaultChecked, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Checkbox";
            m_checked = defaultChecked;
            m_label = label;
        }

    public:
        bool m_checked;
        std::string m_label;

        inline static std::unique_ptr<Checkbox> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto defaultChecked = widgetDef.contains("defaultChecked") && widgetDef["defaultChecked"].is_boolean() ? widgetDef["defaultChecked"].template get<bool>() : false;
                auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return Checkbox::makeWidget(id, label, defaultChecked, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<Checkbox> makeWidget(int id, std::string& label, bool defaultChecked, std::optional<BaseStyle>& style) {
            Checkbox instance(id, label, defaultChecked, style);
            return std::make_unique<Checkbox>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class Button final : public StyledWidget {
    protected:
        Button(int id, std::string& label, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Button";
            m_label = label;
            m_size = ImVec2();

            UpdateSize();
        }

        void UpdateSize() {
            if (m_style.has_value()) {
                if (m_style.value()->maybeWidth.has_value()) {
                    m_size.x = m_style.value()->maybeWidth.value();
                }
                if (m_style.value()->maybeHeight.has_value()) {
                    m_size.y = m_style.value()->maybeHeight.value();
                }
            }
        }

    public:
        std::string m_label;

        ImVec2 m_size;

        static std::unique_ptr<Button> makeWidget(const json& widgetDef, ReactImgui* view);

        inline static std::unique_ptr<Button> makeWidget(int id, std::string& label, std::optional<BaseStyle>& style) {
            Button instance(id, label, style);
            return std::make_unique<Button>(std::move(instance));
        }

        ImGuiCol GetImGuiCol();

        void Render(ReactImgui* view);

        bool HasCustomWidth();

        bool HasCustomHeight();

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);
                UpdateSize();

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
            }
        }
};

class Slider final : public StyledWidget {
    protected:
        Slider(int id, std::string& label, float defaultValue, float min, float max, std::string& sliderType, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Slider";
            m_sliderType = sliderType;
            m_label = label;
            m_value = defaultValue;
            m_min = min;
            m_max = max;
        }

    public:
        std::string m_sliderType;
        float m_value;
        float m_min;
        float m_max;
        std::string m_label;

        inline static std::unique_ptr<Slider> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto defaultValue = widgetDef.contains("defaultValue") && widgetDef["defaultValue"].is_number() ? widgetDef["defaultValue"].template get<float>() : 0.0f;
                auto min = widgetDef.contains("min") && widgetDef["min"].is_number() ? widgetDef["min"].template get<float>() : 0.0f;
                auto max = widgetDef.contains("max") && widgetDef["max"].is_number() ? widgetDef["max"].template get<float>() : 10.0f;
                auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";
                auto sliderType = widgetDef.contains("sliderType") && widgetDef["sliderType"].is_string() ? widgetDef["sliderType"].template get<std::string>() : "default";

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return Slider::makeWidget(id, label, defaultValue, min, max, sliderType, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<Slider> makeWidget(int id, std::string& label, float defaultValue, float min, float max, std::string& sliderType, std::optional<BaseStyle>& style) {
            Slider instance(id, label, defaultValue, min, max, sliderType, style);
            return std::make_unique<Slider>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
                if (widgetPatchDef.contains("min") && widgetPatchDef["min"].is_number()) {
                    m_min = widgetPatchDef["min"].template get<float>();
                }
                if (widgetPatchDef.contains("max") && widgetPatchDef["max"].is_number()) {
                    m_max = widgetPatchDef["max"].template get<float>();
                }
            }
        }
};

class MultiSlider final : public StyledWidget {
    protected:
        MultiSlider(int id, std::string& label, float min, float max, int numValues, int decimalDigits, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "MultiSlider";
            m_label = label;
            m_numValues = numValues;
            m_values = std::make_unique<float[]>(numValues);
            m_min = min;
            m_max = max;
            m_decimalDigits = decimalDigits;
        }

    public:
        std::unique_ptr<float[]> m_values;
        float m_min;
        float m_max;
        int m_numValues;
        int m_decimalDigits;
        std::string m_label;

        inline static std::unique_ptr<MultiSlider> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto numValues = widgetDef.contains("numValues") && widgetDef["numValues"].is_number() ? widgetDef["numValues"].template get<int>() : 2;
                auto decimalDigits = widgetDef.contains("decimalDigits") && widgetDef["decimalDigits"].is_number() ? widgetDef["decimalDigits"].template get<int>() : 0;
                
                auto min = widgetDef.contains("min") && widgetDef["min"].is_number() ? widgetDef["min"].template get<float>() : 0.0f;
                auto max = widgetDef.contains("max") && widgetDef["max"].is_number() ? widgetDef["max"].template get<float>() : 10.0f;
                auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                
                if (widgetDef.contains("defaultValues") && widgetDef["defaultValues"].is_array() && widgetDef["defaultValues"].size() == numValues) {
                    return MultiSlider::makeWidget(id, label, min, max, numValues, decimalDigits, widgetDef["defaultValues"], style);
                } else {
                    return MultiSlider::makeWidget(id, label, min, max, numValues, decimalDigits, style);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<MultiSlider> makeWidget(int id, std::string& label, float min, float max, int numValues, int decimalDigits, const json& defaultValues, std::optional<BaseStyle>& style) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits, style);

            for (auto& [key, item] : defaultValues.items()) {
                instance.m_values[stoi(key)] = item.template get<float>();
            }

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static std::unique_ptr<MultiSlider> makeWidget(int id, std::string& label, float min, float max, int numValues, int decimalDigits, std::optional<BaseStyle>& style) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits, style);

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);

                if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
                    m_label = widgetPatchDef["label"].template get<std::string>();
                }
                if (widgetPatchDef.contains("min") && widgetPatchDef["min"].is_number()) {
                    m_min = widgetPatchDef["min"].template get<float>();
                }
                if (widgetPatchDef.contains("max") && widgetPatchDef["max"].is_number()) {
                    m_max = widgetPatchDef["max"].template get<float>();
                }
                if (widgetPatchDef.contains("decimalDigits") && widgetPatchDef["decimalDigits"].is_number()) {
                    m_decimalDigits = widgetPatchDef["decimalDigits"].template get<int>();
                }
            }
        }
};

// todo: for those use cases where we expect large quantities of data, should we preallocate?
class Table final : public StyledWidget {
    typedef struct { 
        std::optional<std::string> fieldId;
        std::string heading;
    } TableColumn;

    protected:
        ImGuiTableFlags m_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        Table(int id, std::vector<TableColumn>& columns, std::optional<int> clipRows, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "Table";
            m_columns = columns;
            m_clipRows = 0;

            if (clipRows.has_value()) {
                m_clipRows = clipRows.value();
            }
        }

    public:
        TableData m_data;
        std::vector<TableColumn> m_columns;
        int m_clipRows;

        inline static std::unique_ptr<Table> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                if (widgetDef.contains("columns") && widgetDef["columns"].is_array()) {
                    std::optional<int> clipRows;
                    std::vector<TableColumn> columns;

                    if (widgetDef.contains("clipRows") && widgetDef["clipRows"].is_number_integer()) {
                        clipRows.emplace(widgetDef["clipRows"].template get<int>());
                    }

                    for (auto& [key, item] : widgetDef["columns"].items()) {
                        columns.push_back({
                            std::make_optional(item["fieldId"].template get<std::string>()),
                            item["heading"].template get<std::string>()
                        });
                    }

                    auto style = StyledWidget::ExtractStyle(widgetDef, view);

                    return Table::makeWidget(id, columns, clipRows, style);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Table> makeWidget(int id, std::vector<TableColumn>& columns, std::optional<int> clipRows, std::optional<BaseStyle>& style) {
            Table instance(id, columns, clipRows, style);

            return std::make_unique<Table>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);
                // not sure what can be patched - presumably the columns? though that'd likely force us to clear the data
            }
        }

        void SetData(TableData& data) {
            m_data.clear();
            AppendData(data);
        }

        void AppendData(TableData& data) {
            m_data.insert(m_data.end(), data.begin(), data.end());

        }
};

// todo: should we preallocate buffer size?
class ClippedMultiLineTextRenderer final : public StyledWidget {
    protected:
        ClippedMultiLineTextRenderer(int id, std::optional<BaseStyle>& style) : StyledWidget(id, style) {
            m_type = "ClippedMultiLineTextRenderer";
        }

    public:
        ImVector<int> m_lineOffsets;
        ImGuiTextBuffer m_textBuffer;

        inline static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                auto style = StyledWidget::ExtractStyle(widgetDef, view);

                return ClippedMultiLineTextRenderer::makeWidget(id, style);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(int id, std::optional<BaseStyle>& style) {
            ClippedMultiLineTextRenderer instance(id, style);

            return std::make_unique<ClippedMultiLineTextRenderer>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& widgetPatchDef, ReactImgui* view) {
            if (widgetPatchDef.is_object()) {
                StyledWidget::Patch(widgetPatchDef, view);
                // not sure what can be patched
            }
        }

        void Clear() {
            m_lineOffsets.clear();
            m_lineOffsets.push_back(0);
            m_textBuffer.clear();
        }

        void SetData(const char* str) {
            Clear();
            AppendText(str);
        }

        void AppendText(const char* str) {
            int old_size = m_textBuffer.size();

            m_textBuffer.append(str);

            for (int new_size = m_textBuffer.size(); old_size < new_size; old_size++)
                if (m_textBuffer[old_size] == '\n')
                    m_lineOffsets.push_back(old_size + 1);
        }
};

class Map final : public Widget {
    private:
        ImVec2 m_offset;

    public:
        static std::unique_ptr<Map> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                
                return std::make_unique<Map>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Map(int id) : Widget(id) {
            m_type = "Map";

            m_offset = ImVec2(0.0f, 0.0f);
        }

        void Render(ReactImgui* view);
};
