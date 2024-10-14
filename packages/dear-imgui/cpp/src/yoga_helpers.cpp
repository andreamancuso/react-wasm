#include <string>
#include <optional>
#include <yoga/YGEnums.h>

std::optional<YGAlign> ResolveAlignItems(std::string_view value) {
    std::optional<YGAlign> alignItems;

    if (value == "auto") {
        alignItems = YGAlignAuto;
    } else if (value == "flex-start") {
        alignItems = YGAlignFlexStart;
    } else if (value == "center") {
        alignItems = YGAlignCenter;
    } else if (value == "flex-end") {
        alignItems = YGAlignFlexEnd;
    } else if (value == "stretch") {
        alignItems = YGAlignStretch;
    } else if (value == "baseline") {
        alignItems = YGAlignBaseline;
    }

    return alignItems;
};

std::optional<YGAlign> ResolveAlignContent(std::string_view value) {
    std::optional<YGAlign> alignContent;

    if (value == "auto") {
        alignContent = YGAlignAuto;
    } else if (value == "flex-start") {
        alignContent = YGAlignFlexStart;
    } else if (value == "center") {
        alignContent = YGAlignCenter;
    } else if (value == "flex-end") {
        alignContent = YGAlignFlexEnd;
    } else if (value == "stretch") {
        alignContent = YGAlignStretch;
    } else if (value == "space-between") {
        alignContent = YGAlignSpaceBetween;
    } else if (value == "space-around") {
        alignContent = YGAlignSpaceAround;
    } else if (value == "space-evenly") {
        alignContent = YGAlignSpaceEvenly;
    }

    return alignContent;
};

std::optional<YGJustify> ResolveJustifyContent(std::string_view value) {
    std::optional<YGJustify> justifyContent;

    if (value == "flex-start") {
        justifyContent = YGJustifyFlexStart;
    } else if (value == "center") {
        justifyContent = YGJustifyCenter;
    } else if (value == "flex-end") {
        justifyContent = YGJustifyFlexEnd;
    } else if (value == "space-between") {
        justifyContent = YGJustifySpaceBetween;
    } else if (value == "space-around") {
        justifyContent = YGJustifySpaceAround;
    } else if (value == "space-evenly") {
        justifyContent = YGJustifySpaceEvenly;
    }

    return justifyContent;
};

std::optional<YGFlexDirection> ResolveFlexDirection(std::string_view value) {
    std::optional<YGFlexDirection> flexDirection;

    if (value == "column") {
        flexDirection = YGFlexDirectionColumn;
    } else if (value == "column-reverse") {
        flexDirection = YGFlexDirectionColumnReverse;
    } else if (value == "row") {
        flexDirection = YGFlexDirectionRow;
    } else if (value == "row-reverse") {
        flexDirection = YGFlexDirectionRowReverse;
    }

    return flexDirection;
};

std::optional<YGDirection> ResolveDirection(std::string_view value) {
    std::optional<YGDirection> direction;

    if (value == "inherit") {
        direction = YGDirectionInherit;
    } else if (value == "ltr") {
        direction = YGDirectionLTR;
    } else if (value == "rtl") {
        direction = YGDirectionRTL;
    }

    return direction;
};

std::optional<YGPositionType> ResolvePositionType(std::string_view value) {
    std::optional<YGPositionType> positionType;

    if (value == "static") {
        positionType = YGPositionTypeStatic;
    } else if (value == "relative") {
        positionType = YGPositionTypeRelative;
    } else if (value == "absolute") {
        positionType = YGPositionTypeAbsolute;
    }

    return positionType;
};

std::optional<YGWrap> ResolveFlexWrap(std::string_view value) {
    std::optional<YGWrap> flexWrap;

    if (value == "no-wrap") {
        flexWrap = YGWrapNoWrap;
    } else if (value == "wrap") {
        flexWrap = YGWrapWrap;
    } else if (value == "wrap-reverse") {
        flexWrap = YGWrapWrapReverse;
    }

    return flexWrap;
};

std::optional<YGOverflow> ResolveOverflow(std::string_view value) {
    std::optional<YGOverflow> overflow;

    if (value == "visible") {
        overflow = YGOverflowVisible;
    } else if (value == "hidden") {
        overflow = YGOverflowHidden;
    } else if (value == "scroll") {
        overflow = YGOverflowScroll;
    }

    return overflow;
};

std::optional<YGDisplay> ResolveDisplay(std::string_view value) {
    std::optional<YGDisplay> display;

    if (value == "flex") {
        display = YGDisplayFlex;
    } else if (value == "none") {
        display = YGDisplayNone;
    }

    return display;
};

std::optional<YGEdge> ResolveEdge(std::string_view edgeKey) {
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

std::optional<YGGutter> ResolveGutter(std::string_view gutterKey) {
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


