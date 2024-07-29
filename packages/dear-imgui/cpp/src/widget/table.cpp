
#include "widget/table.h"

#include "reactimgui.h"

void Table::Render(ReactImgui* view) {
    ImGui::PushID(m_id);

    ImGui::BeginGroup();

    // ImGui::Text("Table data length: %d", (int) m_data.size());

    ImVec2 outerSize = ImVec2(YGNodeLayoutGetWidth(m_layoutNode->m_node), YGNodeLayoutGetHeight(m_layoutNode->m_node));

    if (m_clipRows > 0) {
        // static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        // const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        // ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * (m_clipRows + 1)); // account for frozen table headings

        if (ImGui::BeginTable("t", (int)m_columns.size(), m_flags | ImGuiTableFlags_ScrollY, outerSize)) {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            for (const auto& columnSpec : m_columns) {
                ImGui::TableSetupColumn(columnSpec.heading.c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
            }
            ImGui::TableHeadersRow();

            const auto numColumns = m_columns.size();

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin((int)m_data.size());
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    ImGui::TableNextRow();
                    for (int i = 0; i < numColumns; i++) {
                        ImGui::TableSetColumnIndex(i);
                        if (m_columns[i].fieldId.has_value()) {
                            auto& fieldId = m_columns[i].fieldId.value();

                            if (m_data[row].contains(fieldId)) {
                                ImGui::TextUnformatted(m_data[row][fieldId].c_str());
                            }
                        }
                    }
                }
            }
            ImGui::EndTable();
        }
    } else if (ImGui::BeginTable("t", (int)m_columns.size(), m_flags, outerSize)) {
        for (const auto& columnSpec : m_columns) {
            ImGui::TableSetupColumn(columnSpec.heading.c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
        }

        ImGui::TableHeadersRow();

        const auto numColumns = m_columns.size();

        for (auto& dataRow : m_data) {
            ImGui::TableNextRow();
            for (int i = 0; i < numColumns; i++) {
                ImGui::TableSetColumnIndex(i);
                if (m_columns[i].fieldId.has_value()) {
                    auto& fieldId = m_columns[i].fieldId.value();

                    if (dataRow.contains(fieldId)) {
                        ImGui::TextUnformatted(dataRow[fieldId].c_str());
                    }
                }
            }
        }

        ImGui::EndTable();
        
    }

    ImGui::EndGroup();
    ImGui::PopID();
};

void Table::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);
        // not sure what can be patched - presumably the columns? though that'd likely force us to clear the data
    }
};

YGSize Table::Measure(const YGNodeConstRef node, const float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
    const auto widget = static_cast<Table*>(YGNodeGetContext(node));
    float fontSize = widget->m_view->GetWidgetFontSize(widget);
    YGSize size;

    size.width = width / 2;
    if (widget->m_clipRows > 0) {
        size.height = fontSize * static_cast<float>(widget->m_clipRows); // TODO: call ReactImgui::GetWidgetFontSize(widget)
    } else {
        size.height = fontSize * 5; // TODO: call ReactImgui::GetWidgetFontSize(widget)
    }

    return size;
};