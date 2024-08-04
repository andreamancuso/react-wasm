#include "styled_widget.h"

class Group final : public StyledWidget {
public:
    static std::unique_ptr<Group> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();

            return std::make_unique<Group>(view, id, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    Group(ReactImgui* view, int id, std::optional<WidgetStyle>& style);

    void Render(ReactImgui* view) override;
};
