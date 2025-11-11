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

/**
 * GPIO Tests using MockGPIO
 *
 * Note: Real GPIO class requires hardware access (/sys/class/gpio).
 * These tests verify the mock infrastructure for GPIO simulation.
 */
class GPIOTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockGpio = new MockGPIO();
    }

    void TearDown() override {
        delete mockGpio;
    }

    MockGPIO* mockGpio;
};

// Test GPIO export state
TEST_F(GPIOTest, DefaultNotExported) {
    EXPECT_FALSE(mockGpio->isExported());
}

TEST_F(GPIOTest, SetExported) {
    mockGpio->setExported(true);
    EXPECT_TRUE(mockGpio->isExported());
}

TEST_F(GPIOTest, UnExport) {
    mockGpio->setExported(true);
    EXPECT_TRUE(mockGpio->isExported());

    mockGpio->setExported(false);
    EXPECT_FALSE(mockGpio->isExported());
}

// Test GPIO open state
TEST_F(GPIOTest, DefaultNotOpen) {
    EXPECT_FALSE(mockGpio->isOpen());
}

TEST_F(GPIOTest, OpenGPIO) {
    mockGpio->setOpen(true);
    EXPECT_TRUE(mockGpio->isOpen());
}

TEST_F(GPIOTest, CloseGPIO) {
    mockGpio->setOpen(true);
    EXPECT_TRUE(mockGpio->isOpen());

    mockGpio->setOpen(false);
    EXPECT_FALSE(mockGpio->isOpen());
}

// Test GPIO direction
TEST_F(GPIOTest, DefaultDirection) {
    EXPECT_FALSE(mockGpio->isReadDirection());
}

TEST_F(GPIOTest, SetReadDirection) {
    mockGpio->setDirection(true);
    EXPECT_TRUE(mockGpio->isReadDirection());
}

TEST_F(GPIOTest, SetWriteDirection) {
    mockGpio->setDirection(false);
    EXPECT_FALSE(mockGpio->isReadDirection());
}

TEST_F(GPIOTest, ToggleDirection) {
    mockGpio->setDirection(true);
    EXPECT_TRUE(mockGpio->isReadDirection());

    mockGpio->setDirection(false);
    EXPECT_FALSE(mockGpio->isReadDirection());

    mockGpio->setDirection(true);
    EXPECT_TRUE(mockGpio->isReadDirection());
}

// Test GPIO value
TEST_F(GPIOTest, DefaultValue) {
    EXPECT_EQ(0, mockGpio->getValue());
}

TEST_F(GPIOTest, SetValueHigh) {
    mockGpio->setValue(1);
    EXPECT_EQ(1, mockGpio->getValue());
}

TEST_F(GPIOTest, SetValueLow) {
    mockGpio->setValue(0);
    EXPECT_EQ(0, mockGpio->getValue());
}

TEST_F(GPIOTest, ToggleValue) {
    mockGpio->setValue(0);
    EXPECT_EQ(0, mockGpio->getValue());

    mockGpio->setValue(1);
    EXPECT_EQ(1, mockGpio->getValue());

    mockGpio->setValue(0);
    EXPECT_EQ(0, mockGpio->getValue());
}

TEST_F(GPIOTest, ArbitraryValues) {
    // Test that mock can hold arbitrary values
    mockGpio->setValue(42);
    EXPECT_EQ(42, mockGpio->getValue());

    mockGpio->setValue(-1);
    EXPECT_EQ(-1, mockGpio->getValue());
}

// Test GPIO edge detection
TEST_F(GPIOTest, DefaultEdge) {
    EXPECT_EQ(0, mockGpio->getEdge());
}

TEST_F(GPIOTest, SetEdgeRising) {
    mockGpio->setEdge(1);  // RISING
    EXPECT_EQ(1, mockGpio->getEdge());
}

TEST_F(GPIOTest, SetEdgeFalling) {
    mockGpio->setEdge(2);  // FALLING
    EXPECT_EQ(2, mockGpio->getEdge());
}

TEST_F(GPIOTest, SetEdgeBoth) {
    mockGpio->setEdge(3);  // BOTH
    EXPECT_EQ(3, mockGpio->getEdge());
}

TEST_F(GPIOTest, SetEdgeNone) {
    mockGpio->setEdge(1);
    EXPECT_EQ(1, mockGpio->getEdge());

    mockGpio->setEdge(0);  // NONE
    EXPECT_EQ(0, mockGpio->getEdge());
}

// Test complete GPIO lifecycle
TEST_F(GPIOTest, CompleteLifecycle) {
    // Initial state
    EXPECT_FALSE(mockGpio->isExported());
    EXPECT_FALSE(mockGpio->isOpen());
    EXPECT_EQ(0, mockGpio->getValue());

    // Export and configure
    mockGpio->setExported(true);
    mockGpio->setDirection(false);  // Output
    EXPECT_TRUE(mockGpio->isExported());
    EXPECT_FALSE(mockGpio->isReadDirection());

    // Open and use
    mockGpio->setOpen(true);
    mockGpio->setValue(1);
    EXPECT_TRUE(mockGpio->isOpen());
    EXPECT_EQ(1, mockGpio->getValue());

    // Close and unexport
    mockGpio->setOpen(false);
    mockGpio->setExported(false);
    EXPECT_FALSE(mockGpio->isOpen());
    EXPECT_FALSE(mockGpio->isExported());
}

// Test GPIO as input with edge detection
TEST_F(GPIOTest, InputWithInterrupt) {
    // Configure as input
    mockGpio->setExported(true);
    mockGpio->setDirection(true);  // Read
    mockGpio->setEdge(3);  // Both edges
    mockGpio->setOpen(true);

    EXPECT_TRUE(mockGpio->isExported());
    EXPECT_TRUE(mockGpio->isReadDirection());
    EXPECT_EQ(3, mockGpio->getEdge());
    EXPECT_TRUE(mockGpio->isOpen());

    // Simulate input changes
    mockGpio->setValue(0);
    EXPECT_EQ(0, mockGpio->getValue());

    mockGpio->setValue(1);
    EXPECT_EQ(1, mockGpio->getValue());
}

// Test GPIO as output
TEST_F(GPIOTest, OutputMode) {
    // Configure as output
    mockGpio->setExported(true);
    mockGpio->setDirection(false);  // Write
    mockGpio->setOpen(true);

    EXPECT_TRUE(mockGpio->isExported());
    EXPECT_FALSE(mockGpio->isReadDirection());
    EXPECT_TRUE(mockGpio->isOpen());

    // Write values
    mockGpio->setValue(1);
    EXPECT_EQ(1, mockGpio->getValue());

    mockGpio->setValue(0);
    EXPECT_EQ(0, mockGpio->getValue());
}
