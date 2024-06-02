// Dear ImGui: standalone example application for Emscripten, using GLFW + WebGPU
// (Emscripten is a C++-to-javascript compiler, used to publish executables for the web. See https://emscripten.org/)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <cstring>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_wgpu.h"
#include "implot.h"
#include "implot_internal.h"
#include <nlohmann/json.hpp>

#include "shared.h"
#include "widget.h"
#include "reactimgui.h"

using json = nlohmann::json;

// todo: seems redundant
void Widget::HandleChildren(ReactImgui* view) {
    view->RenderChildren(m_id);
};

ImGuiCol Widget::GetImGuiCol() {
    return ImGuiCol_COUNT;
};

void Fragment::Render(ReactImgui* view) {
    // ImGui::Text("Begin Fragment (ID: %d)", m_id);
    Widget::HandleChildren(view);
    // ImGui::Text("End Fragment (ID: %d)", m_id);
};

void Window::Render(ReactImgui* view) {
    // ImGui::PushID(m_id);
    ImGui::SetNextWindowSize(ImVec2(m_width, m_height), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin(m_title.c_str(), &m_open, m_flags)) {
        ImGui::End();
        return;
    }
    Widget::HandleChildren(view);
    ImGui::End();
    // ImGui::PopID();
};

void Group::Render(ReactImgui* view) {
    ImGui::BeginGroup();
    Widget::HandleChildren(view);
    ImGui::EndGroup();
};

void Child::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    ImGui::BeginChild("", ImVec2(m_width, m_height), m_flags);
    Widget::HandleChildren(view);
    ImGui::EndChild();
    ImGui::PopID();
};

void SameLine::Render(ReactImgui* view) {
    // Special case
    if (view->m_hierarchy.contains(m_id)) {
        size_t size = view->m_hierarchy[m_id].size() - 1;

        int i = 0;
        for (auto& childId : view->m_hierarchy[m_id]) {
            view->RenderWidgets(childId);

            if (i < (size)) {
                ImGui::SameLine();
            }

            i++;
        }

        // ImGui::Text("(ID: %d)", m_id);
    }
};

void Separator::Render(ReactImgui* view) {
    ImGui::Separator();
};

void Indent::Render(ReactImgui* view) {
    ImGui::Indent();
    Widget::HandleChildren(view);
    ImGui::Unindent();
};

void Unindent::Render(ReactImgui* view) {
    ImGui::Unindent();
};

void SeparatorText::Render(ReactImgui* view) {
    ImGui::SeparatorText(m_label.c_str());
};

void BulletText::Render(ReactImgui* view) {
    ImGui::BulletText(m_text.c_str());
};

void UnformattedText::Render(ReactImgui* view) {
    ImGui::TextUnformatted(m_text.c_str());
};

ImGuiCol UnformattedText::GetImGuiCol() {
    return ImGuiCol_Text;
};

std::unique_ptr<UnformattedText> UnformattedText::makeWidget(const json& widgetDef, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        std::string text = widgetDef["text"].template get<std::string>();

        std::optional<int> maybeFontIndex;
        std::optional<ImVec4> maybeColor;
        
        if (widgetDef.contains("font") 
            && widgetDef["font"].is_object() 
            && widgetDef["font"]["name"].is_string() 
            && widgetDef["font"]["size"].is_number_unsigned()) {

            maybeFontIndex.emplace(view->GetFontIndex(
                widgetDef["font"]["name"].template get<std::string>(), 
                widgetDef["font"]["size"].template get<int>()
            ));
        }
        
        if (widgetDef.contains("color") 
            && widgetDef["color"].is_string()) {

            auto color = widgetDef["color"].template get<std::string>();

            printf("color string length: %d", (int)color.size());

            if (color.size() == 6) {
                maybeColor.emplace(HEXAtoIV4(color.c_str()));
            }
        }

        return std::make_unique<UnformattedText>(id, text, maybeFontIndex, maybeColor);
    }

    throw std::invalid_argument("Invalid JSON data");
};

void DisabledText::Render(ReactImgui* view) {
    ImGui::TextDisabled(m_text.c_str());
};

void TabBar::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    // todo: double-check if we need to pass a proper id here?
    if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None)) {
        Widget::HandleChildren(view);
        ImGui::EndTabBar();
    }
    ImGui::PopID();
};

void TabItem::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::BeginTabItem(m_label.c_str())) {
        Widget::HandleChildren(view);
        ImGui::EndTabItem();
    }
    ImGui::PopID();
};

void CollapsingHeader::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::CollapsingHeader(m_label.c_str())) {
        Widget::HandleChildren(view);
    }
    ImGui::PopID();
};

void TextWrap::Render(ReactImgui* view) {
    ImGui::PushTextWrapPos(m_width);
    
    Widget::HandleChildren(view);

    ImGui::PopTextWrapPos();
};

void ItemTooltip::Render(ReactImgui* view) {
    if (ImGui::BeginItemTooltip()) {
        Widget::HandleChildren(view);

        ImGui::EndTooltip();
    }
};

void TreeNode::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::TreeNode(m_label.c_str())) {
        Widget::HandleChildren(view);

        ImGui::TreePop();
        ImGui::Spacing();
    }
    ImGui::PopID();
};

void Combo::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::Combo(m_label.c_str(), &m_selectedIndex, m_itemsSeparatedByZeros.get())) {
        view->m_onComboChange(m_id, m_selectedIndex);
    }
    ImGui::PopID();
};

void InputText::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    ImGui::InputText(m_label.c_str(), m_bufferPointer.get(), 100, inputTextFlags, InputTextCb, (void*)this);
    ImGui::PopID();
};

int InputText::InputTextCb(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        auto pInputText = reinterpret_cast<InputText*>(data->UserData);

        std::string value = data->Buf;
        Widget::onInputTextChange_(pInputText->m_id, value);
    }

    return 0;
};

void Checkbox::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::Checkbox(m_label.c_str(), &m_checked)) {
        view->m_onBooleanValueChange(m_id, m_checked);
    }
    ImGui::PopID();
};

void Button::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::Button(m_label.c_str())) {
        view->m_onClick(m_id);
    }
    ImGui::PopID();
};

void Slider::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (m_type == "angle") {
        if (ImGui::SliderAngle(m_label.c_str(), &m_value, m_min, m_max, "%.0f")) {
            view->m_onNumericValueChange(m_id, m_value);
        }
    } else {
        if (ImGui::SliderFloat(m_label.c_str(), &m_value, m_min, m_max, "%.0f")) {
            view->m_onNumericValueChange(m_id, m_value);
        }
    }
    ImGui::PopID();
};

void MultiSlider::Render(ReactImgui* view) {
    ImGui::PushID(m_id);

    if (m_numValues == 2) {
        if (ImGui::SliderFloat2(m_label.c_str(), m_values.get(), m_min, m_max, view->m_floatFormatChars[m_decimalDigits].get())) {
            view->m_onMultiValueChange(m_id, m_values.get(), m_numValues);
        }
    } else if (m_numValues == 3) {
        if (ImGui::SliderFloat3(m_label.c_str(), m_values.get(), m_min, m_max, view->m_floatFormatChars[m_decimalDigits].get())) {
            view->m_onMultiValueChange(m_id, m_values.get(), m_numValues);
        }
    } else if (m_numValues == 4) {
        if (ImGui::SliderFloat4(m_label.c_str(), m_values.get(), m_min, m_max, view->m_floatFormatChars[m_decimalDigits].get())) {
            view->m_onMultiValueChange(m_id, m_values.get(), m_numValues);
        }
    }

    ImGui::PopID();
};

void Table::Render(ReactImgui* view) {
    ImGui::Text("Table data length: %d", (int) m_data.size());

    ImGui::PushID(m_id);

    if (m_clipRows > 0) {
        // static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * (m_clipRows + 1)); // account for frozen table headings
        if (ImGui::BeginTable("t", (int)m_columns.size(), m_flags | ImGuiTableFlags_ScrollY, outer_size)) {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            for (const auto& columnSpec : m_columns) {
                ImGui::TableSetupColumn(columnSpec.heading.c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
            }
            ImGui::TableHeadersRow();

            auto numColumns = m_columns.size();

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
    } else {
        if (ImGui::BeginTable("t", (int)m_columns.size(), m_flags)) {
            for (const auto& columnSpec : m_columns) {
                ImGui::TableSetupColumn(columnSpec.heading.c_str(), ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_WidthStretch);
            }

            ImGui::TableHeadersRow();

            auto numColumns = m_columns.size();

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
    }

    ImGui::PopID();
};

void ClippedMultiLineTextRenderer::Render(ReactImgui* view) {
    ImGui::PushID(m_id);

    const char* buf = m_textBuffer.begin();
    const char* buf_end = m_textBuffer.end();
    ImGuiListClipper clipper;
    clipper.Begin(m_lineOffsets.Size);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    while (clipper.Step()) {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
            const char* line_start = buf + m_lineOffsets[line_no];
            const char* line_end = (line_no + 1 < m_lineOffsets.Size) ? (buf + m_lineOffsets[line_no + 1] - 1) : buf_end;
            ImGui::TextUnformatted(line_start, line_end);
        }
    }

    ImGui::PopStyleVar();

    // ImGui::Text("%d lines", m_lineOffsets.Size);

    ImGui::PopID();
};
