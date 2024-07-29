#include <string>

#include "widget/tree_node.h"
#include "widget/styled_widget.h"

TreeNode::TreeNode(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "TreeNode";
    m_handlesChildrenWithinRenderMethod = true;
    m_label = label;
}

void TreeNode::Render(ReactImgui* view) {
    YGNodeStyleSetMargin(m_layoutNode->m_node, YGEdgeBottom, m_view->GetWidgetFontSize(this));

    ImGui::PushID(m_id);
    if (ImGui::TreeNode(m_label.c_str())) {
        SetChildrenDisplay(view, YGDisplayFlex);

        const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
        const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);
        const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
        const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

        ImGui::SetCursorPos(ImVec2(left, top + m_view->GetWidgetFontSize(this))); // account for header

        ImGui::BeginChild("##", ImVec2(width, height), ImGuiChildFlags_None);

        ImGui::SetCursorPos(ImVec2(0, 0));

        Widget::HandleChildren(view);

        ImGui::EndChild();
        ImGui::TreePop();
    } else {
        SetChildrenDisplay(view, YGDisplayNone);
    }

    ImGui::PopID();
};

void TreeNode::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

bool TreeNode::HasCustomWidth() {
    return false;
}

bool TreeNode::HasCustomHeight() {
    return false;
}
