#include <gtest/gtest.h>

#include "shared.h"

// Demonstrate some basic assertions.
TEST(SharedFunctionsTest, charPercentageToFloat) {
    EXPECT_EQ(charPercentageToFloat("100%"), 100.0f);
}
