#include <optional>
#include <widget/widget.h>

#pragma once

class ReactImgui;

struct BaseStyle {
    std::optional<StyleColors> maybeColors;
    std::optional<StyleVars> maybeStyleVars;
    std::optional<int> maybeFontIndex;
    std::optional<HorizontalAlignment> maybeHorizontalAlignment;
};

class StyledWidget : public Widget {
public:
    std::optional<std::unique_ptr<BaseStyle>> m_style;

    static std::optional<BaseStyle> ExtractStyle(const json& widgetDef, ReactImgui* view);

    explicit StyledWidget(ReactImgui* view, int id);

    StyledWidget(ReactImgui* view, int id, std::optional<BaseStyle>& maybeStyle);

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

    void ReplaceStyle(BaseStyle& newStyle);

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;

    [[nodiscard]] bool HasCustomStyleVar(ImGuiStyleVar key) const;

    [[nodiscard]] StyleVarValue& GetCustomStyleVar(ImGuiStyleVar key) const;
};