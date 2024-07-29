#include "widget/group.h"

Group::Group(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "Group";
    m_handlesChildrenWithinRenderMethod = true;
}

void Group::Render(ReactImgui* view) {
    ImGui::BeginGroup();
    Widget::HandleChildren(view);
    ImGui::EndGroup();
};