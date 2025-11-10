/*
 * test_pwm.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/PWM.h"
#include "mocks/MockHardware.h"

using namespace apra;
using namespace apra::testing;

/**
 * PWM Tests using MockPWM
 *
 * Note: Real PWM class requires hardware access (/sys/class/pwm).
 * These tests verify the mock infrastructure for PWM simulation.
 */
class PWMTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockPwm = new MockPWM();
    }

    void TearDown() override {
        delete mockPwm;
    }

    MockPWM* mockPwm;
};

// Test PWM export state
TEST_F(PWMTest, DefaultNotExported) {
    EXPECT_FALSE(mockPwm->isExported());
}

TEST_F(PWMTest, SetExported) {
    mockPwm->setExported(true);
    EXPECT_TRUE(mockPwm->isExported());
}

TEST_F(PWMTest, UnExport) {
    mockPwm->setExported(true);
    EXPECT_TRUE(mockPwm->isExported());

    mockPwm->setExported(false);
    EXPECT_FALSE(mockPwm->isExported());
}

// Test PWM enabled state
TEST_F(PWMTest, DefaultNotEnabled) {
    EXPECT_FALSE(mockPwm->isEnabled());
}

TEST_F(PWMTest, EnablePWM) {
    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());
}

TEST_F(PWMTest, DisablePWM) {
    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());

    mockPwm->setEnabled(false);
    EXPECT_FALSE(mockPwm->isEnabled());
}

TEST_F(PWMTest, ToggleEnabled) {
    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());

    mockPwm->setEnabled(false);
    EXPECT_FALSE(mockPwm->isEnabled());

    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());
}

// Test PWM period
TEST_F(PWMTest, DefaultPeriod) {
    EXPECT_EQ(0ULL, mockPwm->getPeriod());
}

TEST_F(PWMTest, SetPeriod) {
    uint64_t period = 1000000;  // 1ms in nanoseconds
    mockPwm->setPeriod(period);
    EXPECT_EQ(period, mockPwm->getPeriod());
}

TEST_F(PWMTest, SetVariousPeriods) {
    // 1 kHz (1ms period)
    mockPwm->setPeriod(1000000);
    EXPECT_EQ(1000000ULL, mockPwm->getPeriod());

    // 10 kHz (100us period)
    mockPwm->setPeriod(100000);
    EXPECT_EQ(100000ULL, mockPwm->getPeriod());

    // 50 Hz (20ms period)
    mockPwm->setPeriod(20000000);
    EXPECT_EQ(20000000ULL, mockPwm->getPeriod());
}

// Test PWM duty cycle
TEST_F(PWMTest, DefaultDutyCycle) {
    EXPECT_EQ(0ULL, mockPwm->getDutyCycle());
}

TEST_F(PWMTest, SetDutyCycle) {
    uint64_t dutyCycle = 500000;  // 0.5ms in nanoseconds
    mockPwm->setDutyCycle(dutyCycle);
    EXPECT_EQ(dutyCycle, mockPwm->getDutyCycle());
}

TEST_F(PWMTest, SetVariousDutyCycles) {
    // 25% duty cycle (250us for 1ms period)
    mockPwm->setDutyCycle(250000);
    EXPECT_EQ(250000ULL, mockPwm->getDutyCycle());

    // 50% duty cycle
    mockPwm->setDutyCycle(500000);
    EXPECT_EQ(500000ULL, mockPwm->getDutyCycle());

    // 75% duty cycle
    mockPwm->setDutyCycle(750000);
    EXPECT_EQ(750000ULL, mockPwm->getDutyCycle());
}

// Test PWM duty cycle percentage calculation
TEST_F(PWMTest, DutyCyclePercentZeroPeriod) {
    mockPwm->setPeriod(0);
    mockPwm->setDutyCycle(500000);
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercent50) {
    mockPwm->setPeriod(1000000);   // 1ms
    mockPwm->setDutyCycle(500000);  // 0.5ms
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercent25) {
    mockPwm->setPeriod(1000000);   // 1ms
    mockPwm->setDutyCycle(250000);  // 0.25ms
    EXPECT_DOUBLE_EQ(25.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercent75) {
    mockPwm->setPeriod(1000000);   // 1ms
    mockPwm->setDutyCycle(750000);  // 0.75ms
    EXPECT_DOUBLE_EQ(75.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercent100) {
    mockPwm->setPeriod(1000000);    // 1ms
    mockPwm->setDutyCycle(1000000);  // 1ms (100%)
    EXPECT_DOUBLE_EQ(100.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercent0) {
    mockPwm->setPeriod(1000000);  // 1ms
    mockPwm->setDutyCycle(0);     // 0ms (0%)
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getDutyCyclePercent());
}

TEST_F(PWMTest, DutyCyclePercentVariousPeriods) {
    // Test with different period values
    mockPwm->setPeriod(20000000);   // 20ms (50Hz)
    mockPwm->setDutyCycle(1500000);  // 1.5ms (7.5%)
    EXPECT_DOUBLE_EQ(7.5, mockPwm->getDutyCyclePercent());

    mockPwm->setPeriod(100000);    // 100us (10kHz)
    mockPwm->setDutyCycle(30000);  // 30us (30%)
    EXPECT_DOUBLE_EQ(30.0, mockPwm->getDutyCyclePercent());
}

// Test PWM frequency
TEST_F(PWMTest, DefaultFrequency) {
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getFrequency());
}

TEST_F(PWMTest, SetFrequency) {
    mockPwm->setFrequency(1000.0);  // 1 kHz
    EXPECT_DOUBLE_EQ(1000.0, mockPwm->getFrequency());
}

TEST_F(PWMTest, SetVariousFrequencies) {
    mockPwm->setFrequency(50.0);    // 50 Hz
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getFrequency());

    mockPwm->setFrequency(1000.0);  // 1 kHz
    EXPECT_DOUBLE_EQ(1000.0, mockPwm->getFrequency());

    mockPwm->setFrequency(10000.0); // 10 kHz
    EXPECT_DOUBLE_EQ(10000.0, mockPwm->getFrequency());
}

// Test complete PWM lifecycle
TEST_F(PWMTest, CompleteLifecycle) {
    // Initial state
    EXPECT_FALSE(mockPwm->isExported());
    EXPECT_FALSE(mockPwm->isEnabled());
    EXPECT_EQ(0ULL, mockPwm->getPeriod());
    EXPECT_EQ(0ULL, mockPwm->getDutyCycle());

    // Export and configure
    mockPwm->setExported(true);
    mockPwm->setPeriod(1000000);   // 1ms (1kHz)
    mockPwm->setDutyCycle(500000);  // 50% duty cycle
    mockPwm->setFrequency(1000.0);  // 1kHz

    EXPECT_TRUE(mockPwm->isExported());
    EXPECT_EQ(1000000ULL, mockPwm->getPeriod());
    EXPECT_EQ(500000ULL, mockPwm->getDutyCycle());
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());
    EXPECT_DOUBLE_EQ(1000.0, mockPwm->getFrequency());

    // Enable PWM
    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());

    // Update duty cycle while running
    mockPwm->setDutyCycle(750000);  // 75%
    EXPECT_EQ(750000ULL, mockPwm->getDutyCycle());
    EXPECT_DOUBLE_EQ(75.0, mockPwm->getDutyCyclePercent());

    // Disable and unexport
    mockPwm->setEnabled(false);
    mockPwm->setExported(false);
    EXPECT_FALSE(mockPwm->isEnabled());
    EXPECT_FALSE(mockPwm->isExported());
}

// Test PWM for servo control (typical 50Hz with 1-2ms pulse)
TEST_F(PWMTest, ServoControl) {
    mockPwm->setExported(true);
    mockPwm->setFrequency(50.0);      // 50 Hz
    mockPwm->setPeriod(20000000);     // 20ms period

    // Servo at 0 degrees (1ms pulse)
    mockPwm->setDutyCycle(1000000);
    EXPECT_EQ(1000000ULL, mockPwm->getDutyCycle());
    EXPECT_DOUBLE_EQ(5.0, mockPwm->getDutyCyclePercent());

    // Servo at 90 degrees (1.5ms pulse)
    mockPwm->setDutyCycle(1500000);
    EXPECT_EQ(1500000ULL, mockPwm->getDutyCycle());
    EXPECT_DOUBLE_EQ(7.5, mockPwm->getDutyCyclePercent());

    // Servo at 180 degrees (2ms pulse)
    mockPwm->setDutyCycle(2000000);
    EXPECT_EQ(2000000ULL, mockPwm->getDutyCycle());
    EXPECT_DOUBLE_EQ(10.0, mockPwm->getDutyCyclePercent());
}

// Test PWM for LED dimming (high frequency)
TEST_F(PWMTest, LEDDimming) {
    mockPwm->setExported(true);
    mockPwm->setFrequency(1000.0);  // 1 kHz (flicker-free)
    mockPwm->setPeriod(1000000);    // 1ms period

    // LED off (0%)
    mockPwm->setDutyCycle(0);
    mockPwm->setEnabled(true);
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getDutyCyclePercent());

    // LED dim (25%)
    mockPwm->setDutyCycle(250000);
    EXPECT_DOUBLE_EQ(25.0, mockPwm->getDutyCyclePercent());

    // LED medium (50%)
    mockPwm->setDutyCycle(500000);
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());

    // LED bright (75%)
    mockPwm->setDutyCycle(750000);
    EXPECT_DOUBLE_EQ(75.0, mockPwm->getDutyCyclePercent());

    // LED full (100%)
    mockPwm->setDutyCycle(1000000);
    EXPECT_DOUBLE_EQ(100.0, mockPwm->getDutyCyclePercent());
}

// Test PWM for motor speed control
TEST_F(PWMTest, MotorSpeedControl) {
    mockPwm->setExported(true);
    mockPwm->setFrequency(10000.0);  // 10 kHz
    mockPwm->setPeriod(100000);      // 100us period
    mockPwm->setEnabled(true);

    // Motor stopped
    mockPwm->setDutyCycle(0);
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getDutyCyclePercent());

    // Motor at 33% speed
    mockPwm->setDutyCycle(33000);
    EXPECT_NEAR(33.0, mockPwm->getDutyCyclePercent(), 0.1);

    // Motor at 66% speed
    mockPwm->setDutyCycle(66000);
    EXPECT_NEAR(66.0, mockPwm->getDutyCyclePercent(), 0.1);

    // Motor at full speed
    mockPwm->setDutyCycle(100000);
    EXPECT_DOUBLE_EQ(100.0, mockPwm->getDutyCyclePercent());
}

// Test edge cases
TEST_F(PWMTest, EdgeCases) {
    // Very small period
    mockPwm->setPeriod(1);
    mockPwm->setDutyCycle(0);
    EXPECT_DOUBLE_EQ(0.0, mockPwm->getDutyCyclePercent());

    // Very large period (1 second)
    mockPwm->setPeriod(1000000000);
    mockPwm->setDutyCycle(500000000);
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());

    // Maximum uint64_t values
    uint64_t maxVal = 0xFFFFFFFFFFFFFFFFULL;
    mockPwm->setPeriod(maxVal);
    mockPwm->setDutyCycle(maxVal / 2);
    EXPECT_NEAR(50.0, mockPwm->getDutyCyclePercent(), 0.1);
}

// Test state transitions
TEST_F(PWMTest, StateTransitions) {
    // Can't enable without export
    EXPECT_FALSE(mockPwm->isExported());
    // In real hardware, this would fail, but mock allows it
    mockPwm->setEnabled(true);
    EXPECT_TRUE(mockPwm->isEnabled());

    // Proper sequence
    mockPwm->setEnabled(false);
    mockPwm->setExported(true);
    mockPwm->setPeriod(1000000);
    mockPwm->setDutyCycle(500000);
    mockPwm->setEnabled(true);

    EXPECT_TRUE(mockPwm->isExported());
    EXPECT_TRUE(mockPwm->isEnabled());

    // Disable before unexport
    mockPwm->setEnabled(false);
    mockPwm->setExported(false);

    EXPECT_FALSE(mockPwm->isEnabled());
    EXPECT_FALSE(mockPwm->isExported());
}

// Test duty cycle updates while running
TEST_F(PWMTest, DynamicDutyCycleUpdate) {
    mockPwm->setExported(true);
    mockPwm->setPeriod(1000000);
    mockPwm->setEnabled(true);

    // Update duty cycle multiple times while running
    for (int i = 0; i <= 100; i += 10) {
        uint64_t dutyCycle = (mockPwm->getPeriod() * i) / 100;
        mockPwm->setDutyCycle(dutyCycle);
        EXPECT_NEAR(static_cast<double>(i), mockPwm->getDutyCyclePercent(), 0.1);
    }

    EXPECT_TRUE(mockPwm->isEnabled());
}

// Test period and duty cycle relationship
TEST_F(PWMTest, PeriodDutyCycleRelationship) {
    mockPwm->setPeriod(1000000);
    mockPwm->setDutyCycle(500000);
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());

    // Change period, duty cycle percentage should change
    mockPwm->setPeriod(2000000);  // Double the period
    // Duty cycle value stays the same, but percentage halves
    EXPECT_DOUBLE_EQ(25.0, mockPwm->getDutyCyclePercent());

    // Adjust duty cycle to maintain percentage
    mockPwm->setDutyCycle(1000000);  // Double the duty cycle
    EXPECT_DOUBLE_EQ(50.0, mockPwm->getDutyCyclePercent());
}
