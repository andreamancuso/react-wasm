#include "styled_widget.h"

class ReactImgui;

class Button final : public StyledWidget {
    protected:
        Button(ReactImgui* view, const int id, const std::string& label, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {
            m_type = "di-button";
            m_label = label;
        }
    public:
        std::string m_label;

        static std::unique_ptr<Button> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view);

        static std::unique_ptr<Button> makeWidget(ReactImgui* view, const int id, const std::string& label, std::optional<WidgetStyle>& style) {
            Button instance(view, id, label, style);

            return std::make_unique<Button>(std::move(instance));
        }

        static YGSize Measure(const YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
            YGSize size{};
            const auto context = YGNodeGetContext(node);
            if (context) {
                const auto widget = static_cast<Button*>(context);

                size.width = (widget->m_view->m_widgetStyle.FramePadding.x * 2.0f) + widget->m_view->CalcTextSize(widget, widget->m_label.c_str()).x;
                size.height = widget->m_view->GetFrameHeight(widget);

                if (widget->HasCustomStyles() && widget->HasCustomStyleVar(ImGuiStyleVar_FramePadding)) {
                    auto styleVar = widget->GetCustomStyleVar(ImGuiStyleVar_FramePadding);

                    if (std::holds_alternative<ImVec2>(styleVar)) {
                        size.width = (std::get<ImVec2>(styleVar).x * 2.0f) + widget->m_view->CalcTextSize(widget, widget->m_label.c_str()).x;
                    }
                }
            }

            return size;
        }

        void Render(ReactImgui* view) override;

        bool HasCustomWidth() override;

        bool HasCustomHeight() override;

        void Patch(const json& widgetPatchDef, ReactImgui* view) override;

        void Init(const json& elementDef) override {
            Element::Init(elementDef);

            YGNodeSetContext(m_layoutNode->m_node, this);
            YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);
        }
};
