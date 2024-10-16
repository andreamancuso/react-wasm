#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "color_helpers.h"

using ::testing::FloatEq;
using ::testing::Eq;

// Demonstrate some basic assertions.
TEST(ColorHelpersTest, RGBAtoIV4handlesZeroValues) {
    auto result = RGBAtoIV4(0, 0, 0, 0.0f);

    EXPECT_THAT(result.x, FloatEq(0.0f));
    EXPECT_THAT(result.y, FloatEq(0.0f));
    EXPECT_THAT(result.z, FloatEq(0.0f));
    EXPECT_THAT(result.w, FloatEq(0.0f));
}

TEST(ColorHelpersTest, RGBAtoIV4handlesNonZeroValues) {
    auto result = RGBAtoIV4(255, 255, 255, 0.5f);

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(0.5f));
}

TEST(ColorHelpersTest, RGBAtoIV4WithoutAlphaValuehandlesNonZeroValues) {
    auto result = RGBAtoIV4(255, 255, 255);

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(1.0f));
}

TEST(ColorHelpersTest, HEXAtoIV4ReturnsNulloptWhenItFailsToParseValue) {
    auto result = HEXAtoIV4(255, 255, 255);

    EXPECT_(result, Eq(std::nullopt));
}
