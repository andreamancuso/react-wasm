#include <sstream>
#include <cstdio>
#include <imgui.h>

#include "imgui_helpers.h"

ImDrawFlags cornersToDrawFlags(ImDrawFlags accumulator, const std::string_view side) {
    if (side == "all") {
        accumulator |= ImDrawFlags_RoundCornersAll;
    } else if (side == "topLeft") {
        printf("topLeft\n");

        accumulator |= ImDrawFlags_RoundCornersTopLeft;
    } else if (side == "topRight") {
        printf("topRight\n");

        accumulator |= ImDrawFlags_RoundCornersTopRight;
    } else if (side == "bottomLeft") {
        accumulator |= ImDrawFlags_RoundCornersBottomLeft;
    } else if (side == "bottomRight") {
        accumulator |= ImDrawFlags_RoundCornersBottomRight;
    }

    return accumulator;
};