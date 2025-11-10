/*
 * test_message.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/Message.h"
#include "constants/MessageType.h"

using namespace apra;

class MessageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test Message creation
TEST_F(MessageTest, MessageCreation) {
    Message msg;
    // Message should be created successfully
    SUCCEED();
}

// Test default message type
TEST_F(MessageTest, DefaultMessageType) {
    Message msg;
    EXPECT_EQ(REQUEST_ONLY, msg.getType());
}

// Test setting and getting message type
TEST_F(MessageTest, SetAndGetType) {
    Message msg;
    msg.setType(REQUEST_RESPONSE);
    EXPECT_EQ(REQUEST_RESPONSE, msg.getType());

    msg.setType(REQUEST_ONLY);
    EXPECT_EQ(REQUEST_ONLY, msg.getType());
}

// Test message handle
TEST_F(MessageTest, GetHandle) {
    Message msg;
    uint64_t handle = msg.getHandle();
    // Handle should be non-zero (assuming it's initialized to a unique value)
    EXPECT_NE(0, handle);
}

// Test multiple messages have unique handles
TEST_F(MessageTest, UniqueHandles) {
    Message msg1;
    Message msg2;
    EXPECT_NE(msg1.getHandle(), msg2.getHandle());
}
