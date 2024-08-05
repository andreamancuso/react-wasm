#include <yoga/Yoga.h>
#include <yoga/YGNode.h>
#include <nlohmann/json.hpp>

#include "shared.h"
#include "element/layout_node.h"

using json = nlohmann::json;

LayoutNode::LayoutNode() {
    using namespace std::placeholders;

    m_node = YGNodeNew();

    AddSetter<YGDirection>("direction", std::function<void(YGDirection)>(std::bind(&LayoutNode::SetDirection, this, _1)));
    AddSetter<YGFlexDirection>("flexDirection", std::function<void(YGFlexDirection)>(std::bind(&LayoutNode::SetFlexDirection, this, _1)));
    AddSetter<YGJustify>("justifyContent", std::function<void(YGJustify)>(std::bind(&LayoutNode::SetJustifyContent, this, _1)));
    AddSetter<YGAlign>("alignContent", std::function<void(YGAlign)>(std::bind(&LayoutNode::SetAlignContent, this, _1)));
    AddSetter<YGAlign>("alignItems", std::function<void(YGAlign)>(std::bind(&LayoutNode::SetAlignItems, this, _1)));
    AddSetter<YGAlign>("alignSelf", std::function<void(YGAlign)>(std::bind(&LayoutNode::SetAlignSelf, this, _1)));
    AddSetter<YGPositionType>("positionType", std::function<void(YGPositionType)>(std::bind(&LayoutNode::SetPositionType, this, _1)));
    AddSetter<YGWrap>("flexWrap", std::function<void(YGWrap)>(std::bind(&LayoutNode::SetFlexWrap, this, _1)));
    AddSetter<YGOverflow>("overflow", std::function<void(YGOverflow)>(std::bind(&LayoutNode::SetOverflow, this, _1)));
    AddSetter<YGDisplay>("display", std::function<void(YGDisplay)>(std::bind(&LayoutNode::SetDisplay, this, _1)));

    AddSetter<YGEdge, float>("padding", std::function<void(YGEdge, float)>(std::bind(&LayoutNode::SetPadding, this, _1, _2)));
    AddSetter<YGEdge, float>("margin", std::function<void(YGEdge, float)>(std::bind(&LayoutNode::SetMargin, this, _1, _2)));
    AddSetter<YGEdge, float>("position", std::function<void(YGEdge, float)>(std::bind(&LayoutNode::SetPosition, this, _1, _2)));
}

void LayoutNode::InsertChild(LayoutNode* child, size_t index) const {
    YGNodeInsertChild(m_node, child->m_node, index);
};

size_t LayoutNode::GetChildCount() const {
    return YGNodeGetChildCount(m_node);
};

void LayoutNode::ApplyStyle(const json& styleDef) const {
    ApplyOptionalStyleProperty<YGDirection>(styleDef, "direction", ResolveDirection);
    ApplyOptionalStyleProperty<YGFlexDirection>(styleDef, "flexDirection", ResolveFlexDirection);
    ApplyOptionalStyleProperty<YGJustify>(styleDef, "justifyContent", ResolveJustifyContent);
    ApplyOptionalStyleProperty<YGAlign>(styleDef, "alignContent", ResolveAlignContent);
    ApplyOptionalStyleProperty<YGAlign>(styleDef, "alignItems", ResolveAlignItems);
    ApplyOptionalStyleProperty<YGAlign>(styleDef, "alignSelf", ResolveAlignItems);
    ApplyOptionalStyleProperty<YGPositionType>(styleDef, "positionType", ResolvePositionType);
    ApplyOptionalStyleProperty<YGWrap>(styleDef, "flexWrap", ResolveFlexWrap);
    ApplyOptionalStyleProperty<YGOverflow>(styleDef, "overflow", ResolveOverflow);
    ApplyOptionalStyleProperty<YGDisplay>(styleDef, "display", ResolveDisplay);

    if (styleDef.contains("flex") && styleDef["flex"].is_number()) {
        SetFlex(styleDef["flex"].template get<float>());
    }
    
    if (styleDef.contains("flexGrow") && styleDef["flexGrow"].is_number()) {
        SetFlexGrow(styleDef["flexGrow"].template get<float>());
    }
    
    if (styleDef.contains("flexShrink") && styleDef["flexShrink"].is_number()) {
        SetFlexShrink(styleDef["flexShrink"].template get<float>());
    }
    
    if (styleDef.contains("flexBasis")) {
        if (styleDef["flexBasis"].is_number()) {
            SetFlexBasis(styleDef["flexBasis"].template get<float>());
        } else if (styleDef["flexBasis"].is_string()) {
            auto flexBasis = styleDef["flexBasis"].template get<std::string>();

            if (flexBasis == "auto") {
                SetFlexBasisAuto();
            } else if (auto maybePct = charPercentageToFloat(flexBasis.c_str()); maybePct.has_value()) {
                SetFlexBasisPercent(maybePct.value());
            }
        }
    }

    ApplyOptionalMultiEdgeStyleProperty<YGEdge, float>(styleDef, "position", ResolveEdge);
    ApplyOptionalMultiEdgeStyleProperty<YGEdge, float>(styleDef, "margin", ResolveEdge);
    ApplyOptionalMultiEdgeStyleProperty<YGEdge, float>(styleDef, "padding", ResolveEdge);

    // TODO: Border must be applied equally on all 4 sides
    if (styleDef.contains("border") && styleDef["border"].is_object()) {
        for (auto& [key, item] : styleDef["border"].items()) {
            if (item.is_number()) {
                std::optional<YGEdge> edge = ResolveEdge(key);
                if (edge.has_value()) {
                    SetBorder(edge.value(), item.template get<float>());
                }
            }
        }
    }
    
    if (styleDef.contains("gap") && styleDef["gap"].is_object()) {
        for (auto& [key, item] : styleDef["gap"].items()) {
            if (item.is_number()) {
                std::optional<YGGutter> gutter = ResolveGutter(key);
                if (gutter.has_value()) {
                    SetGap(gutter.value(), item.template get<float>());
                }
            }
        }
    }

    if (styleDef.contains("aspectRatio") && styleDef["aspectRatio"].is_number()) {
        SetAspectRatio(styleDef["aspectRatio"].template get<float>());
    }

    if (styleDef.contains("width")) {
        if (styleDef["width"].is_number()) {
            SetWidth(styleDef["width"].template get<float>());
        } else if (styleDef["width"].is_string()) {
            auto width = styleDef["width"].template get<std::string>();

            if (width == "auto") {
                SetWidthAuto();
            } else if (auto maybePct = charPercentageToFloat(width.c_str()); maybePct.has_value()) {
                SetWidthPercent(maybePct.value());
            }
        }
    }

    if (styleDef.contains("minWidth")) {
        if (styleDef["minWidth"].is_number()) {
            SetMinWidth(styleDef["minWidth"].template get<float>());
        } else if (styleDef["minWidth"].is_string()) {
            auto minWidth = styleDef["minWidth"].template get<std::string>();

            if (auto maybePct = charPercentageToFloat(minWidth.c_str()); maybePct.has_value()) {
                SetMinWidthPercent(maybePct.value());
            }
        }
    }

    if (styleDef.contains("maxWidth")) {
        if (styleDef["maxWidth"].is_number()) {
            SetMaxWidth(styleDef["maxWidth"].template get<float>());
        } else if (styleDef["maxWidth"].is_string()) {
            auto maxWidth = styleDef["maxWidth"].template get<std::string>();

            if (auto maybePct = charPercentageToFloat(maxWidth.c_str()); maybePct.has_value()) {
                SetMaxWidthPercent(maybePct.value());
            }
        }
    }

    if (styleDef.contains("height")) {
        if (styleDef["height"].is_number()) {
            SetHeight(styleDef["height"].template get<float>());
        } else if (styleDef["height"].is_string()) {
            auto height = styleDef["height"].template get<std::string>();

            if (height == "auto") {
                SetHeightAuto();
            } else if (auto maybePct = charPercentageToFloat(height.c_str()); maybePct.has_value()) {
                SetHeightPercent(maybePct.value());
            }
        }
    }

    if (styleDef.contains("minHeight")) {
        if (styleDef["minHeight"].is_number()) {
            SetMinHeight(styleDef["minHeight"].template get<float>());
        } else if (styleDef["minHeight"].is_string()) {
            auto minHeight = styleDef["minHeight"].template get<std::string>();

            if (auto maybePct = charPercentageToFloat(minHeight.c_str()); maybePct.has_value()) {
                SetMinHeightPercent(maybePct.value());
            }
        }
    }

    if (styleDef.contains("maxHeight")) {
        if (styleDef["maxHeight"].is_number()) {
            SetMaxHeight(styleDef["maxHeight"].template get<float>());
        } else if (styleDef["maxHeight"].is_string()) {
            auto maxHeight = styleDef["maxHeight"].template get<std::string>();

            if (auto maybePct = charPercentageToFloat(maxHeight.c_str()); maybePct.has_value()) {
                SetMaxHeightPercent(maybePct.value());
            }
        }
    }
};

