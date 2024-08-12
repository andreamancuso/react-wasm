#include "mapgenerator.h"
#include "styled_widget.h"

class MapView final : public StyledWidget {
private:
    ImVec2 m_offset;

    int m_mapGeneratorJobCounter = 0;
    std::unordered_map<int, std::unique_ptr<MapGenerator>> m_mapGeneratorJobs;

    std::unordered_map<int, std::unique_ptr<Texture>> m_textures;

public:
    static std::unique_ptr<MapView> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        auto id = widgetDef["id"].template get<int>();

        return std::make_unique<MapView>(view, id, maybeStyle);

        // throw std::invalid_argument("Invalid JSON data");
    }

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;

    MapView(ReactImgui* view, const int id, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style) {
        m_type = "map-view";

        m_offset = ImVec2(0.0f, 0.0f);
    }

    void Render(ReactImgui* view) override;

    bool HasInternalOps() override;

    void HandleInternalOp(const json& opDef) override;
};
