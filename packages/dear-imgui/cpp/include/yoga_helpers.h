#include <optional>
#include <yoga/YGEnums.h>

#ifndef YOGA_HELPERS_H
#define YOGA_HELPERS_H

std::optional<YGAlign> ResolveAlignItems(std::string_view value);
std::optional<YGAlign> ResolveAlignContent(std::string_view value);
std::optional<YGJustify> ResolveJustifyContent(std::string_view value);
std::optional<YGFlexDirection> ResolveFlexDirection(std::string_view value);
std::optional<YGDirection> ResolveDirection(std::string_view value);
std::optional<YGPositionType> ResolvePositionType(std::string_view value);
std::optional<YGWrap> ResolveFlexWrap(std::string_view value);
std::optional<YGOverflow> ResolveOverflow(std::string_view value);
std::optional<YGDisplay> ResolveDisplay(std::string_view value);
std::optional<YGEdge> ResolveEdge(std::string_view edgeKey);
std::optional<YGGutter> ResolveGutter(std::string_view gutterKey);

#endif //YOGA_HELPERS_H
