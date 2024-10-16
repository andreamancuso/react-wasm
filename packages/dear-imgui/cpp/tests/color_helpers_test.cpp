#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "color_helpers.h"

using ::testing::FloatEq;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::IsTrue;
using ::testing::StrEq;

// Demonstrate some basic assertions.
TEST(RGBAtoIV4, handlesZeroValues) {
    auto maybeResult = RGBAtoIV4(0, 0, 0, 0.0f);

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result.x, FloatEq(0.0f));
    EXPECT_THAT(result.y, FloatEq(0.0f));
    EXPECT_THAT(result.z, FloatEq(0.0f));
    EXPECT_THAT(result.w, FloatEq(0.0f));
}

TEST(RGBAtoIV4, handlesNonZeroValues) {
    auto maybeResult = RGBAtoIV4(255, 255, 255, 0.5f);

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(0.5f));
}

TEST(RGBAtoIV4, handlesDefaultAlphaValue) {
    auto maybeResult = RGBAtoIV4(255, 255, 255);

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

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
    auto result = HEXAtoIV4("#ffffff").value();

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(1.0f));
}

TEST(HEXAtoIV4, handlesFullLengthHexValuesWithSpecificAlphaValue) {
    auto result = HEXAtoIV4("#ffffff", 0.2f).value();

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(0.2f));
}

TEST(HEXAtoIV4, handlesShortHexValuesWithDefaultAlphaValue) {
    auto result = HEXAtoIV4("#fff").value();

    EXPECT_THAT(result.x, FloatEq(1.0f));
    EXPECT_THAT(result.y, FloatEq(1.0f));
    EXPECT_THAT(result.z, FloatEq(1.0f));
    EXPECT_THAT(result.w, FloatEq(1.0f));
}

TEST(IV4toJson, returnsJsonObject) {
    auto maybeResult = IV4toJson(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result.is_object(), IsTrue());
    EXPECT_THAT(result["x"].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result["y"].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result["z"].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result["w"].template get<float>(), FloatEq(1.0f));
}

TEST(IV4toJson, handlesInvalidValues) {
    auto result = IV4toJson(ImVec4(10.0f, 10.0f, 10.0f, 1.0f));
    EXPECT_THAT(result, Eq(std::nullopt));
}

TEST(IV4toJsonTuple, returnsJsonArray) {
    auto maybeResult = IV4toJsonTuple(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result.is_array(), IsTrue());
    EXPECT_THAT(result[0].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result[1].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result[2].template get<float>(), FloatEq(1.0f));
    EXPECT_THAT(result[3].template get<float>(), FloatEq(1.0f));
}

TEST(IV4toJsonTuple, handlesInvalidValues) {
    auto result = IV4toJsonTuple(ImVec4(10.0f, 10.0f, 10.0f, 1.0f));
    EXPECT_THAT(result, Eq(std::nullopt));
}

TEST(IV4toCSSColor, returnsColorInstance) {
    auto maybeResult = IV4toCSSColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    EXPECT_THAT(maybeResult.has_value(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result.r, Eq(255));
    EXPECT_THAT(result.g, Eq(255));
    EXPECT_THAT(result.b, Eq(255));
    EXPECT_THAT(result.a, FloatEq(1.0f));
}

TEST(IV4toCSSColor, handlesInvalidValues) {
    auto result = IV4toCSSColor(ImVec4(10.0f, 10.0f, 10.0f, 1.0f));
    EXPECT_THAT(result, Eq(std::nullopt));
}

TEST(IV4toJsonRGBATuple, returnsJsonArray) {
    auto maybeResult = IV4toJsonRGBATuple(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    EXPECT_THAT(maybeResult.has_value(), IsTrue());
    EXPECT_THAT(maybeResult.value().is_array(), IsTrue());

    auto& result = maybeResult.value();

    EXPECT_THAT(result[0].template get<int>(), Eq(255));
    EXPECT_THAT(result[1].template get<int>(), Eq(255));
    EXPECT_THAT(result[2].template get<int>(), Eq(255));
    EXPECT_THAT(result[3].template get<float>(), FloatEq(1.0f));
}

TEST(IV4toJsonRGBATuple, handlesInvalidValues) {
    auto result = IV4toJsonRGBATuple(ImVec4(10.0f, 10.0f, 10.0f, 1.0f));
    EXPECT_THAT(result, Eq(std::nullopt));
}

TEST(IV4toHEXATuple, returnsHEXATuple) {
    auto result = IV4toHEXATuple(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    auto& [hexValue, alphaValue] = result.value();

    EXPECT_THAT(hexValue, StrEq("#ffffff"));
    EXPECT_THAT(alphaValue, FloatEq(1.0f));
}

TEST(IV4toHEXATuple, handlesInvalidValues) {
    auto result = IV4toHEXATuple(ImVec4(10.0f, 10.0f, 10.0f, 1.0f));
    EXPECT_THAT(result, Eq(std::nullopt));
}
