#include <yoga/Yoga.h>
#include <yoga/YGNode.h>
#include <nlohmann/json.hpp>

#include "shared.h"
#include "element/layout_node.h"

using json = nlohmann::json;

LayoutNode::LayoutNode() {
    m_node = YGNodeNew();
}

void LayoutNode::InsertChild(LayoutNode* child, size_t index) const {
    YGNodeInsertChild(m_node, child->m_node, index);
};

size_t LayoutNode::GetChildCount() const {
    return YGNodeGetChildCount(m_node);
};

void LayoutNode::ApplyStyle(const json& styleDef) {
    if (styleDef.contains("direction") && styleDef["direction"].is_string()) {
        auto rawDirection = styleDef["direction"].template get<std::string>();
        std::optional<YGDirection> direction = ResolveDirection(rawDirection);

        if (direction.has_value()) {
            SetDirection(direction.value());
        }
    }
    
    if (styleDef.contains("flexDirection") && styleDef["flexDirection"].is_string()) {
        auto rawFlexDirection = styleDef["flexDirection"].template get<std::string>();
        std::optional<YGFlexDirection> flexDirection = ResolveFlexDirection(rawFlexDirection);

        if (flexDirection.has_value()) {
            SetFlexDirection(flexDirection.value());
        }
    }
    
    if (styleDef.contains("justifyContent") && styleDef["justifyContent"].is_string()) {
        auto rawJustifyContent = styleDef["justifyContent"].template get<std::string>();
        std::optional<YGJustify> justifyContent = ResolveJustifyContent(rawJustifyContent);

        if (justifyContent.has_value()) {
            SetJustifyContent(justifyContent.value());
        }
    }
    
    if (styleDef.contains("alignContent") && styleDef["alignContent"].is_string()) {
        auto rawAlignContent = styleDef["alignContent"].template get<std::string>();
        std::optional<YGAlign> alignContent = ResolveAlignItems(rawAlignContent);

        if (alignContent.has_value()) {
            SetAlignContent(alignContent.value());
        }
    }
    
    if (styleDef.contains("alignItems") && styleDef["alignItems"].is_string()) {
        auto rawAlignItems = styleDef["alignItems"].template get<std::string>();
        std::optional<YGAlign> alignItems = ResolveAlignItems(rawAlignItems);

        if (alignItems.has_value()) {
            SetAlignItems(alignItems.value());
        }
    }
    
    if (styleDef.contains("alignSelf") && styleDef["alignSelf"].is_string()) {
        auto def = styleDef["alignSelf"].template get<std::string>();
        std::optional<YGAlign> alignSelf = ResolveAlignItems(def);

        if (alignSelf.has_value()) {
            SetAlignSelf(alignSelf.value());
        }
    }
    
    if (styleDef.contains("positionType") && styleDef["positionType"].is_string()) {
        auto rawPositionType = styleDef["positionType"].template get<std::string>();
        std::optional<YGPositionType> positionType = ResolvePositionType(rawPositionType);

        if (positionType.has_value()) {
            SetPositionType(positionType.value());
        }
    }
    
    if (styleDef.contains("flexWrap") && styleDef["flexWrap"].is_string()) {
        auto rawFlexWrap = styleDef["flexWrap"].template get<std::string>();
        std::optional<YGWrap> flexWrap = ResolveFlexWrap(rawFlexWrap);

        if (flexWrap.has_value()) {
            SetFlexWrap(flexWrap.value());
        }
    }
    
    if (styleDef.contains("overflow") && styleDef["overflow"].is_string()) {
        auto rawOverflow = styleDef["overflow"].template get<std::string>();
        std::optional<YGOverflow> overflow = ResolveOverflow(rawOverflow);

        if (overflow.has_value()) {
            SetOverflow(overflow.value());
        }
    }
    
    if (styleDef.contains("display") && styleDef["display"].is_string()) {
        auto rawDisplay = styleDef["display"].template get<std::string>();
        std::optional<YGDisplay> display = ResolveDisplay(rawDisplay);

        if (display.has_value()) {
            SetDisplay(display.value());
        }
    }
    
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
    
    if (styleDef.contains("position") && styleDef["position"].is_object()) {
        for (auto& [key, item] : styleDef["position"].items()) {
            if (item.is_number()) {
                std::optional<YGEdge> edge = ResolveEdge(key);
                if (edge.has_value()) {
                    SetPosition(edge.value(), item.template get<float>());
                }
            }
        }
    }
    
    if (styleDef.contains("margin") && styleDef["margin"].is_object()) {
        for (auto& [key, item] : styleDef["margin"].items()) {
            if (item.is_number()) {
                std::optional<YGEdge> edge = ResolveEdge(key);
                if (edge.has_value()) {
                    SetMargin(edge.value(), item.template get<float>());
                }
            }
        }
    }
    
    if (styleDef.contains("padding") && styleDef["padding"].is_object()) {
        for (auto& [key, item] : styleDef["padding"].items()) {
            if (item.is_number()) {
                std::optional<YGEdge> edge = ResolveEdge(key);
                if (edge.has_value()) {
                    SetPadding(edge.value(), item.template get<float>());
                }
            }
        }
    }
    
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
                // todo: what about percentage?
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

