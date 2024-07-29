#include "widget/item_tooltip.h"

#include <imgui.h>

#include "widget/widget.h"

ItemTooltip::ItemTooltip(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "ItemTooltip";
    m_handlesChildrenWithinRenderMethod = true;
}

void ItemTooltip::Render(ReactImgui* view) {
    if (ImGui::BeginItemTooltip()) {
        Widget::HandleChildren(view);

        ImGui::EndTooltip();
    }
};