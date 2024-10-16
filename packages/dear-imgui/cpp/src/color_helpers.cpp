#include <sstream>
#include <string>
#include <optional>
#include "csscolorparser.hpp"
#include <imgui.h>

#include "color_helpers.h"

ImVec4 RGBAtoIV4(int r, int g, int b, float a) {
    float newr = r / 255.0;
    float newg = g / 255.0;
    float newb = b / 255.0;

    return {newr, newg, newb, a};
}
ImVec4 RGBAtoIV4(int r, int g, int b) {
    return RGBAtoIV4(r, g, b, 1.0f);
}

std::optional<ImVec4> HEXAtoIV4(const std::string& hex, float a) {
    auto maybeColor = CSSColorParser::parse(hex);

    if (maybeColor.has_value()) {
        CSSColorParser::Color& color = maybeColor.value();

        return RGBAtoIV4(color.r, color.g, color.b, a);
    }

    return std::nullopt;
}

std::optional<ImVec4> HEXAtoIV4(const std::string& hex) {
    return HEXAtoIV4(hex, 1.0f);
}

json IV4toJson(const ImVec4& imVec4) {
    json j = {
        {"x", imVec4.x},
        {"y", imVec4.y},
        {"z", imVec4.z},
        {"w", imVec4.w}
    };

    return j;
};

json IV4toJsonTuple(const ImVec4& imVec4) {
    json j = {
        imVec4.x,
        imVec4.y,
        imVec4.z,
        imVec4.w
    };

    return j;
};

CSSColorParser::Color IV4toCSSColor(const ImVec4& imVec4) {
    CSSColorParser::Color color;

    color.r = (int)(imVec4.x * 255);
    color.g = (int)(imVec4.y * 255);
    color.b = (int)(imVec4.z * 255);
    color.a = imVec4.w;

    return color;
};

json IV4toJsonRGBATuple(const ImVec4& imVec4) {
    json j = {
        (int)(imVec4.x * 255),
        (int)(imVec4.y * 255),
        (int)(imVec4.z * 255),
        imVec4.w
    };

    return j;
};

HEXA IV4toHEXATuple(const ImVec4& imVec4) {
    auto rAsInt = (int)(imVec4.x * 255);
    auto gAsInt = (int)(imVec4.y * 255);
    auto bAsInt = (int)(imVec4.z * 255);

    std::string h = "#";

    std::ostringstream rSs;
    rSs << std::setfill('0') << std::setw(2) << std::hex << rAsInt;
    h += rSs.str();

    std::ostringstream gSs;
    gSs << std::setfill('0') << std::setw(2) << std::hex << gAsInt;
    h +=  gSs.str();

    std::ostringstream bSs;
    bSs << std::setfill('0') << std::setw(2) << std::hex << bAsInt;
    h += bSs.str();

    return std::make_tuple(h, imVec4.w);
};

json IV4toJsonHEXATuple(const ImVec4& imVec4) {
    auto [hex, a] = IV4toHEXATuple(imVec4);

    json j = {hex, a };

    return j;
};

std::optional<ImVec4> jsonHEXATupleToIV4(const json& tupleDef) {
    if (!tupleDef.is_array() || tupleDef.size() != 2
        || !tupleDef[0].is_string() || !tupleDef[1].is_number()) {
        printf("Color definition not recognised: %s\n", tupleDef.dump().c_str());

        return std::nullopt;
        }

    const auto color = tupleDef[0].template get<std::string>();
    const auto alpha = tupleDef[1].template get<float>();

    return HEXAtoIV4(color, alpha);
};

std::optional<ImVec4> extractColor(const json& item) {
    if (item.is_string()) {
        return HEXAtoIV4(item.template get<std::string>());
    }

    return jsonHEXATupleToIV4(item);
};