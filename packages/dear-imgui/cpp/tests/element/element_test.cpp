#include <gtest/gtest.h>
#include <gmock/gmock.h>

// #include <yoga/enums/YogaEnums.h>
// #include <yoga/enums/Direction.h>
// #include <yoga/node/Node.h>
#include <nlohmann/json.hpp>
// #include <imgui_internal.h>

#include "element/element.h"

// using json = nlohmann::json;
// using ::testing::Eq;

TEST(Element, ctor) {
    Element element(nullptr, 1, true, false, false);
}
