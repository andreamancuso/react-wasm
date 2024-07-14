// Dear ImGui: standalone example application for Emscripten, using GLFW + WebGPU
// (Emscripten is a C++-to-javascript compiler, used to publish executables for the web. See https://emscripten.org/)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <cstring>
#include <tuple>
#include <string>
#include <sstream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "IconsFontAwesome6.h"
// #include "IconsMaterialDesignIcons.h"
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

void Widget::Patch(const json& widgetPatchDef, ReactImgui* view) {};

void Widget::PreRender(ReactImgui* view) {};

void Widget::PostRender(ReactImgui* view) {};

std::optional<BaseStyle> StyledWidget::ExtractStyle(const json& widgetDef, ReactImgui* view) {
    std::optional<BaseStyle> maybeStyle;
        
    if (widgetDef.is_object() && widgetDef.contains("style") && widgetDef["style"].is_object()) {
        // Perhaps a bit optimistic, but also rather convenient
        maybeStyle.emplace(BaseStyle{});

        // if (widgetDef["style"].contains("align") 
        //     && widgetDef["style"]["align"].is_string()) {

        //     auto align = widgetDef["style"]["align"].template get<std::string>();

        //     if (align == "left") {
        //         maybeStyle.value().maybeHorizontalAlignment.emplace(HorizontalAlignment_Left);
        //     }
        //     if (align == "right") {
        //         maybeStyle.value().maybeHorizontalAlignment.emplace(HorizontalAlignment_Right);
        //     }
        // }

        if (widgetDef["style"].contains("width") 
            && widgetDef["style"]["width"].is_number()) {

            maybeStyle.value().maybeWidth.emplace(widgetDef["style"]["width"].template get<float>());
        }

        if (widgetDef["style"].contains("height") 
            && widgetDef["style"]["height"].is_number_unsigned()) {

            maybeStyle.value().maybeHeight.emplace(widgetDef["style"]["height"].template get<float>());
        }

        if (widgetDef["style"].contains("font") 
            && widgetDef["style"]["font"].is_object() 
            && widgetDef["style"]["font"]["name"].is_string() 
            && widgetDef["style"]["font"]["size"].is_number_unsigned()) {

            maybeStyle.value().maybeFontIndex.emplace(view->GetFontIndex(
                widgetDef["style"]["font"]["name"].template get<std::string>(), 
                widgetDef["style"]["font"]["size"].template get<int>()
            ));
        }

        if (widgetDef["style"].contains("colors") 
            && widgetDef["style"]["colors"].is_object()) {

            StyleColors colors;

            for (auto& [key, item] : widgetDef["style"]["colors"].items()) {
                if (item.is_string()) {
                    auto color = item.template get<std::string>();

                    if (color.size() == 6) {
                        colors[stoi(key)] = HEXAtoIV4(color.c_str());
                    }
                } else if (item.is_array() && item.size() == 2
                    && item[0].is_string() && item[1].is_number_unsigned()) { // hex + alpha
                    auto color = item[0].template get<std::string>();
                    auto alpha = item[1].template get<float>();

                    if (color.size() == 6) {
                        colors[stoi(key)] = HEXAtoIV4(color.c_str(), alpha);
                    }
                }
            }

            if (colors.size() > 0) {
                maybeStyle.value().maybeColors.emplace(colors);
            }
        }

        if (widgetDef["style"].contains("vars") 
            && widgetDef["style"]["vars"].is_object()) {
            StyleVars styleVars;

            for (auto& [key, item] : widgetDef["style"]["vars"].items()) {
                StyleVarValue value;

                if (item.is_array() && item.size() == 2
                     && item[0].is_number_unsigned() && item[1].is_number_unsigned()) { // ImVec2
                    value = ImVec2(
                        item[0].template get<float>(), 
                        item[1].template get<float>()
                    );
                } else if (item.is_number_unsigned()) { // float
                    value = item.template get<float>();
                }

                if (value.index() != 0) {
                    styleVars[stoi(key)] = value;
                }
            }

            if (styleVars.size() > 0) {
                maybeStyle.value().maybeStyleVars.emplace(styleVars);
            }
        }
    }

    return maybeStyle;
};

void StyledWidget::ReplaceStyle(BaseStyle& newStyle) {
    m_style.reset();
    m_style.emplace(std::make_unique<BaseStyle>(newStyle));
};

void StyledWidget::Patch(const json& widgetPatchDef, ReactImgui* view) {
    auto maybeNewStyle = StyledWidget::ExtractStyle(widgetPatchDef, view);

    if (maybeNewStyle.has_value()) {
        ReplaceStyle(maybeNewStyle.value());
    }
};

bool StyledWidget::HasCustomStyles() {
    return m_style.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomFont(ReactImgui* view) {
    return m_style.value()->maybeFontIndex.has_value() && view->IsFontIndexValid(m_style.value()->maybeFontIndex.value());
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomWidth() {
    return m_style.value()->maybeWidth.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomHeight() {
    return m_style.value()->maybeHeight.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomHorizontalAlignment() {
    return m_style.value()->maybeHorizontalAlignment.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasRightHorizontalAlignment() {
    return HasCustomHorizontalAlignment() && m_style.value()->maybeHorizontalAlignment.value() == HorizontalAlignment_Right;
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomColors() {
    return m_style.value()->maybeColors.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomStyleVars() {
    return m_style.value()->maybeStyleVars.has_value();
};

float StyledWidget::GetComputedWidth(ReactImgui* view) {
    if (m_style.has_value()) {
        auto style = m_style.value().get();

        if (style->maybeWidth.has_value()) {
            ImGuiStyle& imguiStyle = view->GetStyle();

            return (ImGui::GetWindowContentRegionMax().x * style->maybeWidth.value()) - imguiStyle.ItemSpacing.x;
        }
    }

    return 0;
};

float StyledWidget::GetComputedHeight(ReactImgui* view) {
    if (m_style.has_value() && m_style.value()->maybeHeight.has_value()) {
        ImGuiStyle& imguiStyle = view->GetStyle();

        return (ImGui::GetWindowContentRegionMax().y * m_style.value()->maybeHeight.value()) - imguiStyle.ItemSpacing.y;
    }

    return 0;
};

// This allegedly vertically aligns text ImGui::AlignTextToFramePadding();

void StyledWidget::PreRender(ReactImgui* view) {
    if (HasCustomStyles()) {
        if (HasCustomWidth()) {
            ImGui::PushItemWidth(GetComputedWidth(view));
        }
        // else if (HasRightHorizontalAlignment()) {
            // ImGui::PushItemWidth(-FLT_MIN);
        // }

        // if (HasCustomHeight()) {
            // todo: might make more sense to add widget-specific implementations
        // }

        if (HasCustomFont(view)) {
            view->PushFont(m_style.value()->maybeFontIndex.value());
        }

        if (HasCustomColors()) {
            for (auto const& [key, val] : m_style.value()->maybeColors.value()) {
                ImGui::PushStyleColor(key, val);
            }
        }

        if (HasCustomStyleVars()) {
            for (auto const& [key, val] : m_style.value()->maybeStyleVars.value()) {
                if (std::holds_alternative<float>(val)) {
                    ImGui::PushStyleVar(key, std::get<float>(val));
                } else if (std::holds_alternative<ImVec2>(val)) {
                    ImGui::PushStyleVar(key, std::get<ImVec2>(val));
                } else {
                    // todo: throw?
                }
            }
        }
    }
};

void StyledWidget::PostRender(ReactImgui* view) {
    if (HasCustomStyles()) {
        if (HasCustomWidth()) {
            ImGui::PopItemWidth();
        }
        // else if (HasRightHorizontalAlignment()) {
            // ImGui::PopItemWidth();
        // }

        // if (HasCustomHeight()) {
            // todo: might make more sense to add widget-specific implementations
        // }

        if (HasCustomFont(view)) {
            view->PopFont();
        }

        if (HasCustomColors()) {
            ImGui::PopStyleColor(m_style.value()->maybeColors.value().size());
        }

        if (HasCustomStyleVars()) {
            // Big, big assumption that this will match exactly the number of style vars being pushed above... Maybe we should actually keep track
            ImGui::PopStyleVar(m_style.value()->maybeStyleVars.value().size());
        }
    }
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

void SeparatorText::Render(ReactImgui* view) {
    ImGui::SeparatorText(m_label.c_str());
};

void BulletText::Render(ReactImgui* view) {
    ImGui::BulletText(m_text.c_str());
};

void UnformattedText::Render(ReactImgui* view) {
    ImGui::TextUnformatted(m_text.c_str());
};

std::unique_ptr<UnformattedText> UnformattedText::makeWidget(const json& widgetDef, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        std::string text = widgetDef["text"].template get<std::string>();

        // todo: extract and reuse
        auto style = StyledWidget::ExtractStyle(widgetDef, view);

        return std::make_unique<UnformattedText>(id, text, style);
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

int InputText::InputTextCb(ImGuiInputTextCallbackData* data) {
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

std::unique_ptr<Button> Button::makeWidget(const json& widgetDef, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

        auto style = StyledWidget::ExtractStyle(widgetDef, view);
        
        return Button::makeWidget(id, label, style);
    }

    throw std::invalid_argument("Invalid JSON data");
};

// size is passed as part of rendering the widget
bool Button::HasCustomWidth() {
    return false;
}

bool Button::HasCustomHeight() {
    return false;
}

void Button::Render(ReactImgui* view) {
    ImGui::PushID(m_id);

    ImGuiStyle& imguiStyle = view->GetStyle();

    // Apparently ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f)); is the equivalent of width: 100%
    if (ImGui::Button(m_label.c_str(), ImVec2(GetComputedWidth(view), GetComputedHeight(view)))) {
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

void Map::Render(ReactImgui* view) {
    if (view->m_textures.contains(0)) {
        ImGui::Text("offset: %f, %f", m_offset.x, m_offset.y);

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 imageSize = ImGui::GetContentRegionAvail();

        double boxWidthPct = imageSize.x / view->m_textures[0]->width;
        double boxHeightPct = imageSize.y / view->m_textures[0]->height;

        // ImGui::PushID(m_id);
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

        // ImGui::PopID();

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
    }
};