#include "csscolorparser.hpp"
#include "imgui.h"
#include <nlohmann/json.hpp>

#include "shared.h"

#ifndef COLOR_HELPERS_H
#define COLOR_HELPERS_H

using json = nlohmann::json;

using HEXA = std::tuple<std::string, float>;

bool areRGBAValuesValidForImVec4Conversion(const int& r, const int& g, const int& b, const float& a);
bool isImVec4ValidForRGBAConversion(const ImVec4& imVec4);

std::optional<ImVec4> RGBAtoIV4(int r, int g, int b, float a);
std::optional<ImVec4> RGBAtoIV4(int r, int g, int b);

std::optional<ImVec4> HEXAtoIV4(const std::string& hex, float a);
std::optional<ImVec4> HEXAtoIV4(const std::string& hex);

std::optional<json> IV4toJson(const ImVec4& imVec4);
std::optional<json> IV4toJsonTuple(const ImVec4& imVec4);
std::optional<json> IV4toJsonRGBATuple(const ImVec4& imVec4);
std::optional<json> IV4toJsonHEXATuple(const ImVec4& imVec4);

std::optional<CSSColorParser::Color> IV4toCSSColor(const ImVec4& imVec4);
std::optional<HEXA> IV4toHEXATuple(const ImVec4& imVec4);

std::optional<ImVec4> jsonHEXATupleToIV4(const json& tupleDef);

std::optional<ImVec4> extractColor(const json& colorDef);

#endif //COLOR_HELPERS_H