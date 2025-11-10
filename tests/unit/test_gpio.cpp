/*
 * test_gpio.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/GPIO.h"
#include "mocks/MockHardware.h"

using namespace apra;
using namespace apra::testing;

class GPIOTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Basic test to verify GPIO class structure
TEST_F(GPIOTest, BasicGPIOCreation) {
    // This is a placeholder test
    // GPIO requires hardware access, so we'll test with mocks
    MockGPIO mockGpio;
    mockGpio.setExported(true);
    EXPECT_TRUE(mockGpio.isExported());
}

TEST_F(GPIOTest, MockGPIOSetValue) {
    MockGPIO mockGpio;
    mockGpio.setValue(1);
    EXPECT_EQ(1, mockGpio.getValue());

    mockGpio.setValue(0);
    EXPECT_EQ(0, mockGpio.getValue());
}

TEST_F(GPIOTest, MockGPIODirection) {
    MockGPIO mockGpio;
    mockGpio.setDirection(true);  // Read direction
    EXPECT_TRUE(mockGpio.isReadDirection());

    mockGpio.setDirection(false);  // Write direction
    EXPECT_FALSE(mockGpio.isReadDirection());
}
