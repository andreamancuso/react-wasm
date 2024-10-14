#include <unordered_map>
#include <variant>
#include "imgui.h"

#ifndef IMGUI_HELPERS_H
#define IMGUI_HELPERS_H

using StyleVarValue = std::variant<std::monostate, ImVec2, float>;
using StyleVarValueRef = std::variant<std::monostate, const ImVec2*, const float*>;
using StyleColors = std::unordered_map<ImGuiCol, ImVec4>;
using StyleVars = std::unordered_map<ImGuiStyleVar, StyleVarValue>;
using WidgetColors = std::tuple<std::optional<StyleColors>, std::optional<StyleColors>>;

ImDrawFlags cornersToDrawFlags(ImDrawFlags accumulator, std::string_view side);

#endif //IMGUI_HELPERS_H