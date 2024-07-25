#include <yoga/Yoga.h>
#include <yoga/YGNode.h>
#include <nlohmann/json.hpp>

#include "shared.h"
#include "reactimgui.h"
#include "layout_node.h"

using json = nlohmann::json;

LayoutNode::LayoutNode() {
    m_node = YGNodeNew();
}

void LayoutNode::InsertChild(LayoutNode* child, size_t index) {
    YGNodeInsertChild(m_node, child->m_node, index);
};

std::optional<YGAlign> LayoutNode::ResolveAlignItems(std::string def) {
    std::optional<YGAlign> alignItems;

    if (def == "auto") {
        alignItems = YGAlignAuto;
    } else if (def == "flex-start") {
        alignItems = YGAlignFlexStart;
    } else if (def == "center") {
        alignItems = YGAlignCenter;
    } else if (def == "flex-end") {
        alignItems = YGAlignFlexEnd;
    } else if (def == "stretch") {
        alignItems = YGAlignStretch;
    } else if (def == "baseline") {
        alignItems = YGAlignBaseline;
    }

    return alignItems;
};

size_t LayoutNode::GetChildCount() {
    return YGNodeGetChildCount(m_node);
};

void LayoutNode::ApplyStyle(const json& styleDef) {
    if (styleDef.contains("direction") && styleDef["direction"].is_string()) {
        auto rawDirection = styleDef["direction"].template get<std::string>();
        std::optional<YGDirection> direction;

        if (rawDirection == "inherit") {
            direction = YGDirectionInherit;
        } else if (rawDirection == "ltr") {
            direction = YGDirectionLTR;
        } else if (rawDirection == "rtl") {
            direction = YGDirectionRTL;
        }

        if (direction.has_value()) {
            SetDirection(direction.value());
        }
    }
    
    if (styleDef.contains("flexDirection") && styleDef["flexDirection"].is_string()) {
        auto rawFlexDirection = styleDef["flexDirection"].template get<std::string>();
        std::optional<YGFlexDirection> flexDirection;

        if (rawFlexDirection == "column") {
            flexDirection = YGFlexDirectionColumn;
        } else if (rawFlexDirection == "column-reverse") {
            flexDirection = YGFlexDirectionColumnReverse;
        } else if (rawFlexDirection == "row") {
            flexDirection = YGFlexDirectionRow;
        } else if (rawFlexDirection == "row-reverse") {
            flexDirection = YGFlexDirectionRowReverse;
        }

        if (flexDirection.has_value()) {
            SetFlexDirection(flexDirection.value());
        }
    }
    
    if (styleDef.contains("justifyContent") && styleDef["justifyContent"].is_string()) {
        auto rawJustifyContent = styleDef["justifyContent"].template get<std::string>();
        std::optional<YGJustify> justifyContent;

        if (rawJustifyContent == "flex-start") {
            justifyContent = YGJustifyFlexStart;
        } else if (rawJustifyContent == "center") {
            justifyContent = YGJustifyCenter;
        } else if (rawJustifyContent == "flex-end") {
            justifyContent = YGJustifyFlexEnd;
        } else if (rawJustifyContent == "space-between") {
            justifyContent = YGJustifySpaceBetween;
        } else if (rawJustifyContent == "space-around") {
            justifyContent = YGJustifySpaceAround;
        } else if (rawJustifyContent == "space-evenly") {
            justifyContent = YGJustifySpaceEvenly;
        }

        if (justifyContent.has_value()) {
            SetJustifyContent(justifyContent.value());
        }
    }
    
    if (styleDef.contains("alignContent") && styleDef["alignContent"].is_string()) {
        auto rawAlignContent = styleDef["alignContent"].template get<std::string>();
        std::optional<YGAlign> alignContent;

        if (rawAlignContent == "auto") {
            alignContent = YGAlignAuto;
        } else if (rawAlignContent == "flex-start") {
            alignContent = YGAlignFlexStart;
        } else if (rawAlignContent == "center") {
            alignContent = YGAlignCenter;
        } else if (rawAlignContent == "flex-end") {
            alignContent = YGAlignFlexEnd;
        } else if (rawAlignContent == "stretch") {
            alignContent = YGAlignStretch;
        } else if (rawAlignContent == "space-between") {
            alignContent = YGAlignSpaceBetween;
        } else if (rawAlignContent == "space-around") {
            alignContent = YGAlignSpaceAround;
        } else if (rawAlignContent == "space-evenly") {
            alignContent = YGAlignSpaceEvenly;
        }

        if (alignContent.has_value()) {
            SetAlignContent(alignContent.value());
        }
    }
    
    if (styleDef.contains("alignItems") && styleDef["alignItems"].is_string()) {
        auto def = styleDef["alignItems"].template get<std::string>();
        std::optional<YGAlign> alignItems = ResolveAlignItems(def);

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
        std::optional<YGPositionType> positionType;

        if (rawPositionType == "static") {
            positionType = YGPositionTypeStatic;
        } else if (rawPositionType == "relative") {
            positionType = YGPositionTypeRelative;
        } else if (rawPositionType == "absolute") {
            positionType = YGPositionTypeAbsolute;
        }

        if (positionType.has_value()) {
            SetPositionType(positionType.value());
        }
    }
    
    if (styleDef.contains("flexWrap") && styleDef["flexWrap"].is_string()) {
        auto rawFlexWrap = styleDef["flexWrap"].template get<std::string>();
        std::optional<YGWrap> flexWrap;

        if (rawFlexWrap == "no-wrap") {
            flexWrap = YGWrapNoWrap;
        } else if (rawFlexWrap == "wrap") {
            flexWrap = YGWrapWrap;
        } else if (rawFlexWrap == "wrap-reverse") {
            flexWrap = YGWrapWrapReverse;
        }

        if (flexWrap.has_value()) {
            SetFlexWrap(flexWrap.value());
        }
    }
    
    if (styleDef.contains("overflow") && styleDef["overflow"].is_string()) {
        auto rawOverflow = styleDef["overflow"].template get<std::string>();
        std::optional<YGOverflow> overflow;

        if (rawOverflow == "visible") {
            overflow = YGOverflowVisible;
        } else if (rawOverflow == "hidden") {
            overflow = YGOverflowHidden;
        } else if (rawOverflow == "scroll") {
            overflow = YGOverflowScroll;
        }

        if (overflow.has_value()) {
            SetOverflow(overflow.value());
        }
    }
    
    if (styleDef.contains("display") && styleDef["display"].is_string()) {
        auto rawDisplay = styleDef["display"].template get<std::string>();
        std::optional<YGDisplay> display;

        if (rawDisplay == "flex") {
            display = YGDisplayFlex;
        } else if (rawDisplay == "none") {
            display = YGDisplayNone;
        }

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
            // todo: what about percentage? Does it make sense to handle it here or below with an explicit property?
            SetFlexBasis(styleDef["flexBasis"].template get<float>());
        } else if (styleDef["flexBasis"].is_string()) {
            auto flexBasis = styleDef["display"].template get<std::string>();

            if (flexBasis == "auto") {
                SetFlexBasisAuto();
            }
        }
    }
    
    if (styleDef.contains("flexBasisPercent") && styleDef["flexBasisPercent"].is_number()) {
        SetFlexBasisPercent(styleDef["flexBasisPercent"].template get<float>());
    }
    
    if (styleDef.contains("position") && styleDef["position"].is_object()) {
        for (auto& [key, item] : styleDef["position"].items()) {
            if (item.is_number()) {
                std::optional<YGEdge> edge = ResolveEdge(key);
                // todo: what about percentage?
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
                // todo: what about percentage?
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
                // todo: what about percentage?
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
                // todo: what about percentage?
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
            // todo: what about percentage?
            SetWidth(styleDef["width"].template get<float>());
        } else if (styleDef["width"].is_string()) {
            auto width = styleDef["width"].template get<std::string>();

            if (width == "auto") {
                SetWidthAuto();
            } else {
                SetWidthPercent(charPercentageToFloat(width.c_str()));
            }
        }
    }

    if (styleDef.contains("minWidth")) {
        if (styleDef["minWidth"].is_number()) {
            // todo: what about percentage?
            SetMinWidth(styleDef["minWidth"].template get<float>());
        }
    }

    if (styleDef.contains("maxWidth")) {
        if (styleDef["maxWidth"].is_number()) {
            // todo: what about percentage?
            SetMaxWidth(styleDef["maxWidth"].template get<float>());
        }
    }

    if (styleDef.contains("height")) {
        if (styleDef["height"].is_number()) {
            // todo: what about percentage?
            SetHeight(styleDef["height"].template get<float>());
        } else if (styleDef["height"].is_string()) {
            auto height = styleDef["height"].template get<std::string>();

            if (height == "auto") {
                SetHeightAuto();
            } else {
                SetHeightPercent(charPercentageToFloat(height.c_str()));
            }
        }
    }

    if (styleDef.contains("minHeight")) {
        if (styleDef["minHeight"].is_number()) {
            // todo: what about percentage?
            SetMinHeight(styleDef["minHeight"].template get<float>());
        }
    }

    if (styleDef.contains("maxHeight")) {
        if (styleDef["maxHeight"].is_number()) {
            // todo: what about percentage?
            SetMaxHeight(styleDef["maxHeight"].template get<float>());
        }
    }
};

std::optional<YGEdge> LayoutNode::ResolveEdge(const std::string& edgeKey) {
    std::optional<YGEdge> edge;

    if (edgeKey == "left") {
        edge = YGEdgeLeft;
    } else if (edgeKey == "top") {
        edge = YGEdgeTop;
    } else if (edgeKey == "right") {
        edge = YGEdgeRight;
    } else if (edgeKey == "bottom") {
        edge = YGEdgeBottom;
    } else if (edgeKey == "start") {
        edge = YGEdgeStart;
    } else if (edgeKey == "end") {
        edge = YGEdgeEnd;
    } else if (edgeKey == "horizontal") {
        edge = YGEdgeHorizontal;
    } else if (edgeKey == "vertical") {
        edge = YGEdgeVertical;
    } else if (edgeKey == "all") {
        edge = YGEdgeAll;
    }

    return edge;
}

std::optional<YGGutter> LayoutNode::ResolveGutter(const std::string& gutterKey) {
    std::optional<YGGutter> gutter;

    if (gutterKey == "column") {
        gutter = YGGutterColumn;
    } else if (gutterKey == "row") {
        gutter = YGGutterRow;
    } else if (gutterKey == "all") {
        gutter = YGGutterAll;
    }

    return gutter;
};
