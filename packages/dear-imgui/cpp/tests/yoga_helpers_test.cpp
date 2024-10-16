#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <yoga/YGEnums.h>

#include "yoga_helpers.h"

using ::testing::FloatEq;
using ::testing::Eq;
using ::testing::Optional;
using ::testing::IsTrue;
using ::testing::StrEq;

TEST(ResolveAlignItems, handlesAuto) {
    EXPECT_THAT(ResolveAlignItems("auto").value(), Eq(YGAlignAuto));
}

TEST(ResolveAlignItems, handlesFlexStart) {
    EXPECT_THAT(ResolveAlignItems("flex-start").value(), Eq(YGAlignFlexStart));
}

TEST(ResolveAlignItems, handlesCenter) {
    EXPECT_THAT(ResolveAlignItems("center").value(), Eq(YGAlignCenter));
}

TEST(ResolveAlignItems, handlesFlexEnd) {
    EXPECT_THAT(ResolveAlignItems("flex-end").value(), Eq(YGAlignFlexEnd));
}

TEST(ResolveAlignItems, handlesStretch) {
    EXPECT_THAT(ResolveAlignItems("stretch").value(), Eq(YGAlignStretch));
}

TEST(ResolveAlignItems, handlesBaseline) {
    EXPECT_THAT(ResolveAlignItems("baseline").value(), Eq(YGAlignBaseline));
}

TEST(ResolveAlignItems, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveAlignItems("value"), Eq(std::nullopt));
}

// --

TEST(ResolveAlignContent, handlesAuto) {
    EXPECT_THAT(ResolveAlignContent("auto").value(), Eq(YGAlignAuto));
}

TEST(ResolveAlignContent, handlesFlexStart) {
    EXPECT_THAT(ResolveAlignContent("flex-start").value(), Eq(YGAlignFlexStart));
}

TEST(ResolveAlignContent, handlesCenter) {
    EXPECT_THAT(ResolveAlignContent("center").value(), Eq(YGAlignCenter));
}

TEST(ResolveAlignContent, handlesFlexEnd) {
    EXPECT_THAT(ResolveAlignContent("flex-end").value(), Eq(YGAlignFlexEnd));
}

TEST(ResolveAlignContent, handlesStretch) {
    EXPECT_THAT(ResolveAlignContent("stretch").value(), Eq(YGAlignStretch));
}

TEST(ResolveAlignContent, handlesSpaceBetween) {
    EXPECT_THAT(ResolveAlignContent("space-between").value(), Eq(YGAlignSpaceBetween));
}

TEST(ResolveAlignContent, handlesSpaceAround) {
    EXPECT_THAT(ResolveAlignContent("space-around").value(), Eq(YGAlignSpaceAround));
}

TEST(ResolveAlignContent, handlesSpaceEvenly) {
    EXPECT_THAT(ResolveAlignContent("space-evenly").value(), Eq(YGAlignSpaceEvenly));
}

TEST(ResolveAlignContent, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveAlignContent("value"), Eq(std::nullopt));
}

// --

TEST(ResolveJustifyContent, handlesFlexStart) {
    EXPECT_THAT(ResolveJustifyContent("flex-start").value(), Eq(YGJustifyFlexStart));
}

TEST(ResolveJustifyContent, handlesCenter) {
    EXPECT_THAT(ResolveJustifyContent("center").value(), Eq(YGJustifyCenter));
}

TEST(ResolveJustifyContent, handlesFlexEnd) {
    EXPECT_THAT(ResolveJustifyContent("flex-end").value(), Eq(YGJustifyFlexEnd));
}

TEST(ResolveJustifyContent, handlesSpaceBetween) {
    EXPECT_THAT(ResolveJustifyContent("space-between").value(), Eq(YGJustifySpaceBetween));
}

TEST(ResolveJustifyContent, handlesSpaceAround) {
    EXPECT_THAT(ResolveJustifyContent("space-around").value(), Eq(YGJustifySpaceAround));
}

TEST(ResolveJustifyContent, handlesSpaceEvenly) {
    EXPECT_THAT(ResolveJustifyContent("space-evenly").value(), Eq(YGJustifySpaceEvenly));
}

TEST(ResolveJustifyContent, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveJustifyContent("value"), Eq(std::nullopt));
}

// --

TEST(ResolveFlexDirection, handlesColumn) {
    EXPECT_THAT(ResolveFlexDirection("column").value(), Eq(YGFlexDirectionColumn));
}

TEST(ResolveFlexDirection, handlesColumnReverse) {
    EXPECT_THAT(ResolveFlexDirection("column-reverse").value(), Eq(YGFlexDirectionColumnReverse));
}

TEST(ResolveFlexDirection, handlesRow) {
    EXPECT_THAT(ResolveFlexDirection("row").value(), Eq(YGFlexDirectionRow));
}

TEST(ResolveFlexDirection, handlesRowReverse) {
    EXPECT_THAT(ResolveFlexDirection("row-reverse").value(), Eq(YGFlexDirectionRowReverse));
}

TEST(ResolveFlexDirection, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveFlexDirection("value"), Eq(std::nullopt));
}

// --

TEST(ResolveDirection, handlesInherit) {
    EXPECT_THAT(ResolveDirection("inherit").value(), Eq(YGDirectionInherit));
}

TEST(ResolveDirection, handlesLtr) {
    EXPECT_THAT(ResolveDirection("ltr").value(), Eq(YGDirectionLTR));
}

TEST(ResolveDirection, handlesRtl) {
    EXPECT_THAT(ResolveDirection("rtl").value(), Eq(YGDirectionRTL));
}

TEST(ResolveDirection, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveDirection("value"), Eq(std::nullopt));
}

// --

TEST(ResolvePositionType, handlesStatic) {
    EXPECT_THAT(ResolvePositionType("static").value(), Eq(YGPositionTypeStatic));
}

TEST(ResolvePositionType, handlesRelative) {
    EXPECT_THAT(ResolvePositionType("relative").value(), Eq(YGPositionTypeRelative));
}

TEST(ResolvePositionType, handlesAbsolute) {
    EXPECT_THAT(ResolvePositionType("absolute").value(), Eq(YGPositionTypeAbsolute));
}

TEST(ResolvePositionType, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolvePositionType("value"), Eq(std::nullopt));
}

// --

TEST(ResolveFlexWrap, handlesNoWrap) {
    EXPECT_THAT(ResolveFlexWrap("no-wrap").value(), Eq(YGWrapNoWrap));
}

TEST(ResolveFlexWrap, handlesWrap) {
    EXPECT_THAT(ResolveFlexWrap("wrap").value(), Eq(YGWrapWrap));
}

TEST(ResolveFlexWrap, handlesWrapReverse) {
    EXPECT_THAT(ResolveFlexWrap("wrap-reverse").value(), Eq(YGWrapWrapReverse));
}

TEST(ResolveFlexWrap, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveFlexWrap("value"), Eq(std::nullopt));
}

// --

TEST(ResolveOverflow, handlesVisible) {
    EXPECT_THAT(ResolveOverflow("visible").value(), Eq(YGOverflowVisible));
}

TEST(ResolveOverflow, handlesHidden) {
    EXPECT_THAT(ResolveOverflow("hidden").value(), Eq(YGOverflowHidden));
}

TEST(ResolveOverflow, handlesScroll) {
    EXPECT_THAT(ResolveOverflow("scroll").value(), Eq(YGOverflowScroll));
}

TEST(ResolveOverflow, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveOverflow("value"), Eq(std::nullopt));
}

// --

TEST(ResolveDisplay, handlesFlex) {
    EXPECT_THAT(ResolveDisplay("flex").value(), Eq(YGDisplayFlex));
}

TEST(ResolveDisplay, handlesHidden) {
    EXPECT_THAT(ResolveDisplay("none").value(), Eq(YGDisplayNone));
}

TEST(ResolveDisplay, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveDisplay("value"), Eq(std::nullopt));
}

// --

TEST(ResolveEdge, handlesLeft) {
    EXPECT_THAT(ResolveEdge("left").value(), Eq(YGEdgeLeft));
}

TEST(ResolveEdge, handlesTop) {
    EXPECT_THAT(ResolveEdge("top").value(), Eq(YGEdgeTop));
}

TEST(ResolveEdge, handlesRight) {
    EXPECT_THAT(ResolveEdge("right").value(), Eq(YGEdgeRight));
}

TEST(ResolveEdge, handlesbottom) {
    EXPECT_THAT(ResolveEdge("bottom").value(), Eq(YGEdgeBottom));
}

TEST(ResolveEdge, handlesStart) {
    EXPECT_THAT(ResolveEdge("start").value(), Eq(YGEdgeStart));
}

TEST(ResolveEdge, handlesEnd) {
    EXPECT_THAT(ResolveEdge("end").value(), Eq(YGEdgeEnd));
}

TEST(ResolveEdge, handlesHorizontal) {
    EXPECT_THAT(ResolveEdge("horizontal").value(), Eq(YGEdgeHorizontal));
}

TEST(ResolveEdge, handlesVertical) {
    EXPECT_THAT(ResolveEdge("vertical").value(), Eq(YGEdgeVertical));
}

TEST(ResolveEdge, handlesAll) {
    EXPECT_THAT(ResolveEdge("all").value(), Eq(YGEdgeAll));
}

TEST(ResolveEdge, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveEdge("value"), Eq(std::nullopt));
}

// --

TEST(ResolveGutter, handlesColumn) {
    EXPECT_THAT(ResolveGutter("column").value(), Eq(YGGutterColumn));
}

TEST(ResolveGutter, handlesRow) {
    EXPECT_THAT(ResolveGutter("row").value(), Eq(YGGutterRow));
}

TEST(ResolveGutter, handlesAll) {
    EXPECT_THAT(ResolveGutter("all").value(), Eq(YGGutterAll));
}

TEST(ResolveGutter, handlesUnrecognisedValues) {
    EXPECT_THAT(ResolveGutter("value"), Eq(std::nullopt));
}
