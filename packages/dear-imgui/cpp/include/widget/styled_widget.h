#include <optional>
#include <widget/widget.h>

#pragma once

class ReactImgui;

struct WidgetStyle {
    std::optional<StyleColors> maybeColors;
    std::optional<StyleVars> maybeStyleVars;
    std::optional<int> maybeFontIndex;
    std::optional<HorizontalAlignment> maybeHorizontalAlignment;
};

class StyledWidget : public Widget {
public:
    std::optional<std::unique_ptr<WidgetStyle>> m_style;

    static std::optional<WidgetStyle> ExtractStyle(const json& widgetDef, ReactImgui* view);

    explicit StyledWidget(ReactImgui* view, int id);

    StyledWidget(ReactImgui* view, int id, std::optional<WidgetStyle>& maybeStyle);

    void PreRender(ReactImgui* view) override;

    void PostRender(ReactImgui* view) override;

    [[nodiscard]] bool HasCustomStyles() const;

    bool HasCustomFont(ReactImgui* view) const;

    [[nodiscard]] bool HasCustomColors() const;

    virtual bool HasCustomWidth();

    virtual bool HasCustomHeight();

    [[nodiscard]] bool HasCustomStyleVars() const;

    [[nodiscard]] bool HasCustomHorizontalAlignment() const;

    [[nodiscard]] bool HasRightHorizontalAlignment() const;

    void ReplaceStyle(WidgetStyle& newStyle);

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;

    [[nodiscard]] bool HasCustomStyleVar(ImGuiStyleVar key) const;

    [[nodiscard]] StyleVarValue& GetCustomStyleVar(ImGuiStyleVar key) const;
};