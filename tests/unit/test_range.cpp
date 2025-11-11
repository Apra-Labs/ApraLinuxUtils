/*
 * test_range.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/Range.h"

using namespace apra;

class RangeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test default Range creation
TEST_F(RangeTest, DefaultRangeCreation) {
    Range range;
    SUCCEED();
}

// Test Range creation with parameters
TEST_F(RangeTest, RangeCreationWithParameters) {
    Range range(0, 100);
    EXPECT_EQ(0, range.m_min);
    EXPECT_EQ(100, range.m_max);
    EXPECT_FALSE(range.m_isReversed);
}

// Test Range creation with reversed flag
TEST_F(RangeTest, RangeCreationReversed) {
    Range range(100, 0, true);
    EXPECT_EQ(100, range.m_min);
    EXPECT_EQ(0, range.m_max);
    EXPECT_TRUE(range.m_isReversed);
}

// Test Range assignment operator
TEST_F(RangeTest, RangeAssignment) {
    Range range1(0, 100);
    Range range2;
    range2 = range1;
    EXPECT_EQ(range1.m_min, range2.m_min);
    EXPECT_EQ(range1.m_max, range2.m_max);
    EXPECT_EQ(range1.m_isReversed, range2.m_isReversed);
}

// Test negative ranges
TEST_F(RangeTest, NegativeRange) {
    Range range(-100, -50);
    EXPECT_EQ(-100, range.m_min);
    EXPECT_EQ(-50, range.m_max);
}
