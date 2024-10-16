#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "color_helpers.h"

using ::testing::FloatEq;
using ::testing::Eq;
using ::testing::IsTrue;

// Demonstrate some basic assertions.
TEST(RGBAtoIV4, handlesZeroValues) {
    auto result = RGBAtoIV4(0, 0, 0, 0.0f);

    EXPECT_THAT(result.x, FloatEq(0.0f));
    EXPECT_THAT(result.y, FloatEq(0.0f));
    EXPECT_THAT(result.z, FloatEq(0.0f));
    EXPECT_THAT(result.w, FloatEq(0.0f));
}

TEST(RGBAtoIV4, handlesNonZeroValues) {
    auto result = RGBAtoIV4(255, 255, 255, 0.5f);

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(0.5f));
}

TEST(RGBAtoIV4, handlesDefaultAlphaValue) {
    auto result = RGBAtoIV4(255, 255, 255);

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(1.0f));
}

TEST(HEXAtoIV4, returnsNulloptWhenItFailsToParseValue) {
    auto result = HEXAtoIV4("");

    EXPECT_THAT(result, Eq(std::nullopt));
}

TEST(HEXAtoIV4, handlesFullLengthHexValuesWithDefaultAlphaValue) {
    auto result = HEXAtoIV4("#ffffff");

    EXPECT_THAT(result.value().x, FloatEq(1.0f));
    EXPECT_THAT(result.value().y, FloatEq(1.0f));
    EXPECT_THAT(result.value().z, FloatEq(1.0f));
    EXPECT_THAT(result.value().w, FloatEq(1.0f));
}

TEST(HEXAtoIV4, handlesFullLengthHexValuesWithSpecificAlphaValue) {
    auto result = HEXAtoIV4("#ffffff", 0.2f);

    EXPECT_THAT(result.value().x, FloatEq(1.0f));
    EXPECT_THAT(result.value().y, FloatEq(1.0f));
    EXPECT_THAT(result.value().z, FloatEq(1.0f));
    EXPECT_THAT(result.value().w, FloatEq(0.2f));
}

TEST(HEXAtoIV4, handlesShortHexValuesWithDefaultAlphaValue) {
    auto result = HEXAtoIV4("#fff");

    EXPECT_THAT(result.value().x, FloatEq(1.0f));
    EXPECT_THAT(result.value().y, FloatEq(1.0f));
    EXPECT_THAT(result.value().z, FloatEq(1.0f));
    EXPECT_THAT(result.value().w, FloatEq(1.0f));
}

TEST(IV4toJson, returnsJsonInstanceWithExpectedProperties) {
    auto result = IV4toJson(ImVec4(0, 0, 0, 0.0f));

    EXPECT_THAT(result.is_object(), IsTrue());
}
