#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <nlohmann/json.hpp>

#include "element/element.h"

using json = nlohmann::json;
using ::testing::Eq;

TEST(Element, ctor) {
    Element element(nullptr, 1, true, false, false);
}

TEST(Element, initSetsStyleCorrectly) {
    json styleDef = json::parse(R"(
         {
            "style": {
                "width": 200,
                "height": 250,
                "margin": {
                    "bottom": 10
                },
                "borderBottom": { "thickness": 10 }
            }
        }
    )");

    Element element(nullptr, 1, true, false, false);
    element.Init(styleDef);

    EXPECT_THAT(element.m_layoutNode->GetStyle().margin(facebook::yoga::scopedEnum(YGEdgeBottom)), Eq(facebook::yoga::StyleLength::points(10)));

    EXPECT_THAT(element.m_layoutNode->GetStyle().border(facebook::yoga::scopedEnum(YGEdgeBottom)), Eq(facebook::yoga::StyleLength::points(10)));

    EXPECT_THAT(element.m_layoutNode->GetStyle().dimension(facebook::yoga::scopedEnum(YGDimensionWidth)), Eq(facebook::yoga::StyleLength::points(200)));
    EXPECT_THAT(element.m_layoutNode->GetStyle().dimension(facebook::yoga::scopedEnum(YGDimensionHeight)), Eq(facebook::yoga::StyleLength::points(250)));
}
