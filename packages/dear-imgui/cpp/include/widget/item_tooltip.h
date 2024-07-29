#include "styled_widget.h"

class ItemTooltip final : public StyledWidget {
public:
    static std::unique_ptr<ItemTooltip> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();

            return std::make_unique<ItemTooltip>(view, id, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    ItemTooltip(ReactImgui* view, int id, std::optional<BaseStyle>& style);

    void Render(ReactImgui* view) override;
};
