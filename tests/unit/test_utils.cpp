/*
 * test_utils.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include "utils/Utils.h"
#include "models/Range.h"

using namespace apra;

class UtilsTest : public ::testing::Test {
protected:
    std::string testDir;
    std::string testFile;

    void SetUp() override {
        testDir = "/tmp/apra_utils_test";
        testFile = "/tmp/apra_utils_test_file.txt";

        // Clean up any existing test files
        std::remove(testFile.c_str());
        Utils::exec("rm -rf " + testDir);
    }

    void TearDown() override {
        // Clean up test files
        std::remove(testFile.c_str());
        Utils::exec("rm -rf " + testDir);
    }
};

// Test extractBytes function
TEST_F(UtilsTest, ExtractBytesBasic) {
    uint64_t value = 0x0102030405060708;
    std::vector<uint8_t> bytes = Utils::extractBytes(value, 8);

    ASSERT_EQ(8, bytes.size());
    EXPECT_EQ(0x01, bytes[0]);
    EXPECT_EQ(0x02, bytes[1]);
    EXPECT_EQ(0x03, bytes[2]);
    EXPECT_EQ(0x04, bytes[3]);
    EXPECT_EQ(0x05, bytes[4]);
    EXPECT_EQ(0x06, bytes[5]);
    EXPECT_EQ(0x07, bytes[6]);
    EXPECT_EQ(0x08, bytes[7]);
}

// Test extractBytes with fewer bytes
TEST_F(UtilsTest, ExtractBytesPartial) {
    uint64_t value = 0xABCD;
    std::vector<uint8_t> bytes = Utils::extractBytes(value, 2);

    ASSERT_EQ(2, bytes.size());
    EXPECT_EQ(0xAB, bytes[0]);
    EXPECT_EQ(0xCD, bytes[1]);
}

// Test extractBytes with single byte
TEST_F(UtilsTest, ExtractBytesSingle) {
    uint64_t value = 0x42;
    std::vector<uint8_t> bytes = Utils::extractBytes(value, 1);

    ASSERT_EQ(1, bytes.size());
    EXPECT_EQ(0x42, bytes[0]);
}

// Test extractBytes with zero
TEST_F(UtilsTest, ExtractBytesZero) {
    uint64_t value = 0;
    std::vector<uint8_t> bytes = Utils::extractBytes(value, 4);

    ASSERT_EQ(4, bytes.size());
    for (auto byte : bytes) {
        EXPECT_EQ(0, byte);
    }
}

// Test extractBytes with overflow (more than 8 bytes requested)
TEST_F(UtilsTest, ExtractBytesOverflow) {
    uint64_t value = 0xFFFFFFFFFFFFFFFF;
    std::vector<uint8_t> bytes = Utils::extractBytes(value, 10);

    // Should cap at 8 bytes
    ASSERT_EQ(8, bytes.size());
}

// Test combineBytes function
TEST_F(UtilsTest, CombineBytesBasic) {
    std::vector<uint8_t> bytes = {0x01, 0x02, 0x03, 0x04};
    uint64_t result = Utils::combineBytes(bytes);

    EXPECT_EQ(0x01020304, result);
}

// Test combineBytes with single byte
TEST_F(UtilsTest, CombineBytesSingle) {
    std::vector<uint8_t> bytes = {0xFF};
    uint64_t result = Utils::combineBytes(bytes);

    EXPECT_EQ(0xFF, result);
}

// Test combineBytes with all bytes
TEST_F(UtilsTest, CombineBytesAll) {
    std::vector<uint8_t> bytes = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint64_t result = Utils::combineBytes(bytes);

    EXPECT_EQ(0x0102030405060708, result);
}

// Test combineBytes with empty vector
TEST_F(UtilsTest, CombineBytesEmpty) {
    std::vector<uint8_t> bytes;
    uint64_t result = Utils::combineBytes(bytes);

    EXPECT_EQ(0, result);
}

// Test extractBytes and combineBytes round-trip
TEST_F(UtilsTest, ExtractCombineRoundTrip) {
    uint64_t original = 0x123456789ABCDEF0;
    std::vector<uint8_t> bytes = Utils::extractBytes(original, 8);
    uint64_t result = Utils::combineBytes(bytes);

    EXPECT_EQ(original, result);
}

// Test inRange function
TEST_F(UtilsTest, InRangeWithinBounds) {
    Range range(10, 20);

    EXPECT_TRUE(Utils::inRange(10, range));
    EXPECT_TRUE(Utils::inRange(15, range));
    EXPECT_TRUE(Utils::inRange(20, range));
}

// Test inRange function outside bounds
TEST_F(UtilsTest, InRangeOutsideBounds) {
    Range range(10, 20);

    EXPECT_FALSE(Utils::inRange(9, range));
    EXPECT_FALSE(Utils::inRange(21, range));
    EXPECT_FALSE(Utils::inRange(0, range));
    EXPECT_FALSE(Utils::inRange(100, range));
}

// Test inRange with negative values
TEST_F(UtilsTest, InRangeNegative) {
    Range range(-50, -10);

    EXPECT_TRUE(Utils::inRange(-50, range));
    EXPECT_TRUE(Utils::inRange(-30, range));
    EXPECT_TRUE(Utils::inRange(-10, range));
    EXPECT_FALSE(Utils::inRange(-51, range));
    EXPECT_FALSE(Utils::inRange(-9, range));
    EXPECT_FALSE(Utils::inRange(0, range));
}

// Test inRange with mixed negative and positive
TEST_F(UtilsTest, InRangeMixed) {
    Range range(-10, 10);

    EXPECT_TRUE(Utils::inRange(-10, range));
    EXPECT_TRUE(Utils::inRange(0, range));
    EXPECT_TRUE(Utils::inRange(10, range));
    EXPECT_FALSE(Utils::inRange(-11, range));
    EXPECT_FALSE(Utils::inRange(11, range));
}

// Test caseInsensitiveSearch function
TEST_F(UtilsTest, CaseInsensitiveSearchFound) {
    EXPECT_TRUE(Utils::caseInsensitiveSearch("Hello World", "world"));
    EXPECT_TRUE(Utils::caseInsensitiveSearch("Hello World", "HELLO"));
    EXPECT_TRUE(Utils::caseInsensitiveSearch("Hello World", "LLO W"));
    EXPECT_TRUE(Utils::caseInsensitiveSearch("TestString", "string"));
}

// Test caseInsensitiveSearch not found
TEST_F(UtilsTest, CaseInsensitiveSearchNotFound) {
    EXPECT_FALSE(Utils::caseInsensitiveSearch("Hello World", "xyz"));
    EXPECT_FALSE(Utils::caseInsensitiveSearch("Hello World", "worldx"));
    EXPECT_FALSE(Utils::caseInsensitiveSearch("Test", "testing"));
}

// Test caseInsensitiveSearch with empty strings
TEST_F(UtilsTest, CaseInsensitiveSearchEmpty) {
    EXPECT_TRUE(Utils::caseInsensitiveSearch("Hello", ""));
    EXPECT_TRUE(Utils::caseInsensitiveSearch("", ""));
    EXPECT_FALSE(Utils::caseInsensitiveSearch("", "test"));
}

// Test trim function
TEST_F(UtilsTest, TrimBasic) {
    EXPECT_EQ("test", Utils::trim("  test  "));
    EXPECT_EQ("test", Utils::trim("test  "));
    EXPECT_EQ("test", Utils::trim("  test"));
    EXPECT_EQ("test", Utils::trim("test"));
}

// Test trim with various whitespace
TEST_F(UtilsTest, TrimWhitespace) {
    EXPECT_EQ("test", Utils::trim("\t\ttest\n\n"));
    EXPECT_EQ("test", Utils::trim("\r\ntest\r\n"));
    EXPECT_EQ("test", Utils::trim("\f\vtest\f\v"));
}

// Test trim with only whitespace
TEST_F(UtilsTest, TrimOnlyWhitespace) {
    EXPECT_EQ("", Utils::trim("   "));
    EXPECT_EQ("", Utils::trim("\t\n\r"));
    EXPECT_EQ("", Utils::trim(""));
}

// Test trim preserving internal whitespace
TEST_F(UtilsTest, TrimInternalWhitespace) {
    EXPECT_EQ("hello world", Utils::trim("  hello world  "));
    EXPECT_EQ("a  b  c", Utils::trim("  a  b  c  "));
}

// Test convertToU12p4 and convertFrom12p4
TEST_F(UtilsTest, Convert12p4Integer) {
    double value = 5.0;
    uint16_t encoded = Utils::convertToU12p4(value);
    double decoded = Utils::convertFrom12p4(encoded);

    EXPECT_DOUBLE_EQ(value, decoded);
}

// Test convertToU12p4 and convertFrom12p4 with decimal
TEST_F(UtilsTest, Convert12p4Decimal) {
    double value = 5.5;
    uint16_t encoded = Utils::convertToU12p4(value);
    double decoded = Utils::convertFrom12p4(encoded);

    EXPECT_NEAR(value, decoded, 0.1);
}

// Test convertToU12p4 and convertFrom12p4 round-trip
TEST_F(UtilsTest, Convert12p4RoundTrip) {
    double values[] = {0.0, 1.0, 2.5, 10.25, 15.75, 100.0};

    for (double value : values) {
        uint16_t encoded = Utils::convertToU12p4(value);
        double decoded = Utils::convertFrom12p4(encoded);
        EXPECT_NEAR(value, decoded, 0.1) << "Failed for value: " << value;
    }
}

// Test convertTo10p6 and convertFrom10p6
TEST_F(UtilsTest, Convert10p6Integer) {
    double value = 3.0;
    uint16_t encoded = Utils::convertTo10p6(value);
    double decoded = Utils::convertFrom10p6(encoded);

    EXPECT_DOUBLE_EQ(value, decoded);
}

// Test convertTo10p6 and convertFrom10p6 round-trip
TEST_F(UtilsTest, Convert10p6RoundTrip) {
    double values[] = {0.0, 1.0, 2.5, 5.25, 8.75, 50.0};

    for (double value : values) {
        uint16_t encoded = Utils::convertTo10p6(value);
        double decoded = Utils::convertFrom10p6(encoded);
        EXPECT_NEAR(value, decoded, 0.02) << "Failed for value: " << value;
    }
}

// Test convertToUFormat and convertFromUFormat
TEST_F(UtilsTest, ConvertUFormatRoundTrip) {
    double value = 7.5;
    uint8_t format = 4;

    uint16_t encoded = Utils::convertToUFormat(value, format);
    double decoded = Utils::convertFromUFormat(encoded, format);

    EXPECT_NEAR(value, decoded, 0.1);
}

// Test mergefrom8Bytes and extractTo8Bytes
TEST_F(UtilsTest, MergeExtract8BytesRoundTrip) {
    uint64_t original = 0x123456789ABCDEF0;
    uint8_t bytes[8];

    Utils::extractTo8Bytes(original, bytes);
    uint64_t result = Utils::mergefrom8Bytes(bytes);

    EXPECT_EQ(original, result);
}

// Test extractTo8Bytes with zero
TEST_F(UtilsTest, ExtractTo8BytesZero) {
    uint64_t value = 0;
    uint8_t bytes[8];

    Utils::extractTo8Bytes(value, bytes);

    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(0, bytes[i]);
    }
}

// Test mergefrom8Bytes with all bytes set
TEST_F(UtilsTest, MergeFrom8BytesAllSet) {
    uint8_t bytes[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint64_t result = Utils::mergefrom8Bytes(bytes);

    EXPECT_EQ(0xFFFFFFFFFFFFFFFF, result);
}

// Test mergefrom8Bytes with nullptr
TEST_F(UtilsTest, MergeFrom8BytesNull) {
    uint64_t result = Utils::mergefrom8Bytes(nullptr);
    EXPECT_EQ(0, result);
}

// Test extractTo8Bytes with nullptr
TEST_F(UtilsTest, ExtractTo8BytesNull) {
    // Should not crash
    Utils::extractTo8Bytes(0x123456, nullptr);
    SUCCEED();
}

// Test saveRawFile
TEST_F(UtilsTest, SaveRawFile) {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
    size_t size = 5;

    bool result = Utils::saveRawFile(testFile, data, size);
    EXPECT_TRUE(result);
    EXPECT_TRUE(Utils::fileExists(testFile));

    // Verify file contents
    std::ifstream file(testFile, std::ios::binary);
    ASSERT_TRUE(file.is_open());

    uint8_t readData[5];
    file.read(reinterpret_cast<char*>(readData), 5);

    for (size_t i = 0; i < size; i++) {
        EXPECT_EQ(data[i], readData[i]);
    }
}

// Test saveRawFile with invalid path
TEST_F(UtilsTest, SaveRawFileInvalidPath) {
    uint8_t data[] = {0x01, 0x02, 0x03};
    std::string invalidPath = "/invalid/path/that/does/not/exist/file.bin";

    bool result = Utils::saveRawFile(invalidPath, data, 3);
    EXPECT_FALSE(result);
}

// Test fileExists
TEST_F(UtilsTest, FileExists) {
    // Create a test file
    std::ofstream file(testFile);
    file << "test content";
    file.close();

    EXPECT_TRUE(Utils::fileExists(testFile));

    std::remove(testFile.c_str());

    EXPECT_FALSE(Utils::fileExists(testFile));
}

// Test fileExists with directory
TEST_F(UtilsTest, FileExistsNotForDirectory) {
    Utils::makeDir(testDir);
    EXPECT_FALSE(Utils::fileExists(testDir));
}

// Test directoryExists
TEST_F(UtilsTest, DirectoryExists) {
    EXPECT_FALSE(Utils::directoryExists(testDir));

    Utils::makeDir(testDir);

    EXPECT_TRUE(Utils::directoryExists(testDir));
}

// Test directoryExists with file
TEST_F(UtilsTest, DirectoryExistsNotForFile) {
    std::ofstream file(testFile);
    file << "test";
    file.close();

    EXPECT_FALSE(Utils::directoryExists(testFile));
}

// Test makeDir
TEST_F(UtilsTest, MakeDir) {
    std::string result = Utils::makeDir(testDir);

    EXPECT_EQ(testDir, result);
    EXPECT_TRUE(Utils::directoryExists(testDir));
}

// Test makeDir with nested directories
TEST_F(UtilsTest, MakeDirNested) {
    std::string nestedDir = testDir + "/nested/deep/path";
    std::string result = Utils::makeDir(nestedDir);

    EXPECT_EQ(nestedDir, result);
    EXPECT_TRUE(Utils::directoryExists(nestedDir));
}

// Test readTextFile
TEST_F(UtilsTest, ReadTextFile) {
    std::ofstream file(testFile);
    file << "testcontent";
    file.close();

    std::string content = Utils::readTextFile(testFile);
    EXPECT_EQ("testcontent", content);
}

// Test readTextFile with non-existent file
TEST_F(UtilsTest, ReadTextFileNotExists) {
    std::string content = Utils::readTextFile("/nonexistent/file.txt");
    EXPECT_EQ("", content);
}

// Test exec function
TEST_F(UtilsTest, ExecCommand) {
    std::string result = Utils::exec("echo 'test'");
    EXPECT_TRUE(result.find("test") != std::string::npos);
}

// Test exec with debug flag
TEST_F(UtilsTest, ExecCommandDebug) {
    std::string result = Utils::exec("echo 'debug'", true);
    EXPECT_TRUE(result.find("debug") != std::string::npos);
}
