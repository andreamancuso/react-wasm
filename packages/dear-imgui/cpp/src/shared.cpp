#include <string>
#include <sstream>
#include <iomanip>
#include <webgpu/webgpu.h>
#include "csscolorparser.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui.h"
#include "shared.h"

#pragma once



std::optional<float> charPercentageToFloat(const char* input) {
    std::optional<float> value;
    float parsedValue;

    if (auto outcome = std::sscanf(input, "%f%%", &parsedValue); outcome != EOF && outcome > 0) {
        value.emplace(parsedValue);
    }

    return value;
}


