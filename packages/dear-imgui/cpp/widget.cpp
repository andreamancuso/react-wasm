#include <cstring>
#include <string>
#include <emscripten/bind.h>
#include "imgui.h"
#include <nlohmann/json.hpp>

#include "shared.h"
#include "element.h"
#include "widget.h"
#include "reactimgui.h"

using json = nlohmann::json;

Widget::Widget(ReactImgui* view, const int id) : Element(view, id, false) {
    m_type = "Unknown";
    m_handlesChildrenWithinRenderMethod = false;
}

const char* Widget::GetElementType() {
    return "widget";
};

// todo: seems redundant
void Widget::HandleChildren(ReactImgui* view) {
    view->RenderChildren(m_id);
};

void Widget::SetChildrenDisplay(ReactImgui* view, YGDisplay display) const {
    view->SetChildrenDisplay(m_id, display);
};

void Widget::Patch(const json& elementPatchDef, ReactImgui* view) {
    Element::Patch(elementPatchDef, view);
};

void Widget::PreRender(ReactImgui* view) {};

void Widget::Render(ReactImgui* view) {};

void Widget::PostRender(ReactImgui* view) {};

std::optional<BaseStyle> StyledWidget::ExtractStyle(const json& widgetDef, ReactImgui* view) {
    std::optional<BaseStyle> maybeStyle;
        
    if (widgetDef.is_object() && widgetDef.contains("style") && widgetDef["style"].is_object()) {
        // Perhaps a bit optimistic, but also rather convenient
        maybeStyle.emplace(BaseStyle{});

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

            if (!colors.empty()) {
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

            if (!styleVars.empty()) {
                maybeStyle.value().maybeStyleVars.emplace(styleVars);
            }
        }
    }

    return maybeStyle;
};

StyledWidget::StyledWidget(ReactImgui* view, const int id) : Widget(view, id) {}

StyledWidget::StyledWidget(ReactImgui* view, const int id, std::optional<BaseStyle>& maybeStyle) : Widget(view, id) {
    if (maybeStyle.has_value()) {
        m_style.emplace(std::make_unique<BaseStyle>(maybeStyle.value()));
    }
}

void StyledWidget::ReplaceStyle(BaseStyle& newStyle) {
    m_style.reset();
    m_style.emplace(std::make_unique<BaseStyle>(newStyle));
};

void StyledWidget::Patch(const json& widgetPatchDef, ReactImgui* view) {
    Widget::Patch(widgetPatchDef, view);
    auto maybeNewStyle = ExtractStyle(widgetPatchDef, view);

    if (maybeNewStyle.has_value()) {
        ReplaceStyle(maybeNewStyle.value());
    }

    if (YGNodeGetParent(m_layoutNode->m_node) && YGNodeHasMeasureFunc(m_layoutNode->m_node)) {
        YGNodeMarkDirty(m_layoutNode->m_node);
    }
};

bool StyledWidget::HasCustomStyles() const {
    return m_style.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomFont(ReactImgui* view) const {
    return m_style.value()->maybeFontIndex.has_value() && view->IsFontIndexValid(m_style.value()->maybeFontIndex.value());
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomWidth() {
    return !YGFloatIsUndefined(YGNodeLayoutGetWidth(m_layoutNode->m_node));
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomHeight() {
    return false;
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomHorizontalAlignment() const {
    return m_style.value()->maybeHorizontalAlignment.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasRightHorizontalAlignment() const {
    return HasCustomHorizontalAlignment() && m_style.value()->maybeHorizontalAlignment.value() == HorizontalAlignment_Right;
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomColors() const {
    return m_style.value()->maybeColors.has_value();
};

// Assumes m_style is not null, you should call HasCustomStyles() first
bool StyledWidget::HasCustomStyleVars() const {
    return m_style.value()->maybeStyleVars.has_value();
};

void StyledWidget::PreRender(ReactImgui* view) {
    const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
    const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);

    if (!YGFloatIsUndefined(left)) {
        // printf("%d %s %f %f\n", m_id, m_type.c_str(), left, top);
        ImGui::SetCursorPos(ImVec2(left, top));
    }

    if (HasCustomWidth()) {
        const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
        ImGui::PushItemWidth(width);
    }

    if (HasCustomStyles()) {
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
    if (HasCustomWidth()) {
        ImGui::PopItemWidth();
    }

    if (HasCustomStyles()) {
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

bool StyledWidget::HasCustomStyleVar(const ImGuiStyleVar key) const {
    return m_style.has_value() && m_style.value()->maybeStyleVars.has_value() && m_style.value()->maybeStyleVars.value().contains(key);
};

// IMPORTANT: call HasCustomStyleVar() first
StyleVarValue& StyledWidget::GetCustomStyleVar(const ImGuiStyleVar key) const {
    return m_style.value()->maybeStyleVars.value().at(key);
};

Window::Window(ReactImgui* view, const int id, const std::string& title, const float width, const float height, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "Window";
    m_handlesChildrenWithinRenderMethod = true;

    m_title = title;
    m_width = width;
    m_height = height;
    m_open = true;
}

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

void Window::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef["title"].is_string()) {
            m_title = widgetPatchDef["title"].template get<std::string>();
        }
        if (widgetPatchDef["width"].is_string()) {
            m_width = widgetPatchDef["width"].template get<float>();
        }
        if (widgetPatchDef["height"].is_string()) {
            m_height = widgetPatchDef["height"].template get<float>();
        }
    }
};

Group::Group(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "Group";
    m_handlesChildrenWithinRenderMethod = true;
}

void Group::Render(ReactImgui* view) {
    ImGui::BeginGroup();
    Widget::HandleChildren(view);
    ImGui::EndGroup();
};

Child::Child(ReactImgui* view, const int id, const float width, const float height, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "Child";
    m_handlesChildrenWithinRenderMethod = true;

    m_width = width;
    m_height = height;
};

void Child::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    ImGui::BeginChild("", ImVec2(m_width, m_height), m_flags);
    Widget::HandleChildren(view);
    ImGui::EndChild();
    ImGui::PopID();
};

void Child::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef["width"].is_string()) {
            m_width = widgetPatchDef["width"].template get<float>();
        }
        if (widgetPatchDef["height"].is_string()) {
            m_height = widgetPatchDef["height"].template get<float>();
        }
    }
};

void Separator::Render(ReactImgui* view) {
    ImGui::Separator();
};

void SeparatorText::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

void SeparatorText::Render(ReactImgui* view) {
    ImGui::SeparatorText(m_label.c_str());
};

void Text::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("text") && widgetPatchDef["text"].is_string()) {
            m_text = widgetPatchDef["text"].template get<std::string>();
        }
    }
};

std::unique_ptr<BulletText> BulletText::makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        auto text = widgetDef["text"].template get<std::string>();

        return std::make_unique<BulletText>(view, id, text, maybeStyle);
    }

    throw std::invalid_argument("Invalid JSON data");
};

void BulletText::Render(ReactImgui* view) {
    ImGui::BulletText("%s", m_text.c_str());
};

std::unique_ptr<UnformattedText> UnformattedText::makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        auto text = widgetDef["text"].template get<std::string>();

        return std::make_unique<UnformattedText>(view, id, text, maybeStyle);
    }

    throw std::invalid_argument("Invalid JSON data");
};

void UnformattedText::Render(ReactImgui* view) {
    ImGui::TextUnformatted(m_text.c_str());
};

std::unique_ptr<DisabledText> DisabledText::makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        auto text = widgetDef["text"].template get<std::string>();

        return std::make_unique<DisabledText>(view, id, text, maybeStyle);
    }

    throw std::invalid_argument("Invalid JSON data");
};

void DisabledText::Render(ReactImgui* view) {
    ImGui::TextDisabled("%s", m_text.c_str());
};

TabBar::TabBar(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "TabBar";
    m_handlesChildrenWithinRenderMethod = true;
}

bool TabBar::HasCustomWidth() {
    return false;
}

bool TabBar::HasCustomHeight() {
    return false;
}

void TabBar::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    // todo: double-check if we need to pass a proper id here?

    const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
    const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);
    const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
    const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

    ImGui::SetCursorPos(ImVec2(left, top));

    // TODO: +4.0f seems somewhat arbitrary
    ImGui::BeginChild("##", ImVec2(width, height), ImGuiChildFlags_None);

    if (ImGui::BeginTabBar("", ImGuiTabBarFlags_None)) {
        Widget::HandleChildren(view);
        ImGui::EndTabBar();
    }

    ImGui::EndChild();
    ImGui::PopID();
};

TabItem::TabItem(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "TabItem";
    m_handlesChildrenWithinRenderMethod = true;
    m_label = label;
}

bool TabItem::HasCustomWidth() {
    return false;
}

bool TabItem::HasCustomHeight() {
    return false;
}

void TabItem::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    if (ImGui::BeginTabItem(m_label.c_str())) {
        m_layoutNode->SetDisplay(YGDisplayFlex);

        const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
        const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

        ImGui::SetCursorPos(ImVec2(0, 25.f)); // padding top to account for tabs

        ImGui::BeginChild("##", ImVec2(width, height), ImGuiChildFlags_None);

        Widget::HandleChildren(view);

        ImGui::EndChild();
        ImGui::EndTabItem();
    } else {
        m_layoutNode->SetDisplay(YGDisplayNone);
    }
    ImGui::PopID();
};

void TabItem::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

CollapsingHeader::CollapsingHeader(ReactImgui* view, const int id, const std::string& label, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
    m_type = "CollapsingHeader";
    m_handlesChildrenWithinRenderMethod = true;
    m_label = label;
}

void CollapsingHeader::Render(ReactImgui* view) {
    YGNodeStyleSetMargin(m_layoutNode->m_node, YGEdgeBottom, m_view->GetFrameHeightWithSpacing(this));

    ImGui::PushID(m_id);
    if (ImGui::CollapsingHeader(m_label.c_str())) {
        SetChildrenDisplay(view, YGDisplayFlex);

        const float left = YGNodeLayoutGetLeft(m_layoutNode->m_node);
        const float top = YGNodeLayoutGetTop(m_layoutNode->m_node);
        const float width = YGNodeLayoutGetWidth(m_layoutNode->m_node);
        const float height = YGNodeLayoutGetHeight(m_layoutNode->m_node);

        ImGui::SetCursorPos(ImVec2(left, top + m_view->GetFrameHeightWithSpacing(this))); // account for header

        ImGui::BeginChild("##", ImVec2(width, height), ImGuiChildFlags_None);

        ImGui::SetCursorPos(ImVec2(0, 0));

        Widget::HandleChildren(view);

        ImGui::EndChild();
    } else {
        SetChildrenDisplay(view, YGDisplayNone);
    }
    ImGui::PopID();
};

void CollapsingHeader::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

bool CollapsingHeader::HasCustomWidth() {
    return false;
}

bool CollapsingHeader::HasCustomHeight() {
    return false;
}

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

void Combo::Render(ReactImgui* view) {
    ImGui::PushID(m_id);

    if (ImGui::BeginCombo("",
        m_selectedIndex >= 0 && m_selectedIndex < m_options.size()
            ? m_options[m_selectedIndex].c_str()
            : m_placeholder.c_str()
        , ImGuiComboFlags_None)) {
        const auto optionsLength = m_options.size();
        for (int n = 0; n < optionsLength; n++)
        {
            const bool is_selected = (m_selectedIndex == n);
            if (ImGui::Selectable(m_options[n].c_str(), is_selected)) {
                m_selectedIndex = n;
                view->m_onComboChange(m_id, m_selectedIndex);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::PopID();
};

void Combo::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("placeholder") && widgetPatchDef["placeholder"].is_string()) {
            m_placeholder = widgetPatchDef["placeholder"].template get<std::string>();
        }

        if (widgetPatchDef.contains("options") && widgetPatchDef["options"].is_array()) {
            SetOptions(widgetPatchDef["options"]);
        }
    }
};

void InputText::Render(ReactImgui* view) {
    ImGui::PushID(m_id);
    ImGui::InputText(m_label.c_str(), m_bufferPointer.get(), 100, inputTextFlags, InputTextCb, (void*)this);
    ImGui::PopID();
};

void InputText::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

int InputText::InputTextCb(ImGuiInputTextCallbackData* data) {
    if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit) {
        auto pInputText = reinterpret_cast<InputText*>(data->UserData);

        std::string value = data->Buf;
        onInputTextChange_(pInputText->m_id, value);
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

void Checkbox::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
};

std::unique_ptr<Button> Button::makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
    if (widgetDef.is_object()) {
        auto id = widgetDef["id"].template get<int>();
        auto label = widgetDef.contains("label") && widgetDef["label"].is_string() ? widgetDef["label"].template get<std::string>() : "";

        return makeWidget(view, id, label, maybeStyle);
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

    ImVec2 size = ImVec2(YGNodeLayoutGetWidth(m_layoutNode->m_node), YGNodeLayoutGetHeight(m_layoutNode->m_node));

    if (ImGui::Button(m_label.c_str(), size)) {
        view->m_onClick(m_id);
    }
    ImGui::PopID();
};

void Button::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
    }
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

void Slider::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
        if (widgetPatchDef.contains("min") && widgetPatchDef["min"].is_number()) {
            m_min = widgetPatchDef["min"].template get<float>();
        }
        if (widgetPatchDef.contains("max") && widgetPatchDef["max"].is_number()) {
            m_max = widgetPatchDef["max"].template get<float>();
        }
    }
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

void MultiSlider::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);

        if (widgetPatchDef.contains("label") && widgetPatchDef["label"].is_string()) {
            m_label = widgetPatchDef["label"].template get<std::string>();
        }
        if (widgetPatchDef.contains("min") && widgetPatchDef["min"].is_number()) {
            m_min = widgetPatchDef["min"].template get<float>();
        }
        if (widgetPatchDef.contains("max") && widgetPatchDef["max"].is_number()) {
            m_max = widgetPatchDef["max"].template get<float>();
        }
        if (widgetPatchDef.contains("decimalDigits") && widgetPatchDef["decimalDigits"].is_number()) {
            m_decimalDigits = widgetPatchDef["decimalDigits"].template get<int>();
        }
    }
};

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

void ClippedMultiLineTextRenderer::Patch(const json& widgetPatchDef, ReactImgui* view) {
    if (widgetPatchDef.is_object()) {
        StyledWidget::Patch(widgetPatchDef, view);
        // not sure what can be patched
    }
};

bool Map::HasCustomWidth() {
    return false;
}

bool Map::HasCustomHeight() {
    return false;
}

void Map::Render(ReactImgui* view) {
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

