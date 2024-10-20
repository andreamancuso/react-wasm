#include "styled_widget.h"

class Child final : public StyledWidget {
public:
    ImGuiChildFlags m_flags = ImGuiChildFlags_None;
    ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_None;
    float m_width;
    float m_height;

    static std::unique_ptr<Child> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        auto id = widgetDef["id"].template get<int>();
        float width = 0;
        float height = 0;

        if (widgetDef.contains("width") && widgetDef["width"].is_number()) {
            width = widgetDef["width"].template get<float>();
        }

        if (widgetDef.contains("height") && widgetDef["height"].is_number()) {
            height = widgetDef["height"].template get<float>();
        }

        return std::make_unique<Child>(view, id, width, height, maybeStyle);

        // throw std::invalid_argument("Invalid JSON data");
    }

    Child(ReactImgui* view, int id, float width, float height, std::optional<WidgetStyle>& style);

    void Render(ReactImgui* view, const std::optional<ImRect>& viewport) override;

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;
};
