/*
 * test_real_hex_parser.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/RealHexParser.h"
#include <cmath>

using namespace apra;

/**
 * RealHexParser Tests
 *
 * Tests for real number to hexadecimal conversion with fixed precision
 */
class RealHexParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test with different precision values
    }

    void TearDown() override {
    }
};

// Test basic construction
TEST_F(RealHexParserTest, Construction) {
    RealHexParser parser(4);
    // Just verify construction doesn't crash
    SUCCEED();
}

TEST_F(RealHexParserTest, ConstructionWithDifferentPrecisions) {
    RealHexParser parser1(1);
    RealHexParser parser2(8);
    RealHexParser parser3(16);
    SUCCEED();
}

// Test conversion: real to hex to real (round trip)
TEST_F(RealHexParserTest, RoundTripZero) {
    RealHexParser parser(4);
    double original = 0.0;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_DOUBLE_EQ(original, result);
}

TEST_F(RealHexParserTest, RoundTripPositiveInteger) {
    RealHexParser parser(4);
    double original = 10.0;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.1);  // Allow small error due to precision
}

TEST_F(RealHexParserTest, RoundTripNegativeInteger) {
    RealHexParser parser(4);
    double original = -10.0;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.1);
}

TEST_F(RealHexParserTest, RoundTripPositiveDecimal) {
    RealHexParser parser(4);
    double original = 3.1416;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.01);  // Allow error based on precision
}

TEST_F(RealHexParserTest, RoundTripNegativeDecimal) {
    RealHexParser parser(4);
    double original = -3.1416;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.01);
}

TEST_F(RealHexParserTest, RoundTripSmallNumber) {
    RealHexParser parser(4);
    double original = 0.0625;  // 1/16
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.01);
}

TEST_F(RealHexParserTest, RoundTripLargeNumber) {
    RealHexParser parser(4);
    double original = 1000.5;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 1.0);  // Larger error for large numbers
}

// Test different precisions
TEST_F(RealHexParserTest, Precision1Digit) {
    RealHexParser parser(1);
    double original = 5.5;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.5);  // Low precision = higher error
}

TEST_F(RealHexParserTest, Precision8Digits) {
    RealHexParser parser(8);
    double original = 3.14159265;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.0001);  // High precision = lower error
}

TEST_F(RealHexParserTest, Precision16Digits) {
    RealHexParser parser(16);
    double original = 2.718281828;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.00001);  // Very high precision
}

// Test specific values
TEST_F(RealHexParserTest, ConvertOne) {
    RealHexParser parser(4);
    double original = 1.0;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.1);
}

TEST_F(RealHexParserTest, ConvertNegativeOne) {
    RealHexParser parser(4);
    double original = -1.0;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.1);
}

TEST_F(RealHexParserTest, ConvertHalf) {
    RealHexParser parser(4);
    double original = 0.5;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.1);
}

TEST_F(RealHexParserTest, ConvertQuarter) {
    RealHexParser parser(4);
    double original = 0.25;
    uint32_t hex = parser.toHex(original);
    double result = parser.toReal(hex);
    EXPECT_NEAR(original, result, 0.05);
}

// Test fractional values
TEST_F(RealHexParserTest, FractionalValues) {
    RealHexParser parser(4);

    std::vector<double> testValues = {
        0.125, 0.375, 0.625, 0.875,
        1.125, 1.375, 1.625, 1.875
    };

    for (double original : testValues) {
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 0.1) << "Failed for value: " << original;
    }
}

// Test negative values
TEST_F(RealHexParserTest, NegativeValues) {
    RealHexParser parser(4);

    std::vector<double> testValues = {
        -0.125, -0.5, -1.0, -2.5, -10.0, -100.0
    };

    for (double original : testValues) {
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 0.5) << "Failed for value: " << original;
    }
}

// Test positive values
TEST_F(RealHexParserTest, PositiveValues) {
    RealHexParser parser(4);

    std::vector<double> testValues = {
        0.125, 0.5, 1.0, 2.5, 10.0, 100.0
    };

    for (double original : testValues) {
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 0.5) << "Failed for value: " << original;
    }
}

// Test mathematical constants
TEST_F(RealHexParserTest, MathematicalConstants) {
    RealHexParser parser(8);

    // Pi
    double pi = 3.14159265359;
    uint32_t hexPi = parser.toHex(pi);
    double resultPi = parser.toReal(hexPi);
    EXPECT_NEAR(pi, resultPi, 0.001);

    // E (Euler's number)
    double e = 2.71828182846;
    uint32_t hexE = parser.toHex(e);
    double resultE = parser.toReal(hexE);
    EXPECT_NEAR(e, resultE, 0.001);
}

// Test temperature conversions (common use case)
TEST_F(RealHexParserTest, TemperatureValues) {
    RealHexParser parser(4);

    std::vector<double> temperatures = {
        -40.0, -20.0, 0.0, 20.0, 25.0, 37.0, 100.0
    };

    for (double temp : temperatures) {
        uint32_t hex = parser.toHex(temp);
        double result = parser.toReal(hex);
        EXPECT_NEAR(temp, result, 1.0) << "Failed for temperature: " << temp;
    }
}

// Test sensor reading values (0-100 range)
TEST_F(RealHexParserTest, SensorReadings) {
    RealHexParser parser(4);

    for (int i = 0; i <= 100; i += 10) {
        double original = static_cast<double>(i);
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 1.0) << "Failed for sensor reading: " << i;
    }
}

// Test very small numbers
TEST_F(RealHexParserTest, VerySmallNumbers) {
    RealHexParser parser(8);

    std::vector<double> smallNumbers = {
        0.001, 0.01, 0.1
    };

    for (double original : smallNumbers) {
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 0.01) << "Failed for small number: " << original;
    }
}

// Test range of values with different precisions
TEST_F(RealHexParserTest, RangeTestPrecision4) {
    RealHexParser parser(4);

    for (double val = -10.0; val <= 10.0; val += 0.5) {
        uint32_t hex = parser.toHex(val);
        double result = parser.toReal(hex);
        EXPECT_NEAR(val, result, 0.1) << "Failed for value: " << val;
    }
}

TEST_F(RealHexParserTest, RangeTestPrecision8) {
    RealHexParser parser(8);

    for (double val = -5.0; val <= 5.0; val += 0.25) {
        uint32_t hex = parser.toHex(val);
        double result = parser.toReal(hex);
        EXPECT_NEAR(val, result, 0.01) << "Failed for value: " << val;
    }
}

// Test conversion consistency
TEST_F(RealHexParserTest, ConversionConsistency) {
    RealHexParser parser(4);
    double original = 42.42;

    // Convert multiple times, should get same result
    uint32_t hex1 = parser.toHex(original);
    uint32_t hex2 = parser.toHex(original);
    EXPECT_EQ(hex1, hex2);

    double result1 = parser.toReal(hex1);
    double result2 = parser.toReal(hex2);
    EXPECT_DOUBLE_EQ(result1, result2);
}

// Test different parser instances with same precision
TEST_F(RealHexParserTest, MultipleParsersSamePrecision) {
    RealHexParser parser1(4);
    RealHexParser parser2(4);

    double original = 12.34;

    uint32_t hex1 = parser1.toHex(original);
    uint32_t hex2 = parser2.toHex(original);

    // Same precision should give same hex value
    EXPECT_EQ(hex1, hex2);

    double result1 = parser1.toReal(hex1);
    double result2 = parser2.toReal(hex2);

    EXPECT_DOUBLE_EQ(result1, result2);
}

// Test edge values
TEST_F(RealHexParserTest, EdgeValues) {
    RealHexParser parser(4);

    // Test values near zero
    std::vector<double> edgeValues = {
        -0.001, -0.0001, 0.0001, 0.001
    };

    for (double original : edgeValues) {
        uint32_t hex = parser.toHex(original);
        double result = parser.toReal(hex);
        EXPECT_NEAR(original, result, 0.01) << "Failed for edge value: " << original;
    }
}
