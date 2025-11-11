/*
 * test_i2c_message.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/I2CMessage.h"
#include "constants/I2CMessageType.h"

using namespace apra;

class I2CMessageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test default I2C_Message creation
TEST_F(I2CMessageTest, DefaultCreation) {
    I2C_Message msg;

    EXPECT_EQ(I2C_READ, msg.m_type);
    EXPECT_EQ(0, msg.m_registerNumber.size());
    EXPECT_EQ(0, msg.m_data.size());
    EXPECT_EQ(0, msg.m_compareData.size());
    EXPECT_EQ(0, msg.m_retryCount);
    EXPECT_EQ(0, msg.m_delayInUsec);
    EXPECT_EQ(I2C_RETRY_FAILURE_DELAY, msg.m_retryDelayInUsec);
    EXPECT_FALSE(msg.m_allowOtherProcessOnIdle);
    EXPECT_FALSE(msg.m_error.isError());
}

// Test configureWrite with vectors
TEST_F(I2CMessageTest, ConfigureWriteWithVectors) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x10};
    std::vector<uint8_t> data = {0xAB, 0xCD};

    msg.configureWrite(registerNum, data);

    EXPECT_EQ(I2C_WRITE, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x10, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_data.size());
    EXPECT_EQ(0xAB, msg.m_data[0]);
    EXPECT_EQ(0xCD, msg.m_data[1]);
}

// Test configureWrite with uint64_t values
TEST_F(I2CMessageTest, ConfigureWriteWithIntegers) {
    I2C_Message msg;
    uint64_t registerNum = 0x42;
    uint64_t data = 0xABCD;

    msg.configureWrite(registerNum, data, 1, 2);

    EXPECT_EQ(I2C_WRITE, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x42, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_data.size());
    EXPECT_EQ(0xAB, msg.m_data[0]);
    EXPECT_EQ(0xCD, msg.m_data[1]);
}

// Test configureWrite with multi-byte register
TEST_F(I2CMessageTest, ConfigureWriteMultiByteRegister) {
    I2C_Message msg;
    uint64_t registerNum = 0x1234;
    uint64_t data = 0x5678;

    msg.configureWrite(registerNum, data, 2, 2);

    EXPECT_EQ(I2C_WRITE, msg.m_type);
    EXPECT_EQ(2, msg.m_registerNumber.size());
    EXPECT_EQ(0x12, msg.m_registerNumber[0]);
    EXPECT_EQ(0x34, msg.m_registerNumber[1]);
    EXPECT_EQ(2, msg.m_data.size());
    EXPECT_EQ(0x56, msg.m_data[0]);
    EXPECT_EQ(0x78, msg.m_data[1]);
}

// Test configureRead with vectors
TEST_F(I2CMessageTest, ConfigureReadWithVectors) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x20};
    uint64_t expectedDataSize = 4;

    msg.configureRead(registerNum, expectedDataSize);

    EXPECT_EQ(I2C_READ, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x20, msg.m_registerNumber[0]);
    EXPECT_EQ(4, msg.getDataSize());
}

// Test configureRead with integers
TEST_F(I2CMessageTest, ConfigureReadWithIntegers) {
    I2C_Message msg;
    uint64_t registerNum = 0x30;
    uint64_t registerSize = 1;
    uint64_t expectedDataSize = 2;

    msg.configureRead(registerNum, registerSize, expectedDataSize);

    EXPECT_EQ(I2C_READ, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x30, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.getDataSize());
}

// Test configureRead with multi-byte register
TEST_F(I2CMessageTest, ConfigureReadMultiByteRegister) {
    I2C_Message msg;
    uint64_t registerNum = 0xABCD;
    uint64_t registerSize = 2;
    uint64_t expectedDataSize = 8;

    msg.configureRead(registerNum, registerSize, expectedDataSize);

    EXPECT_EQ(I2C_READ, msg.m_type);
    EXPECT_EQ(2, msg.m_registerNumber.size());
    EXPECT_EQ(0xAB, msg.m_registerNumber[0]);
    EXPECT_EQ(0xCD, msg.m_registerNumber[1]);
    EXPECT_EQ(8, msg.getDataSize());
}

// Test configureReadWithComparison using vectors (equal comparison)
TEST_F(I2CMessageTest, ConfigureReadWithComparisonVectorsEqual) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x40};
    std::vector<uint8_t> compareData = {0x11, 0x22};
    uint64_t expectedDataSize = 2;

    msg.configureReadWithComparison(registerNum, expectedDataSize, compareData, true);

    EXPECT_EQ(I2C_READ_COMPARE_EQUAL, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x40, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_compareData.size());
    EXPECT_EQ(0x11, msg.m_compareData[0]);
    EXPECT_EQ(0x22, msg.m_compareData[1]);
}

// Test configureReadWithComparison using vectors (not equal comparison)
TEST_F(I2CMessageTest, ConfigureReadWithComparisonVectorsNotEqual) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x50};
    std::vector<uint8_t> compareData = {0x33, 0x44};
    uint64_t expectedDataSize = 2;

    msg.configureReadWithComparison(registerNum, expectedDataSize, compareData, false);

    EXPECT_EQ(I2C_READ_COMPARE_NOT_EQUAL, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x50, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_compareData.size());
    EXPECT_EQ(0x33, msg.m_compareData[0]);
    EXPECT_EQ(0x44, msg.m_compareData[1]);
}

// Test configureReadWithComparison using integers (equal comparison)
TEST_F(I2CMessageTest, ConfigureReadWithComparisonIntegersEqual) {
    I2C_Message msg;
    uint64_t registerNum = 0x60;
    uint64_t compareData = 0xABCD;
    uint64_t registerSize = 1;
    uint64_t expectedDataSize = 2;

    msg.configureReadWithComparison(registerNum, registerSize, expectedDataSize, compareData, true);

    EXPECT_EQ(I2C_READ_COMPARE_EQUAL, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x60, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_compareData.size());
    EXPECT_EQ(0xAB, msg.m_compareData[0]);
    EXPECT_EQ(0xCD, msg.m_compareData[1]);
}

// Test configureReadWithComparison using integers (not equal comparison)
TEST_F(I2CMessageTest, ConfigureReadWithComparisonIntegersNotEqual) {
    I2C_Message msg;
    uint64_t registerNum = 0x70;
    uint64_t compareData = 0x1234;
    uint64_t registerSize = 1;
    uint64_t expectedDataSize = 2;

    msg.configureReadWithComparison(registerNum, registerSize, expectedDataSize, compareData, false);

    EXPECT_EQ(I2C_READ_COMPARE_NOT_EQUAL, msg.m_type);
    EXPECT_EQ(1, msg.m_registerNumber.size());
    EXPECT_EQ(0x70, msg.m_registerNumber[0]);
    EXPECT_EQ(2, msg.m_compareData.size());
    EXPECT_EQ(0x12, msg.m_compareData[0]);
    EXPECT_EQ(0x34, msg.m_compareData[1]);
}

// Test addDelay
TEST_F(I2CMessageTest, AddDelay) {
    I2C_Message msg;

    EXPECT_EQ(0, msg.m_delayInUsec);

    msg.addDelay(1000);
    EXPECT_EQ(1000, msg.m_delayInUsec);

    msg.addDelay(5000);
    EXPECT_EQ(5000, msg.m_delayInUsec);
}

// Test addDelay with zero (should not change)
TEST_F(I2CMessageTest, AddDelayZero) {
    I2C_Message msg;
    msg.addDelay(1000);
    EXPECT_EQ(1000, msg.m_delayInUsec);

    msg.addDelay(0);
    EXPECT_EQ(1000, msg.m_delayInUsec); // Should remain unchanged
}

// Test setRetries
TEST_F(I2CMessageTest, SetRetries) {
    I2C_Message msg;

    EXPECT_EQ(0, msg.m_retryCount);

    msg.setRetries(3);
    EXPECT_EQ(3, msg.m_retryCount);

    msg.setRetries(10);
    EXPECT_EQ(10, msg.m_retryCount);
}

// Test setRetries with zero (should not change)
TEST_F(I2CMessageTest, SetRetriesZero) {
    I2C_Message msg;
    msg.setRetries(5);
    EXPECT_EQ(5, msg.m_retryCount);

    msg.setRetries(0);
    EXPECT_EQ(5, msg.m_retryCount); // Should remain unchanged
}

// Test getCombinedData after write configuration
TEST_F(I2CMessageTest, GetCombinedDataAfterWrite) {
    I2C_Message msg;
    uint64_t registerNum = 0x10;
    uint64_t data = 0xABCD;

    msg.configureWrite(registerNum, data, 1, 2);

    uint64_t combined = msg.getCombinedData();
    EXPECT_EQ(0xABCD, combined);
}

// Test getCombinedData with single byte
TEST_F(I2CMessageTest, GetCombinedDataSingleByte) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x00};
    std::vector<uint8_t> data = {0xFF};

    msg.configureWrite(registerNum, data);

    uint64_t combined = msg.getCombinedData();
    EXPECT_EQ(0xFF, combined);
}

// Test getCombinedData with multiple bytes
TEST_F(I2CMessageTest, GetCombinedDataMultipleBytes) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x00};
    std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};

    msg.configureWrite(registerNum, data);

    uint64_t combined = msg.getCombinedData();
    EXPECT_EQ(0x01020304, combined);
}

// Test getCombinedRegister
TEST_F(I2CMessageTest, GetCombinedRegister) {
    I2C_Message msg;
    uint64_t registerNum = 0x1234;
    uint64_t data = 0xAB;

    msg.configureWrite(registerNum, data, 2, 1);

    uint64_t combined = msg.getCombinedRegister();
    EXPECT_EQ(0x1234, combined);
}

// Test getCombinedRegister with single byte
TEST_F(I2CMessageTest, GetCombinedRegisterSingleByte) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum = {0x42};
    std::vector<uint8_t> data = {0x00};

    msg.configureWrite(registerNum, data);

    uint64_t combined = msg.getCombinedRegister();
    EXPECT_EQ(0x42, combined);
}

// Test getDataSize
TEST_F(I2CMessageTest, GetDataSize) {
    I2C_Message msg;
    uint64_t registerNum = 0x10;
    uint64_t registerSize = 1;
    uint64_t expectedDataSize = 4;

    msg.configureRead(registerNum, registerSize, expectedDataSize);

    EXPECT_EQ(4, msg.getDataSize());
}

// Test message reconfiguration from write to read
TEST_F(I2CMessageTest, ReconfigureWriteToRead) {
    I2C_Message msg;

    msg.configureWrite(0x10, 0xABCD, 1, 2);
    EXPECT_EQ(I2C_WRITE, msg.m_type);

    msg.configureRead(0x20, 1, 4);
    EXPECT_EQ(I2C_READ, msg.m_type);
    EXPECT_EQ(4, msg.getDataSize());
}

// Test message reconfiguration from read to write
TEST_F(I2CMessageTest, ReconfigureReadToWrite) {
    I2C_Message msg;

    msg.configureRead(0x20, 1, 4);
    EXPECT_EQ(I2C_READ, msg.m_type);

    msg.configureWrite(0x10, 0xABCD, 1, 2);
    EXPECT_EQ(I2C_WRITE, msg.m_type);
}

// Test complex configuration sequence
TEST_F(I2CMessageTest, ComplexConfigurationSequence) {
    I2C_Message msg;

    msg.configureWrite(0x10, 0xFF, 1, 1);
    msg.addDelay(1000);
    msg.setRetries(3);

    EXPECT_EQ(I2C_WRITE, msg.m_type);
    EXPECT_EQ(1000, msg.m_delayInUsec);
    EXPECT_EQ(3, msg.m_retryCount);
    EXPECT_EQ(0xFF, msg.getCombinedData());
}

// Test default retry delay
TEST_F(I2CMessageTest, DefaultRetryDelay) {
    I2C_Message msg;
    EXPECT_EQ(I2C_RETRY_FAILURE_DELAY, msg.m_retryDelayInUsec);
}

// Test error state (default should be no error)
TEST_F(I2CMessageTest, DefaultErrorState) {
    I2C_Message msg;
    EXPECT_FALSE(msg.m_error.isError());
    EXPECT_EQ(NO_ERROR, msg.m_error.getCode());
}

// Test allowOtherProcessOnIdle flag
TEST_F(I2CMessageTest, AllowOtherProcessOnIdleFlag) {
    I2C_Message msg;

    EXPECT_FALSE(msg.m_allowOtherProcessOnIdle);

    msg.m_allowOtherProcessOnIdle = true;
    EXPECT_TRUE(msg.m_allowOtherProcessOnIdle);
}

// Test empty vectors in configureWrite
TEST_F(I2CMessageTest, ConfigureWriteEmptyVectors) {
    I2C_Message msg;
    std::vector<uint8_t> registerNum;
    std::vector<uint8_t> data;

    msg.configureWrite(registerNum, data);

    EXPECT_EQ(I2C_WRITE, msg.m_type);
    EXPECT_EQ(0, msg.m_registerNumber.size());
    EXPECT_EQ(0, msg.m_data.size());
}

// Test large register numbers
TEST_F(I2CMessageTest, LargeRegisterNumbers) {
    I2C_Message msg;
    uint64_t largeRegister = 0xFFFFFFFFFFFFFFFF;

    msg.configureWrite(largeRegister, 0x00, 8, 1);

    EXPECT_EQ(8, msg.m_registerNumber.size());
    uint64_t combined = msg.getCombinedRegister();
    EXPECT_EQ(largeRegister, combined);
}

// Test large data values
TEST_F(I2CMessageTest, LargeDataValues) {
    I2C_Message msg;
    uint64_t largeData = 0x123456789ABCDEF0;

    msg.configureWrite(0x00, largeData, 1, 8);

    EXPECT_EQ(8, msg.m_data.size());
    uint64_t combined = msg.getCombinedData();
    EXPECT_EQ(largeData, combined);
}

// Test all message types
TEST_F(I2CMessageTest, AllMessageTypes) {
    I2C_Message writeMsg;
    writeMsg.configureWrite(0x10, 0xAB, 1, 1);
    EXPECT_EQ(I2C_WRITE, writeMsg.m_type);

    I2C_Message readMsg;
    readMsg.configureRead(0x20, 1, 2);
    EXPECT_EQ(I2C_READ, readMsg.m_type);

    I2C_Message compareEqualMsg;
    compareEqualMsg.configureReadWithComparison(0x30, 1, 2, 0xABCD, true);
    EXPECT_EQ(I2C_READ_COMPARE_EQUAL, compareEqualMsg.m_type);

    I2C_Message compareNotEqualMsg;
    compareNotEqualMsg.configureReadWithComparison(0x40, 1, 2, 0x1234, false);
    EXPECT_EQ(I2C_READ_COMPARE_NOT_EQUAL, compareNotEqualMsg.m_type);
}
