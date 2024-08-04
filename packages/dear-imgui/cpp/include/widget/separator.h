#include "styled_widget.h"

class Separator final : public StyledWidget {
public:
    static std::unique_ptr<Separator> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
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

    Separator(ReactImgui* view, const int id, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {}

    void Render(ReactImgui* view) override;

    void Init() override {
        Element::Init();

        YGNodeSetContext(m_layoutNode->m_node, this);
        YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
    }
};
