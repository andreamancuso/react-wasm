#include "styled_widget.h"

class MapView final : public StyledWidget {
private:
    ImVec2 m_offset;

public:
    static std::unique_ptr<MapView> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();

            return std::make_unique<MapView>(view, id, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;

    MapView(ReactImgui* view, const int id, std::optional<BaseStyle>& style) : StyledWidget(view, id, style) {
        m_type = "MapView";

        m_offset = ImVec2(0.0f, 0.0f);
    }

    void Render(ReactImgui* view) override;
};
