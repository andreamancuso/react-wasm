

#include "styled_widget.h"

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