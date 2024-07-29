#include "styled_widget.h"

class TabBar final : public StyledWidget {
public:
    static std::unique_ptr<TabBar> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();

            return std::make_unique<TabBar>(view, id, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    TabBar(ReactImgui* view, int id, std::optional<BaseStyle>& style);

    void Render(ReactImgui* view) override;

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;

    void Init() override {
        YGNodeStyleSetPadding(m_layoutNode->m_node, YGEdgeTop, 25.0f);
    }
};

class TabItem final : public StyledWidget {
public:
    std::string m_label;

    static std::unique_ptr<TabItem> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();
            auto label = widgetDef["label"].template get<std::string>();

            return std::make_unique<TabItem>(view, id, label, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    TabItem(ReactImgui* view, int id, const std::string& label, std::optional<BaseStyle>& style);

    void Render(ReactImgui* view) override;

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;
};