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

struct Style {
    std::optional<int> maybeFontIndex;
    std::optional<ImVec4> maybeColor;
};

class Widget {
    public:
        typedef std::tuple<std::optional<int>, std::optional<ImVec4>> StyleTuple;

        int m_id;
        std::string m_type;
        bool m_handlesChildrenWithinRenderMethod;
        // todo: still not entirely sure whether fontIndex is better than name+size combo
        std::unique_ptr<Style> m_style;

        // todo: does this belong here?
        inline static OnTextChangedCallback onInputTextChange_;

        static StyleTuple ExtractStyle(const json& widgetDef, ReactImgui* view);

        Widget(int id) {
            m_id = id;
            m_type = "Unknown";
            m_handlesChildrenWithinRenderMethod = false;

            m_style = std::make_unique<Style>();
        }

        void HandleChildren(ReactImgui* view);

        virtual void Render(ReactImgui* view) = 0;

        virtual void Patch(const json& val) = 0;

        virtual ImGuiCol GetImGuiCol();
};

class Fragment final : public Widget {
    public:
        static std::unique_ptr<Fragment> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<Fragment>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Fragment(int id) : Widget(id) {
            m_type = "Fragment";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class Group final : public Widget {
    public:
        static std::unique_ptr<Group> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<Group>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Group(int id) : Widget(id) {
            m_type = "Group";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class Window final : public Widget {
    public:
        ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
        bool m_open;
        std::string m_title;
        float m_width;
        float m_height;

        static std::unique_ptr<Window> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto title = val["title"].template get<std::string>();
                auto width = val["width"].template get<float>();
                auto height = val["height"].template get<float>();
                
                return std::make_unique<Window>(id, title, width, height);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Window(int id, std::string title, float width, float height) : Widget(id) {
            m_type = "Window";
            m_handlesChildrenWithinRenderMethod = true;

            m_title = title;
            m_width = width;
            m_height = height;
            m_open = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class Child final : public Widget {
    public:
        ImGuiChildFlags m_flags = ImGuiChildFlags_None;
        ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_None;
        float m_width;
        float m_height;

        static std::unique_ptr<Child> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                float width = 0;
                auto height = 0;

                if (val.contains("width") && val["width"].is_number()) {
                    width = val["width"].template get<float>();
                }

                if (val.contains("height") && val["height"].is_number()) {
                    height = val["height"].template get<float>();
                }
                
                return std::make_unique<Child>(id, width, height);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Child(int id, float width, float height) : Widget(id) {
            m_type = "Child";
            m_handlesChildrenWithinRenderMethod = true;

            m_width = width;
            m_height = height;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class SameLine final : public Widget {
    public:
        static std::unique_ptr<SameLine> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<SameLine>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        SameLine(int id) : Widget(id) {
            m_type = "SameLine";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class Separator final : public Widget {
    public:
        static std::unique_ptr<Separator> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<Separator>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Separator(int id) : Widget(id) {}

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class Indent final : public Widget {
    public:
        static std::unique_ptr<Indent> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<Indent>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Indent(int id) : Widget(id) {
            m_type = "Indent";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

// Likely unused
class Unindent final : public Widget {
    public:
        static std::unique_ptr<Unindent> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<Unindent>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Unindent(int id) : Widget(id) {
            m_type = "Unindent";
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class SeparatorText final : public Widget {
    public:
        std::string m_label;

        static std::unique_ptr<SeparatorText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                return std::make_unique<SeparatorText>(id, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        SeparatorText(int id, std::string& label) : Widget(id) {
            m_type = "SeparatorText";
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class BulletText final : public Widget {
    public:
        std::string m_text;

        static std::unique_ptr<BulletText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string text = widgetDef["text"].template get<std::string>();

                return std::make_unique<BulletText>(id, text);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        BulletText(int id, std::string& text) : Widget(id) {
            m_type = "BulletText";
            m_text = text;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("text") && val["text"].is_string()) {
                    m_text = val["text"].template get<std::string>();
                }
            }
        }
};

class UnformattedText final : public Widget {
    public:
        std::string m_text;

        static std::unique_ptr<UnformattedText> makeWidget(const json& widgetDef, ReactImgui* view);

        UnformattedText(int id, std::string& text, Widget::StyleTuple& style) : Widget(id) {
            m_type = "UnformattedText";
            m_text = text;
            m_style->maybeFontIndex = std::get<0>(style);
            m_style->maybeColor = std::get<1>(style);
        }

        ImGuiCol GetImGuiCol();

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("text") && val["text"].is_string()) {
                    m_text = val["text"].template get<std::string>();
                }
            }
        }
};

class DisabledText final : public Widget {
    public:
        std::string m_text;

        static std::unique_ptr<DisabledText> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string text = widgetDef["text"].template get<std::string>();

                return std::make_unique<DisabledText>(id, text);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        DisabledText(int id, std::string& text) : Widget(id) {
            m_type = "DisabledText";
            m_text = text;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("text") && val["text"].is_string()) {
                    m_text = val["text"].template get<std::string>();
                }
            }
        }
};

class TabBar final : public Widget {
    public:
        static std::unique_ptr<TabBar> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<TabBar>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabBar(int id) : Widget(id) {
            m_type = "TabBar";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class TabItem final : public Widget {
    public:
        std::string m_label;

        static std::unique_ptr<TabItem> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                return std::make_unique<TabItem>(id, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabItem(int id, std::string& label) : Widget(id) {
            m_type = "TabItem";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class CollapsingHeader final : public Widget {
    public:
        std::string m_label;

        static std::unique_ptr<CollapsingHeader> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                return std::make_unique<CollapsingHeader>(id, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        CollapsingHeader(int id, std::string& label) : Widget(id) {
            m_type = "CollapsingHeader";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class TextWrap final : public Widget {
    public:
        double m_width;

        static std::unique_ptr<TextWrap> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                double width = widgetDef["width"].template get<double>();

                return std::make_unique<TextWrap>(id, width);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TextWrap(int id, double& width) : Widget(id) {
            m_type = "TextWrap";
            m_handlesChildrenWithinRenderMethod = true;
            m_width = width;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("width") && val["width"].is_string()) {
                    m_width = val["width"].template get<double>();
                }
            }
        }
};

class ItemTooltip final : public Widget {
    public:
        static std::unique_ptr<ItemTooltip> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                
                return std::make_unique<ItemTooltip>(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        ItemTooltip(int id) : Widget(id) {
            m_type = "ItemTooltip";
            m_handlesChildrenWithinRenderMethod = true;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {}
};

class TreeNode final : public Widget {
    public:
        std::string m_label;

        static std::unique_ptr<TreeNode> makeWidget(const json& widgetDef, ReactImgui* view) {
            if (widgetDef.is_object()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                return std::make_unique<TreeNode>(id, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TreeNode(int id, std::string& label) : Widget(id) {
            m_type = "TreeNode";
            m_handlesChildrenWithinRenderMethod = true;
            m_label = label;
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class Combo final : public Widget {
    protected:
        Combo(int id, std::string& label, int defaultValue, const json& options) : Widget(id) {
            m_type = "Combo";
            m_selectedIndex = defaultValue;
            m_label = label;
            m_itemsSeparatedByZeros = Combo::getItemsSeparatedByZeros(options);
        }
        
        Combo(int id, std::string& label, int defaultValue, std::string& optionsList) : Widget(id) {
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

        static std::unique_ptr<Combo> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<int>() : 0;
                auto label = val["label"].template get<std::string>();
                auto optionsList = val["optionsList"].template get<std::string>();

                return Combo::makeWidget(id, label, defaultValue, optionsList);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Combo> makeWidget(int id, std::string& label, int defaultValue, const json& options) {
            Combo instance(id, label, defaultValue, options);
            return std::make_unique<Combo>(std::move(instance));
        }

        static std::unique_ptr<Combo> makeWidget(int id, std::string& label, int defaultValue, std::string optionsList) {
            Combo instance(id, label, defaultValue, optionsList);
            return std::make_unique<Combo>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class InputText final : public Widget {
    protected:
        inline static ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        static int InputTextCb(ImGuiInputTextCallbackData* data);

        InputText(int id, std::string& defaultValue, std::string& label) : Widget(id) {
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

        inline static std::unique_ptr<InputText> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_string() ? val["defaultValue"].template get<std::string>() : "";
                auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

                return InputText::makeWidget(id, defaultValue, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<InputText> makeWidget(int id, std::string& defaultValue, std::string& label) {
            InputText instance(id, defaultValue, label);
            return std::make_unique<InputText>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
        
};

class Checkbox final : public Widget {
    protected:
        Checkbox(int id, std::string& label, bool defaultChecked) : Widget(id) {
            m_type = "Checkbox";
            m_checked = defaultChecked;
            m_label = label;
        }

    public:
        bool m_checked;
        std::string m_label;

        inline static std::unique_ptr<Checkbox> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto defaultChecked = val.contains("defaultChecked") && val["defaultChecked"].is_boolean() ? val["defaultChecked"].template get<bool>() : false;
                auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";

                return Checkbox::makeWidget(id, label, defaultChecked);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<Checkbox> makeWidget(int id, std::string& label, bool defaultChecked) {
            Checkbox instance(id, label, defaultChecked);
            return std::make_unique<Checkbox>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class Button final : public Widget {
    protected:
        Button(int id, std::string& label) : Widget(id) {
            m_type = "Button";
            m_label = label;
        }

    public:
        std::string m_label;

        inline static std::unique_ptr<Button> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";
                
                return Button::makeWidget(id, label);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<Button> makeWidget(int id, std::string& label) {
            Button instance(id, label);
            return std::make_unique<Button>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
            }
        }
};

class Slider final : public Widget {
    protected:
        Slider(int id, std::string& label, float defaultValue, float min, float max, std::string& sliderType) : Widget(id) {
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

        inline static std::unique_ptr<Slider> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto defaultValue = val.contains("defaultValue") && val["defaultValue"].is_number() ? val["defaultValue"].template get<float>() : 0.0f;
                auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
                auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
                auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";
                auto sliderType = val.contains("sliderType") && val["sliderType"].is_string() ? val["sliderType"].template get<std::string>() : "default";

                return Slider::makeWidget(id, label, defaultValue, min, max, sliderType);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        inline static std::unique_ptr<Slider> makeWidget(int id, std::string& label, float defaultValue, float min, float max, std::string& sliderType) {
            Slider instance(id, label, defaultValue, min, max, sliderType);
            return std::make_unique<Slider>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
                if (val.contains("min") && val["min"].is_number()) {
                    m_min = val["min"].template get<float>();
                }
                if (val.contains("max") && val["max"].is_number()) {
                    m_max = val["max"].template get<float>();
                }
            }
        }
};

class MultiSlider final : public Widget {
    protected:
        MultiSlider(int id, std::string& label, float min, float max, int numValues, int decimalDigits) : Widget(id) {
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

        inline static std::unique_ptr<MultiSlider> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();
                auto numValues = val.contains("numValues") && val["numValues"].is_number() ? val["numValues"].template get<int>() : 2;
                auto decimalDigits = val.contains("decimalDigits") && val["decimalDigits"].is_number() ? val["decimalDigits"].template get<int>() : 0;
                
                auto min = val.contains("min") && val["min"].is_number() ? val["min"].template get<float>() : 0.0f;
                auto max = val.contains("max") && val["max"].is_number() ? val["max"].template get<float>() : 10.0f;
                auto label = val.contains("label") && val["label"].is_string() ? val["label"].template get<std::string>() : "";
                
                if (val.contains("defaultValues") && val["defaultValues"].is_array() && val["defaultValues"].size() == numValues) {
                    return MultiSlider::makeWidget(id, label, min, max, numValues, decimalDigits, val["defaultValues"]);
                } else {
                    return MultiSlider::makeWidget(id, label, min, max, numValues, decimalDigits);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<MultiSlider> makeWidget(int id, std::string& label, float min, float max, int numValues, int decimalDigits, const json& defaultValues) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            for (auto& [key, item] : defaultValues.items()) {
                instance.m_values[stoi(key)] = item.template get<float>();
            }

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static std::unique_ptr<MultiSlider> makeWidget(int id, std::string& label, float min, float max, int numValues, int decimalDigits) {
            MultiSlider instance(id, label, min, max, numValues, decimalDigits);

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                if (val.contains("label") && val["label"].is_string()) {
                    m_label = val["label"].template get<std::string>();
                }
                if (val.contains("min") && val["min"].is_number()) {
                    m_min = val["min"].template get<float>();
                }
                if (val.contains("max") && val["max"].is_number()) {
                    m_max = val["max"].template get<float>();
                }
                if (val.contains("decimalDigits") && val["decimalDigits"].is_number()) {
                    m_decimalDigits = val["decimalDigits"].template get<int>();
                }
            }
        }
};

// todo: for those use cases where we expect large quantities of data, should we preallocate?
class Table final : public Widget {
    typedef struct { 
        std::optional<std::string> fieldId;
        std::string heading;
    } TableColumn;

    protected:
        ImGuiTableFlags m_flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

        Table(int id, std::vector<TableColumn>& columns, std::optional<int> clipRows) : Widget(id) {
            m_type = "Table";
            m_columns = columns;
            m_clipRows = 0;

            if (clipRows.has_value()) {
                m_clipRows = clipRows.value();
            }
        }

    public:
        typedef std::unordered_map<std::string, std::string> TableRow;
        typedef std::vector<TableRow> TableData;

        TableData m_data;
        std::vector<TableColumn> m_columns;
        int m_clipRows;

        inline static std::unique_ptr<Table> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();

                if (val.contains("columns") && val["columns"].is_array()) {
                    std::optional<int> clipRows;
                    std::vector<TableColumn> columns;

                    if (val.contains("clipRows") && val["clipRows"].is_number_integer()) {
                        clipRows.emplace(val["clipRows"].template get<int>());
                    }

                    for (auto& [key, item] : val["columns"].items()) {
                        columns.push_back({
                            std::make_optional(item["fieldId"].template get<std::string>()),
                            item["heading"].template get<std::string>()
                        });
                    }

                    return Table::makeWidget(id, columns, clipRows);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Table> makeWidget(int id, std::vector<TableColumn>& columns, std::optional<int> clipRows) {
            Table instance(id, columns, clipRows);

            return std::make_unique<Table>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
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
class ClippedMultiLineTextRenderer final : public Widget {
    protected:
        ClippedMultiLineTextRenderer(int id) : Widget(id) {
            m_type = "ClippedMultiLineTextRenderer";
        }

    public:
        ImVector<int> m_lineOffsets;
        ImGuiTextBuffer m_textBuffer;

        inline static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(const json& val, ReactImgui* view) {
            if (val.is_object()) {
                auto id = val["id"].template get<int>();

                return ClippedMultiLineTextRenderer::makeWidget(id);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(int id) {
            ClippedMultiLineTextRenderer instance(id);

            return std::make_unique<ClippedMultiLineTextRenderer>(std::move(instance));
        }

        void Render(ReactImgui* view);

        void Patch(const json& val) {
            if (val.is_object()) {
                // not sure what can be patched - presumably the columns? though that'd likely force us to clear the data
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
