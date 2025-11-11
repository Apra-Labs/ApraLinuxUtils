/*
 * test_i2c_error.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/I2CError.h"

using namespace apra;

class I2CErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test default I2CError creation
TEST_F(I2CErrorTest, DefaultErrorCreation) {
    I2CError error;
    EXPECT_FALSE(error.isError());
    EXPECT_EQ(NO_ERROR, error.getCode());
    EXPECT_EQ("", error.getMessage());
    EXPECT_EQ("", error.getDebugMessage());
}

// Test I2CError creation with message only
TEST_F(I2CErrorTest, ErrorCreationWithMessage) {
    I2CError error("I2C communication failed");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(NO_ERROR, error.getCode());
    EXPECT_EQ("I2C communication failed", error.getMessage());
}

// Test I2CError creation with message and code
TEST_F(I2CErrorTest, ErrorCreationWithMessageAndCode) {
    I2CError error("Bus open failed", OPEN_BUS_ERROR);
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(OPEN_BUS_ERROR, error.getCode());
    EXPECT_EQ("Bus open failed", error.getMessage());
}

// Test I2CError creation with code only
TEST_F(I2CErrorTest, ErrorCreationWithCodeOnly) {
    I2CError error(WRITE_ERROR);
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(WRITE_ERROR, error.getCode());
    EXPECT_EQ("", error.getMessage());
}

// Test I2CError creation with message and debug message
TEST_F(I2CErrorTest, ErrorCreationWithDebugMessage) {
    I2CError error("Read failed", "Failed at register 0x42");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(NO_ERROR, error.getCode());
    EXPECT_EQ("Read failed", error.getMessage());
    EXPECT_EQ("Failed at register 0x42", error.getDebugMessage());
}

// Test I2CError creation with all parameters
TEST_F(I2CErrorTest, ErrorCreationWithAllParameters) {
    I2CError error("Complete error", "Debug details", READ_ERROR);
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(READ_ERROR, error.getCode());
    EXPECT_EQ("Complete error", error.getMessage());
    EXPECT_EQ("Debug details", error.getDebugMessage());
}

// Test I2CError creation with code and debug message
TEST_F(I2CErrorTest, ErrorCreationWithCodeAndDebugMessage) {
    I2CError error(BUS_UNOPENED, "Device not initialized");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(BUS_UNOPENED, error.getCode());
    EXPECT_EQ("", error.getMessage());
    EXPECT_EQ("Device not initialized", error.getDebugMessage());
}

// Test assignment operator
TEST_F(I2CErrorTest, AssignmentOperator) {
    I2CError error1("Original error", "Debug info", WRITE_ERROR);
    I2CError error2;

    error2 = error1;

    EXPECT_EQ(error1.getCode(), error2.getCode());
    EXPECT_EQ(error1.getMessage(), error2.getMessage());
    EXPECT_EQ(error1.getDebugMessage(), error2.getDebugMessage());
    EXPECT_EQ(error1.isError(), error2.isError());
}

// Test all error codes
TEST_F(I2CErrorTest, AllErrorCodes) {
    I2CError noError(NO_ERROR);
    I2CError openBusError(OPEN_BUS_ERROR);
    I2CError writeError(WRITE_ERROR);
    I2CError readError(READ_ERROR);
    I2CError busUnopenedError(BUS_UNOPENED);

    EXPECT_EQ(NO_ERROR, noError.getCode());
    EXPECT_EQ(OPEN_BUS_ERROR, openBusError.getCode());
    EXPECT_EQ(WRITE_ERROR, writeError.getCode());
    EXPECT_EQ(READ_ERROR, readError.getCode());
    EXPECT_EQ(BUS_UNOPENED, busUnopenedError.getCode());
}

// Test inheritance from GenericError
TEST_F(I2CErrorTest, InheritanceFromGenericError) {
    I2CError error("Test message", "Debug message", READ_ERROR);

    // Test that GenericError methods work
    EXPECT_TRUE(error.isError());
    EXPECT_EQ("Test message", error.getMessage());
    EXPECT_EQ("Debug message", error.getDebugMessage());

    // Test that I2CError-specific method works
    EXPECT_EQ(READ_ERROR, error.getCode());
}

// Test empty strings
TEST_F(I2CErrorTest, EmptyStrings) {
    I2CError error("", "", WRITE_ERROR);
    EXPECT_TRUE(error.isError());
    EXPECT_EQ("", error.getMessage());
    EXPECT_EQ("", error.getDebugMessage());
    EXPECT_EQ(WRITE_ERROR, error.getCode());
}

// Test self-assignment
TEST_F(I2CErrorTest, SelfAssignment) {
    I2CError error("Original", "Debug", READ_ERROR);
    error = error;

    EXPECT_EQ("Original", error.getMessage());
    EXPECT_EQ("Debug", error.getDebugMessage());
    EXPECT_EQ(READ_ERROR, error.getCode());
}

// Test chain assignment
TEST_F(I2CErrorTest, ChainAssignment) {
    I2CError error1("Error 1", "Debug 1", WRITE_ERROR);
    I2CError error2;
    I2CError error3;

    error3 = error2 = error1;

    EXPECT_EQ(error1.getMessage(), error3.getMessage());
    EXPECT_EQ(error1.getDebugMessage(), error3.getDebugMessage());
    EXPECT_EQ(error1.getCode(), error3.getCode());
}

// Test long messages
TEST_F(I2CErrorTest, LongMessages) {
    std::string longMsg = "This is a very long error message with lots of details about what went wrong during the I2C communication process and why it failed";
    std::string longDebug = "Debug information can also be quite lengthy with stack traces and register dumps and other diagnostic information";

    I2CError error(longMsg, longDebug, READ_ERROR);

    EXPECT_EQ(longMsg, error.getMessage());
    EXPECT_EQ(longDebug, error.getDebugMessage());
    EXPECT_EQ(READ_ERROR, error.getCode());
}
