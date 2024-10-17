#include <sstream>
#include <string>
#include <optional>
#include "csscolorparser.hpp"
#include <imgui.h>

#include "color_helpers.h"

bool areRGBAValuesValidForImVec4Conversion(const int& r, const int& g, const int& b, const float& a) {
    if (r < 0 || r > 255) {
        return false;
    }

    if (g < 0 || g > 255) {
        return false;
    }

    if (b < 0 || b > 255) {
        return false;
    }

    if (a < 0.0f || a > 1.0f) {
        return false;
    }

    return true;
};

bool isImVec4ValidForRGBAConversion(const ImVec4& imVec4) {
    if (imVec4.x < 0.0f || imVec4.x > 1.0f) {
        return false;
    }

    if (imVec4.y < 0.0f || imVec4.y > 1.0f) {
        return false;
    }

    if (imVec4.z < 0.0f || imVec4.z > 1.0f) {
        return false;
    }

    if (imVec4.w < 0.0f || imVec4.w > 1.0f) {
        return false;
    }

    return true;
};

std::optional<ImVec4> RGBAtoIV4(int r, int g, int b, float a) {
    if (!areRGBAValuesValidForImVec4Conversion(r, g, b, a)) {
        return std::nullopt;
    }

    float newr = r / 255.0;
    float newg = g / 255.0;
    float newb = b / 255.0;

    return ImVec4(newr, newg, newb, a);
}
std::optional<ImVec4> RGBAtoIV4(int r, int g, int b) {
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

std::optional<json> IV4toJson(const ImVec4& imVec4) {
    if (!isImVec4ValidForRGBAConversion(imVec4)) {
        return std::nullopt;
    }

    json j = {
        {"x", imVec4.x},
        {"y", imVec4.y},
        {"z", imVec4.z},
        {"w", imVec4.w}
    };

    return j;
};

std::optional<json> IV4toJsonTuple(const ImVec4& imVec4) {
    if (!isImVec4ValidForRGBAConversion(imVec4)) {
        return std::nullopt;
    }

    json j = {
        imVec4.x,
        imVec4.y,
        imVec4.z,
        imVec4.w
    };

    return j;
};

std::optional<CSSColorParser::Color> IV4toCSSColor(const ImVec4& imVec4) {
    if (!isImVec4ValidForRGBAConversion(imVec4)) {
        return std::nullopt;
    }

    CSSColorParser::Color color;

    color.r = (int)(imVec4.x * 255);
    color.g = (int)(imVec4.y * 255);
    color.b = (int)(imVec4.z * 255);
    color.a = imVec4.w;

    return color;
};

std::optional<json> IV4toJsonRGBATuple(const ImVec4& imVec4) {
    if (!isImVec4ValidForRGBAConversion(imVec4)) {
        return std::nullopt;
    }

    json j = {
        (int)(imVec4.x * 255),
        (int)(imVec4.y * 255),
        (int)(imVec4.z * 255),
        imVec4.w
    };

    return j;
};

std::optional<HEXA> IV4toHEXATuple(const ImVec4& imVec4) {
    if (!isImVec4ValidForRGBAConversion(imVec4)) {
        return std::nullopt;
    }

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

std::optional<json> IV4toJsonHEXATuple(const ImVec4& imVec4) {
    auto maybeTuple = IV4toHEXATuple(imVec4);

    if (!maybeTuple.has_value()) {
        return std::nullopt;
    }

    auto [hex, a] = maybeTuple.value();

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