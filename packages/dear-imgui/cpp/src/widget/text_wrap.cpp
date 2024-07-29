
#include "widget/text_wrap.h"

TextWrap::TextWrap(ReactImgui* view, const int id, const float& width, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "TextWrap";
    m_handlesChildrenWithinRenderMethod = true;
    m_width = width;
}

void TextWrap::Render(ReactImgui* view) {
    ImGui::PushTextWrapPos(m_width);
    
    Widget::HandleChildren(view);

    ImGui::PopTextWrapPos();
};

void TextWrap::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("width") && widgetPatchDef["width"].is_string()) {
            m_width = widgetPatchDef["width"].template get<float>();
        }
    }
};