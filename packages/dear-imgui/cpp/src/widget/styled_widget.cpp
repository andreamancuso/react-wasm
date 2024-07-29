#include <optional>

#include "widget/styled_widget.h"

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