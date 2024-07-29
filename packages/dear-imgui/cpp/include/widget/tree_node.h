#include "styled_widget.h"

class TreeNode final : public StyledWidget {
public:
    std::string m_label;

    static std::unique_ptr<TreeNode> makeWidget(const json& widgetDef, std::optional<BaseStyle> maybeStyle, ReactImgui* view) {
        if (widgetDef.is_object() && widgetDef.contains("id") && widgetDef["id"].is_number_integer()) {
            auto id = widgetDef["id"].template get<int>();
            std::string label = widgetDef["label"].template get<std::string>();

            return std::make_unique<TreeNode>(view, id, label, maybeStyle);
        }

        throw std::invalid_argument("Invalid JSON data");
    }

    TreeNode(ReactImgui* view, int id, const std::string& label, std::optional<BaseStyle>& style);

    void Render(ReactImgui* view) override;

    void Patch(const json& widgetPatchDef, ReactImgui* view) override;

    bool HasCustomWidth() override;

    bool HasCustomHeight() override;
};
