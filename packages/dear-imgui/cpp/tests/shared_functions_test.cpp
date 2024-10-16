#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "shared.h"

using ::testing::IsTrue;
using ::testing::Eq;
using ::testing::FloatEq;

TEST(charPercentageToFloat, handlesPercentages) {
    auto maybeResult = charPercentageToFloat("100%");

    EXPECT_THAT(maybeResult.has_value(), IsTrue());
    EXPECT_THAT(maybeResult.value(), FloatEq(100.0f));
}

TEST(charPercentageToFloat, handlesInvalidValues) {
    auto result = charPercentageToFloat("%");

    EXPECT_THAT(result, Eq(std::nullopt));
}
