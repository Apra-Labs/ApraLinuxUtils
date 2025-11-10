/*
 * test_generic_error.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/GenericError.h"

using namespace apra;

class GenericErrorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test default GenericError creation
TEST_F(GenericErrorTest, DefaultErrorCreation) {
    GenericError error;
    EXPECT_FALSE(error.isError());
}

// Test GenericError creation with message
TEST_F(GenericErrorTest, ErrorCreationWithMessage) {
    GenericError error("Test error message");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ("Test error message", error.getMessage());
}

// Test GenericError creation with message and debug message
TEST_F(GenericErrorTest, ErrorCreationWithDebugMessage) {
    GenericError error("Test error", "Debug info");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ("Test error", error.getMessage());
    EXPECT_EQ("Debug info", error.getDebugMessage());
}

// Test empty message
TEST_F(GenericErrorTest, EmptyMessage) {
    GenericError error("");
    EXPECT_TRUE(error.isError());
    EXPECT_EQ("", error.getMessage());
}

// Test long message
TEST_F(GenericErrorTest, LongMessage) {
    std::string longMsg = "This is a very long error message that contains multiple words and punctuation marks!";
    GenericError error(longMsg);
    EXPECT_TRUE(error.isError());
    EXPECT_EQ(longMsg, error.getMessage());
}
