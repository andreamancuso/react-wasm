#include "styled_widget.h"

class ItemTooltip final : public StyledWidget {
public:
    static std::unique_ptr<ItemTooltip> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        auto id = widgetDef["id"].template get<int>();

        return std::make_unique<ItemTooltip>(view, id, maybeStyle);

        // throw std::invalid_argument("Invalid JSON data");
    }

    ItemTooltip(ReactImgui* view, int id, std::optional<WidgetStyle>& style);

    void Render(ReactImgui* view, const std::optional<ImRect>& viewport) override;
};
