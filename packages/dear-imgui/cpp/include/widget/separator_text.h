#include "styled_widget.h"

class SeparatorText final : public StyledWidget {
public:
    std::string m_label;

    static std::unique_ptr<SeparatorText> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();
            auto label = widgetDef["label"].template get<std::string>();

            return std::make_unique<SeparatorText>(view, id, label, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
        YGSize size{};
        const auto context = YGNodeGetContext(node);
        if (context) {
            const auto widget = static_cast<SeparatorText*>(context);

            size.width = width; // TODO: sure about that?
            size.height = widget->m_view->GetWidgetFontSize(widget);
        }

        return size;
    }

    SeparatorText(ReactImgui* view, const int id, const std::string& label, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {
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
