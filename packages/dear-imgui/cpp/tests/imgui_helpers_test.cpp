#include <numeric>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <imgui_internal.h>

#include "imgui_helpers.h"

using ::testing::Eq;
using ::testing::Gt;

TEST(cornersToDrawFlags, convertsStringVectorIntoImDrawFlags) {
    std::vector<std::string> input = { "all", "topLeft", "topRight", "bottomLeft", "bottomRight" };

    auto result = std::accumulate(
        input.begin(),
        input.end(),
        static_cast<ImDrawFlags>(ImDrawFlags_None),
        cornersToDrawFlags
    );

    EXPECT_THAT(result & ImDrawFlags_RoundCornersAll, Gt(0));
    EXPECT_THAT(result & ImDrawFlags_RoundCornersTopLeft, Gt(0));
    EXPECT_THAT(result & ImDrawFlags_RoundCornersTopRight, Gt(0));
    EXPECT_THAT(result & ImDrawFlags_RoundCornersBottomLeft, Gt(0));
    EXPECT_THAT(result & ImDrawFlags_RoundCornersBottomRight, Gt(0));
}

TEST(cornersToDrawFlags, handlesEmptyVector) {
    std::vector<std::string> input = { };

    auto result = std::accumulate(
        input.begin(),
        input.end(),
        static_cast<ImDrawFlags>(ImDrawFlags_None),
        cornersToDrawFlags
    );

    EXPECT_THAT(result, Eq(0));
}

TEST(cornersToDrawFlags, handlesVectorWithUnrecognisedValues) {
    std::vector<std::string> input = { "value" };

    auto result = std::accumulate(
        input.begin(),
        input.end(),
        static_cast<ImDrawFlags>(ImDrawFlags_None),
        cornersToDrawFlags
    );

    EXPECT_THAT(result, Eq(0));
}