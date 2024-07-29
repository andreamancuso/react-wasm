#include <optional>
#include <cstring>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "imgui.h"
#include <nlohmann/json.hpp>

#include <reactimgui.h>
#include <element/element.h>

using json = nlohmann::json;

#pragma once

class ReactImgui;

struct BaseStyle {
    std::optional<StyleColors> maybeColors;
    std::optional<StyleVars> maybeStyleVars;
    std::optional<int> maybeFontIndex;
    std::optional<HorizontalAlignment> maybeHorizontalAlignment;
};

class Widget : public Element {
    public:
        std::string m_type;

        // todo: does this belong here?
        inline static OnTextChangedCallback onInputTextChange_;

        explicit Widget(ReactImgui* view, int id);

        const char* GetElementType() override;

        void HandleChildren(ReactImgui* view) override;

        void SetChildrenDisplay(ReactImgui* view, YGDisplay display) const;

        void SetChildrenAsDisplayFlex(ReactImgui* view) const;

        void PreRender(ReactImgui* view) override;

        void Render(ReactImgui* view) override;

        void PostRender(ReactImgui* view) override;

        void Patch(const json& elementPatchDef, ReactImgui* view) override;
};

class StyledWidget : public Widget {
    public:
        std::optional<std::unique_ptr<BaseStyle>> m_style;

        static std::optional<BaseStyle> ExtractStyle(const json& widgetDef, ReactImgui* view);

        explicit StyledWidget(ReactImgui* view, int id);

        StyledWidget(ReactImgui* view, int id, std::optional<BaseStyle>& maybeStyle);

        void PreRender(ReactImgui* view) override;

        void PostRender(ReactImgui* view) override;

        [[nodiscard]] bool HasCustomStyles() const;

        bool HasCustomFont(ReactImgui* view) const;

        [[nodiscard]] bool HasCustomColors() const;

        virtual bool HasCustomWidth();

        virtual bool HasCustomHeight();

        [[nodiscard]] bool HasCustomStyleVars() const;

        [[nodiscard]] bool HasCustomHorizontalAlignment() const;

        [[nodiscard]] bool HasRightHorizontalAlignment() const;

        void ReplaceStyle(BaseStyle& newStyle);

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        [[nodiscard]] bool HasCustomStyleVar(ImGuiStyleVar key) const;

        [[nodiscard]] StyleVarValue& GetCustomStyleVar(ImGuiStyleVar key) const;
};

class Group final : public StyledWidget {
    public:
        static std::unique_ptr<Group> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                return std::make_unique<Group>(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Group(ReactImgui* view, int id, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;
};

class Window final : public StyledWidget {
    public:
        ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
        bool m_open;
        std::string m_title;
        float m_width;
        float m_height;

        static std::unique_ptr<Window> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto title = widgetDef["title"].template get<std::string>();
                auto width = widgetDef["width"].template get<float>();
                auto height = widgetDef["height"].template get<float>();

                return std::make_unique<Window>(view, id, title, width, height, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Window(ReactImgui* view, int id, const std::string& title, float width, float height, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;
};

class Child final : public StyledWidget {
    public:
        ImGuiChildFlags m_flags = ImGuiChildFlags_None;
        ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_None;
        float m_width;
        float m_height;

        static std::unique_ptr<Child> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                float width = 0;
                float height = 0;

                if (widgetDef.contains("width") && widgetDef["width"].is_number()) {
                    width = widgetDef["width"].template get<float>();
                }

                if (widgetDef.contains("height") && widgetDef["height"].is_number()) {
                    height = widgetDef["height"].template get<float>();
                }

                return std::make_unique<Child>(view, id, width, height, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        Child(ReactImgui* view, int id, float width, float height, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;
};

class Separator final : public StyledWidget {
    public:
        static std::unique_ptr<Separator> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                return std::make_unique<Separator>(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static YGSize Measure(YGNodeConstRef node, const float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            YGSize size;
            
            size.width = width;
            // TODO: we likely need to compute this based on the associated font, based on the widget's style
            size.height = 1.0f;

            return size;
        }

        Separator(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {}

        void Render(ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class SeparatorText final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<SeparatorText> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto label = widgetDef["label"].template get<std::string>();

                return std::make_unique<SeparatorText>(view, id, label, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<SeparatorText*>(YGNodeGetContext(node));

            YGSize size;
            
            size.width = width; // TODO: sure about that?
            size.height = widget->m_view->GetWidgetFontSize(widget);

            return size;
        }

        SeparatorText(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "SeparatorText";
            m_label = label;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class Text : public StyledWidget {
    public:
        std::string m_text;

        Text(ReactImgui* view, int id, const std::string& text, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_text = text;
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<Text*>(YGNodeGetContext(node));
        
            YGSize size;
            
            size.width = widget->m_view->CalcTextSize(widget, widget->m_text.c_str()).x;
            size.height = widget->m_view->GetWidgetFontSize(widget);

            return size;
        }

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class BulletText final : public Text {
    public:
        static std::unique_ptr<BulletText> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view);

        BulletText(ReactImgui* view, const int id, const std::string& text, std::optional<BaseStyle>& style) : Text(view, id, text, style) {
            m_type = "BulletText";
        }

        void Render(ReactImgui* view) override;
};

class UnformattedText final : public Text {
    public:
        static std::unique_ptr<UnformattedText> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view);

        UnformattedText(ReactImgui* view, const int id, const std::string& text, std::optional<BaseStyle>& style) : Text(view, id, text, style) {
            m_type = "UnformattedText";
        }

        void Render(ReactImgui* view) override;
};

class DisabledText final : public Text {
    public:
        static std::unique_ptr<DisabledText> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view);

        DisabledText(ReactImgui* view, const int id, const std::string& text, std::optional<BaseStyle>& style) : Text(view, id, text, style) {
            m_type = "DisabledText";
        }

        void Render(ReactImgui* view) override;
};

class TabBar final : public StyledWidget {
    public:
        static std::unique_ptr<TabBar> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                return std::make_unique<TabBar>(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabBar(ReactImgui* view, int id, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;
        
        void Init() override {
            YGNodeStyleSetPadding(m_layoutNode->m_node, YGEdgeTop, 25.0f);
        }
};

class TabItem final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<TabItem> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                auto label = widgetDef["label"].template get<std::string>();

                return std::make_unique<TabItem>(view, id, label, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TabItem(ReactImgui* view, int id, const std::string& label, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;
};

class CollapsingHeader final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<CollapsingHeader> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                const auto label = widgetDef["label"].template get<std::string>();

                return std::make_unique<CollapsingHeader>(view, id, label, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        CollapsingHeader(ReactImgui* view, int id, const std::string& label, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;
};

class TextWrap final : public StyledWidget {
    public:
        float m_width;

        static std::unique_ptr<TextWrap> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                double width = widgetDef["width"].template get<double>();

                return std::make_unique<TextWrap>(view, id, width, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TextWrap(ReactImgui* view, int id, const float& width, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;
};

class ItemTooltip final : public StyledWidget {
    public:
        static std::unique_ptr<ItemTooltip> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();

                return std::make_unique<ItemTooltip>(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        ItemTooltip(ReactImgui* view, int id, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;
};

class TreeNode final : public StyledWidget {
    public:
        std::string m_label;

        static std::unique_ptr<TreeNode> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                std::string label = widgetDef["label"].template get<std::string>();

                return std::make_unique<TreeNode>(view, id, label, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        TreeNode(ReactImgui* view, int id, const std::string& label, std::optional<BaseStyle>& style);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;
};

class Combo final : public StyledWidget {
    protected:
        Combo(ReactImgui* view, const int id, const std::string& placeholder, const int initialSelectedIndex, const std::vector<std::string>& options, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "Combo";
            m_selectedIndex = initialSelectedIndex;
            m_placeholder = placeholder;
            m_options = options;
        }

    public:
        int m_selectedIndex = -1;
        std::string m_placeholder;
        std::vector<std::string> m_options;

        static std::unique_ptr<Combo> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();
                const auto initialSelectedIndex = widgetDef.contains("initialSelectedIndex") && widgetDef["initialSelectedIndex"].is_number()
                    ? widgetDef["initialSelectedIndex"].template get<int>()
                    : -1;
                const auto placeholder = widgetDef.contains("placeholder") ? widgetDef["placeholder"].template get<std::string>() : "";

                std::vector<std::string> options;

                if (widgetDef.contains("options") && widgetDef["options"].is_array()) {
                    for (const auto &[key, item] : widgetDef["options"].items()) {
                        if (item.is_string()) {
                            options.emplace_back(item.template get<std::string>());
                        }
                    }
                }

                return makeWidget(view, id, placeholder, initialSelectedIndex, options, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Combo> makeWidget(ReactImgui* view, const int id, const std::string& placeholder, const int initialSelectedIndex, const std::vector<std::string>& options, std::optional<BaseStyle>& style) {
            Combo instance(view, id, placeholder, initialSelectedIndex, options, style);
            return std::make_unique<Combo>(std::move(instance));
        }

        static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<SeparatorText*>(YGNodeGetContext(node));

            YGSize size;

            // TODO: we may want to define default widths similarly to how browsers do
            size.width = widget->m_view->GetWidgetFontSize(widget) * 10;
            // TODO: we likely need to compute this based on the associated font, based on the widget's style
            size.height = widget->m_view->GetFrameHeight(widget);

            return size;
        }

        void SetOptions(const json& options) {
            m_options.clear();

            if (options.is_array()) {
                for (const auto &[key, item] : options.items()) {
                    if (item.is_string()) {
                        m_options.emplace_back(item.template get<std::string>());
                    }
                }
            }
        }

        void SetOptions(const std::vector<std::string>& options) {
            m_options = options;
        }

        void SetSelectedIndex(int selectedIndex) {
            m_selectedIndex = selectedIndex;
        }

        void ClearSelectedIndex() {
            m_selectedIndex = -1;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class InputText final : public StyledWidget {
    protected:
        inline static ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_CallbackEdit | ImGuiInputTextFlags_NoUndoRedo;

        static int InputTextCb(ImGuiInputTextCallbackData* data);

        InputText(ReactImgui* view, const int id, const std::string& defaultValue, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "InputText";
            m_bufferPointer = std::make_unique<char[]>(100);
            m_defaultValue = defaultValue;
            m_label = label;

            if (!defaultValue.empty()) {
                strncpy(m_bufferPointer.get(), defaultValue.c_str(), 99);
            }
        }

    public:
        std::unique_ptr<char[]> m_bufferPointer;
        std::string m_defaultValue;
        std::string m_label;

        static std::unique_ptr<InputText> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();
                const auto defaultValue = widgetDef.contains("defaultValue") && widgetDef["defaultValue"].is_string() ? widgetDef["defaultValue"].template get<std::string>() : "";
                const auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                return makeWidget(view, id, defaultValue, label, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<InputText> makeWidget(ReactImgui* view, const int id, const std::string& defaultValue, const std::string& label, std::optional<BaseStyle>& style) {
            InputText instance(view, id, defaultValue, label, style);
            return std::make_unique<InputText>(std::move(instance));
        }

        static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<InputText*>(YGNodeGetContext(node));

            YGSize size;
            
            // TODO: we may want to define default widths similarly to how browsers do
            size.width = widget->m_view->GetWidgetFontSize(widget) * 10;
            // TODO: we likely need to compute this based on the associated font, based on the widget's style
            size.height = widget->m_view->GetFrameHeight(widget);

            return size;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class Checkbox final : public StyledWidget {
    protected:
        Checkbox(ReactImgui* view, const int id, const std::string& label, const bool defaultChecked, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "Checkbox";
            m_checked = defaultChecked;
            m_label = label;
        }

    public:
        bool m_checked;
        std::string m_label;

        static std::unique_ptr<Checkbox> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();
                const auto defaultChecked = widgetDef.contains("defaultChecked") && widgetDef["defaultChecked"].is_boolean() ? widgetDef["defaultChecked"].template get<bool>() : false;
                const auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                return makeWidget(view, id, label, defaultChecked, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Checkbox> makeWidget(ReactImgui* view, const int id,  const std::string& label, const bool defaultChecked, std::optional<BaseStyle>& style) {
            Checkbox instance(view, id, label, defaultChecked, style);
            return std::make_unique<Checkbox>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            auto widget = static_cast<Checkbox*>(YGNodeGetContext(node));

            YGSize size;

            const auto frameHeight = widget->m_view->GetFrameHeight(widget);

            size.width = frameHeight;
            size.height = frameHeight;

            return size;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class Button final : public StyledWidget {
    protected:
        Button(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "Button";
            m_label = label;
        }
    public:
        std::string m_label;

        static std::unique_ptr<Button> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view);

        static std::unique_ptr<Button> makeWidget(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) {
            Button instance(view, id, label, style);

            return std::make_unique<Button>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<Button*>(YGNodeGetContext(node));

            YGSize size;

            size.width = (widget->m_view->m_baseStyle.FramePadding.x * 2.0f) + widget->m_view->CalcTextSize(widget, widget->m_label.c_str()).x;
            size.height = widget->m_view->GetFrameHeight(widget);

            if (widget->HasCustomStyles() && widget->HasCustomStyleVar(ImGuiStyleVar_FramePadding)) {
                auto styleVar = widget->GetCustomStyleVar(ImGuiStyleVar_FramePadding);

                if (std::holds_alternative<ImVec2>(styleVar)) {
                    size.width = (std::get<ImVec2>(styleVar).x * 2.0f) + widget->m_view->CalcTextSize(widget, widget->m_label.c_str()).x;
                }
            }

            return size;
        }

        ImGuiCol GetImGuiCol();

        void Render(ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class Slider final : public StyledWidget {
    protected:
        Slider(ReactImgui* view, const int id, const std::string& label, const float defaultValue, const float min, const float max, const std::string& sliderType, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
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

        static std::unique_ptr<Slider> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();
                const auto defaultValue = widgetDef.contains("defaultValue") && widgetDef["defaultValue"].is_number() ? widgetDef["defaultValue"].template get<float>() : 0.0f;
                const auto min = widgetDef.contains("min") && widgetDef["min"].is_number() ? widgetDef["min"].template get<float>() : 0.0f;
                const auto max = widgetDef.contains("max") && widgetDef["max"].is_number() ? widgetDef["max"].template get<float>() : 10.0f;
                const auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";
                const auto sliderType = widgetDef.contains("sliderType") && widgetDef["sliderType"].is_string() ? widgetDef["sliderType"].template get<std::string>() : "default";

                return makeWidget(view, id, label, defaultValue, min, max, sliderType, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Slider> makeWidget(ReactImgui* view, const int id, const std::string& label, const float defaultValue, const float min, const float max, const std::string& sliderType, std::optional<BaseStyle>& style) {
            Slider instance(view, id, label, defaultValue, min, max, sliderType, style);
            return std::make_unique<Slider>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            auto widget = static_cast<Slider*>(YGNodeGetContext(node));
            YGSize size;

            size.width = 60.0f; // TODO: This is rather arbitrary
            size.height = widget->m_view->GetFrameHeight(widget);

            return size;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class MultiSlider final : public StyledWidget {
    protected:
        MultiSlider(ReactImgui* view, const int id, const std::string& label, const float min, const float max, const int numValues, const int decimalDigits, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
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

        static std::unique_ptr<MultiSlider> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();
                const auto numValues = widgetDef.contains("numValues") && widgetDef["numValues"].is_number() ? widgetDef["numValues"].template get<int>() : 2;
                const auto decimalDigits = widgetDef.contains("decimalDigits") && widgetDef["decimalDigits"].is_number() ? widgetDef["decimalDigits"].template get<int>() : 0;
                
                const auto min = widgetDef.contains("min") && widgetDef["min"].is_number() ? widgetDef["min"].template get<float>() : 0.0f;
                const auto max = widgetDef.contains("max") && widgetDef["max"].is_number() ? widgetDef["max"].template get<float>() : 10.0f;
                const auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

                if (widgetDef.contains("defaultValues") && widgetDef["defaultValues"].is_array() && widgetDef["defaultValues"].size() == numValues) {
                    return makeWidget(view, id, label, min, max, numValues, decimalDigits, widgetDef["defaultValues"], maybeStyle);
                } else {
                    return makeWidget(view, id, label, min, max, numValues, decimalDigits, maybeStyle);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<MultiSlider> makeWidget(ReactImgui* view, const int id, const std::string& label, const float min, const float max, const int numValues, const int decimalDigits, const json& defaultValues, std::optional<BaseStyle>& style) {
            MultiSlider instance(view, id, label, min, max, numValues, decimalDigits, style);

            for (auto& [key, item] : defaultValues.items()) {
                instance.m_values[stoi(key)] = item.template get<float>();
            }

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static std::unique_ptr<MultiSlider> makeWidget(ReactImgui* view, const int id, const std::string& label, const float min, const float max, const int numValues, const int decimalDigits, std::optional<BaseStyle>& style) {
            MultiSlider instance(view, id, label, min, max, numValues, decimalDigits, style);

            return std::make_unique<MultiSlider>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            const auto widget = static_cast<MultiSlider*>(YGNodeGetContext(node));

            YGSize size;

            size.width = static_cast<float>(widget->m_numValues) * 60.0f;
            size.height = widget->m_view->GetFrameHeight(widget);

            return size;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
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

        Table(ReactImgui* view, const int id, const std::vector<TableColumn>& columns, const std::optional<int> clipRows, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
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
        int m_clipRows; // todo: potentially redundant?

        static std::unique_ptr<Table> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();

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

                    return makeWidget(view, id, columns, clipRows, maybeStyle);
                }
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<Table> makeWidget(ReactImgui* view, const int id, const std::vector<TableColumn>& columns, std::optional<int> clipRows, std::optional<BaseStyle>& style) {
            Table instance(view, id, columns, clipRows, style);

            return std::make_unique<Table>(std::move(instance));
        }

        static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void SetData(TableData& data) {
            m_data.clear();
            AppendData(data);
        }

        void AppendData(TableData& data) {
            m_data.insert(m_data.end(), data.begin(), data.end());
        }

        // TODO: this is repeated a million times
        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

// todo: should we preallocate buffer size?
class ClippedMultiLineTextRenderer final : public StyledWidget {
    protected:
        ClippedMultiLineTextRenderer(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "ClippedMultiLineTextRenderer";
        }

    public:
        ImVector<int> m_lineOffsets;
        ImGuiTextBuffer m_textBuffer;

        static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                const auto id = widgetDef["id"].template get<int>();

                return makeWidget(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        static std::unique_ptr<ClippedMultiLineTextRenderer> makeWidget(ReactImgui* view, int id, std::optional<BaseStyle>& style) {
            ClippedMultiLineTextRenderer instance(view, id, style);

            return std::make_unique<ClippedMultiLineTextRenderer>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            auto widget = static_cast<ClippedMultiLineTextRenderer*>(YGNodeGetContext(node));
            YGSize size;

            size.width = width;
            size.height = widget->m_view->GetFrameHeight(widget) * 10.0f;

            return size;
        }

        void Render(ReactImgui* view) override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

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

            for (const int new_size = m_textBuffer.size(); old_size < new_size; old_size++)
                if (m_textBuffer[old_size] == '\n')
                    m_lineOffsets.push_back(old_size + 1);
        }

        void Init() override {
            Element::Init();

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};

class Map final : public StyledWidget {
    private:
        ImVec2 m_offset;

    public:
        static std::unique_ptr<Map> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
            if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
                auto id = widgetDef["id"].template get<int>();
                
                return std::make_unique<Map>(view, id, maybeStyle);
            }

            throw std::invalid_argument("Invalid JSON data");
        }

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;

        Map(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
            m_type = "Map";

            m_offset = ImVec2(0.0f, 0.0f);
        }

        void Render(ReactImgui* view) override;
};
