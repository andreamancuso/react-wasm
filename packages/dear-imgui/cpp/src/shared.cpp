#include <string>

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


