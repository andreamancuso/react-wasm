#include <optional>
#include "mapgenerator.h"
#include "styled_widget.h"

using fetchImageCallback = std::function<void(void*, size_t)>;

class Image final : public StyledWidget {
private:
    std::string m_url;
    std::optional<ImVec2> m_size;
    Texture m_texture;

public:
    static std::unique_ptr<Image> makeWidget(const json& widgetDef, std::optional<WidgetStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.contains("url")) {
            auto id = widgetDef["id"].template get<int>();
            auto url = widgetDef["url"].template get<std::string>();

            std::optional<ImVec2> size;

            if (widgetDef.contains("width") && widgetDef.contains("height")) {
                const auto w = widgetDef["width"].template get<float>();
                const auto h = widgetDef["height"].template get<float>();

                size.emplace(ImVec2(w,h));
            }

            return std::make_unique<Image>(view, id, url, size, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;

    Image(ReactImgui* view, const int id, const std::string& url, const std::optional<ImVec2>& size, std::optional<WidgetStyle>& style) : StyledWidget(view, id, style), m_texture() {
        m_type = "Image";
        m_url = url;

        m_size = size;
    }

    void Render(ReactImgui* view) override;

    static YGSize Measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);

    bool HasInternalOps() override;

    void HandleInternalOp(const json& opDef) override;

    void FetchImage();

    void HandleFetchImageSuccess(emscripten_fetch_t *fetch);

    void HandleFetchImageFailure(emscripten_fetch_t *fetch);

    void Init(const json& elementDef) override {
        Element::Init(elementDef);

        YGNodeSetContext(m_layoutNode->m_node, this);
        YGNodeSetMeasureFunc(m_layoutNode->m_node, Measure);

        FetchImage();
    }
};
