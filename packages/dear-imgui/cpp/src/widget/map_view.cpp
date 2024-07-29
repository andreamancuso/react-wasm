#include <imgui.h>
#include <yoga/YGNodeLayout.h>

#include "widget/map_view.h"
#include "reactimgui.h"

bool MapView::HasCustomWidth() {
    return false;
}

bool MapView::HasCustomHeight() {
    return false;
}

void MapView::Render(ReactImgui* view) {
    if (view->m_textures.contains(0)) {

        auto imageSize = ImVec2(YGNodeLayoutGetWidth(m_layoutNode->m_node), YGNodeLayoutGetHeight(m_layoutNode->m_node));

        if (imageSize.x != 0 && imageSize.y != 0) {
            ImGui::PushID(m_id);
            // ImGui::Text("offset: %f, %f", m_offset.x, m_offset.y);

            ImGui::BeginGroup();

            ImDrawList* drawList = ImGui::GetWindowDrawList();

            double boxWidthPct = imageSize.x / view->m_textures[0]->width;
            double boxHeightPct = imageSize.y / view->m_textures[0]->height;

            ImGui::InvisibleButton("##map_canvas", imageSize);
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                m_offset.x += ImGui::GetIO().MouseDelta.x;
                m_offset.y += ImGui::GetIO().MouseDelta.y;

                if (m_offset.x < 0) {
                    m_offset.x = 0;
                }

                if (m_offset.y < 0) {
                    m_offset.y = 0;
                }

                if (m_offset.x > (view->m_textures[0]->width - imageSize.x)) {
                    m_offset.x = view->m_textures[0]->width - imageSize.x;
                }

                if (m_offset.y > (view->m_textures[0]->height - imageSize.y)) {
                    m_offset.y = view->m_textures[0]->height - imageSize.y;
                }
            }

            const ImVec2 p0 = ImGui::GetItemRectMin();
            const ImVec2 p1 = ImGui::GetItemRectMax();

            if (!ImGui::IsItemVisible()) {
                // Skip rendering as ImDrawList elements are not clipped.
                return;
            }


            ImGui::PushClipRect(p0, p1, true);

            ImVec2 uvMin = ImVec2(m_offset.x / view->m_textures[0]->width, m_offset.y / view->m_textures[0]->height);
            ImVec2 uvMax = ImVec2((m_offset.x / view->m_textures[0]->width) + boxWidthPct, (m_offset.y / view->m_textures[0]->height) + boxHeightPct);

            drawList->AddImage((void*)view->m_textures[0]->textureView, p0, p1, uvMin, uvMax);
            ImGui::PopClipRect();

            ImGui::EndGroup();
            ImGui::PopID();
        }
    }
};

