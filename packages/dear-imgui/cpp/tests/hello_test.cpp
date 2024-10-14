#include <gtest/gtest.h>

#include "shared.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    EXPECT_EQ(charPercentageToFloat("100%"), 100.0f);
}
