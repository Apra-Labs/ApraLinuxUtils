/*
 * test_storage_minimal_info.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "models/StorageMinimalInfo.h"

using namespace apra;

class StorageMinimalInfoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test default StorageMinimalInfo creation
TEST_F(StorageMinimalInfoTest, DefaultCreation) {
    StorageMinimalInfo info;

    EXPECT_EQ("", info.m_partition);
    EXPECT_EQ(0, info.m_size);
    EXPECT_EQ("", info.m_fsType);
}

// Test StorageMinimalInfo creation with parameters
TEST_F(StorageMinimalInfoTest, CreationWithParameters) {
    StorageMinimalInfo info("/dev/sda1", 1000000000, "ext4");

    EXPECT_EQ("/dev/sda1", info.m_partition);
    EXPECT_EQ(1000000000, info.m_size);
    EXPECT_EQ("ext4", info.m_fsType);
}

// Test assignment operator
TEST_F(StorageMinimalInfoTest, AssignmentOperator) {
    StorageMinimalInfo info1("/dev/sda1", 500000000, "ext4");
    StorageMinimalInfo info2;

    info2 = info1;

    EXPECT_EQ(info1.m_partition, info2.m_partition);
    EXPECT_EQ(info1.m_size, info2.m_size);
    EXPECT_EQ(info1.m_fsType, info2.m_fsType);
}

// Test assignment operator with different values
TEST_F(StorageMinimalInfoTest, AssignmentOperatorDifferentValues) {
    StorageMinimalInfo info1("/dev/sdb1", 2000000000, "ntfs");
    StorageMinimalInfo info2("/dev/sdc1", 100000, "fat32");

    info2 = info1;

    EXPECT_EQ("/dev/sdb1", info2.m_partition);
    EXPECT_EQ(2000000000, info2.m_size);
    EXPECT_EQ("ntfs", info2.m_fsType);
}

// Test self-assignment
TEST_F(StorageMinimalInfoTest, SelfAssignment) {
    StorageMinimalInfo info("/dev/sda1", 1000000, "ext4");

    info = info;

    EXPECT_EQ("/dev/sda1", info.m_partition);
    EXPECT_EQ(1000000, info.m_size);
    EXPECT_EQ("ext4", info.m_fsType);
}

// Test chain assignment
TEST_F(StorageMinimalInfoTest, ChainAssignment) {
    StorageMinimalInfo info1("/dev/sda1", 500000, "btrfs");
    StorageMinimalInfo info2;
    StorageMinimalInfo info3;

    info3 = info2 = info1;

    EXPECT_EQ("/dev/sda1", info3.m_partition);
    EXPECT_EQ(500000, info3.m_size);
    EXPECT_EQ("btrfs", info3.m_fsType);
}

// Test with empty partition name
TEST_F(StorageMinimalInfoTest, EmptyPartition) {
    StorageMinimalInfo info("", 1000, "ext4");

    EXPECT_EQ("", info.m_partition);
    EXPECT_EQ(1000, info.m_size);
    EXPECT_EQ("ext4", info.m_fsType);
}

// Test with zero size
TEST_F(StorageMinimalInfoTest, ZeroSize) {
    StorageMinimalInfo info("/dev/sda1", 0, "ext4");

    EXPECT_EQ("/dev/sda1", info.m_partition);
    EXPECT_EQ(0, info.m_size);
    EXPECT_EQ("ext4", info.m_fsType);
}

// Test with empty filesystem type
TEST_F(StorageMinimalInfoTest, EmptyFsType) {
    StorageMinimalInfo info("/dev/sda1", 1000, "");

    EXPECT_EQ("/dev/sda1", info.m_partition);
    EXPECT_EQ(1000, info.m_size);
    EXPECT_EQ("", info.m_fsType);
}

// Test with all empty values
TEST_F(StorageMinimalInfoTest, AllEmptyValues) {
    StorageMinimalInfo info("", 0, "");

    EXPECT_EQ("", info.m_partition);
    EXPECT_EQ(0, info.m_size);
    EXPECT_EQ("", info.m_fsType);
}

// Test with large size value
TEST_F(StorageMinimalInfoTest, LargeSize) {
    uint64_t largeSize = 18446744073709551615ULL; // max uint64_t
    StorageMinimalInfo info("/dev/sda1", largeSize, "ext4");

    EXPECT_EQ("/dev/sda1", info.m_partition);
    EXPECT_EQ(largeSize, info.m_size);
    EXPECT_EQ("ext4", info.m_fsType);
}

// Test with common filesystem types
TEST_F(StorageMinimalInfoTest, CommonFilesystemTypes) {
    std::vector<std::string> fsTypes = {"ext4", "ext3", "ext2", "ntfs", "fat32", "exfat", "btrfs", "xfs", "zfs"};

    for (const auto& fsType : fsTypes) {
        StorageMinimalInfo info("/dev/sda1", 1000000, fsType);
        EXPECT_EQ(fsType, info.m_fsType);
    }
}

// Test with various partition naming conventions
TEST_F(StorageMinimalInfoTest, PartitionNamingConventions) {
    std::vector<std::string> partitions = {
        "/dev/sda1",
        "/dev/sdb2",
        "/dev/nvme0n1p1",
        "/dev/mmcblk0p1",
        "/dev/vda1",
        "C:",
        "D:",
        "/mnt/usb"
    };

    for (const auto& partition : partitions) {
        StorageMinimalInfo info(partition, 1000000, "ext4");
        EXPECT_EQ(partition, info.m_partition);
    }
}

// Test with long partition names
TEST_F(StorageMinimalInfoTest, LongPartitionName) {
    std::string longPartition = "/dev/disk/by-uuid/12345678-1234-1234-1234-123456789012";
    StorageMinimalInfo info(longPartition, 5000000, "ext4");

    EXPECT_EQ(longPartition, info.m_partition);
}

// Test with long filesystem type
TEST_F(StorageMinimalInfoTest, LongFsType) {
    std::string longFsType = "very_long_filesystem_type_name_that_is_unlikely";
    StorageMinimalInfo info("/dev/sda1", 1000, longFsType);

    EXPECT_EQ(longFsType, info.m_fsType);
}

// Test multiple assignments
TEST_F(StorageMinimalInfoTest, MultipleAssignments) {
    StorageMinimalInfo info1("/dev/sda1", 100, "ext4");
    StorageMinimalInfo info2;

    info2 = info1;
    EXPECT_EQ("/dev/sda1", info2.m_partition);

    StorageMinimalInfo info3("/dev/sdb1", 200, "ntfs");
    info2 = info3;
    EXPECT_EQ("/dev/sdb1", info2.m_partition);
    EXPECT_EQ(200, info2.m_size);
    EXPECT_EQ("ntfs", info2.m_fsType);
}

// Test typical USB storage scenario
TEST_F(StorageMinimalInfoTest, TypicalUSBStorage) {
    StorageMinimalInfo usbInfo("/dev/sdb1", 32000000000ULL, "vfat");

    EXPECT_EQ("/dev/sdb1", usbInfo.m_partition);
    EXPECT_EQ(32000000000ULL, usbInfo.m_size);
    EXPECT_EQ("vfat", usbInfo.m_fsType);
}

// Test typical SD card scenario
TEST_F(StorageMinimalInfoTest, TypicalSDCard) {
    StorageMinimalInfo sdInfo("/dev/mmcblk0p1", 16000000000ULL, "ext4");

    EXPECT_EQ("/dev/mmcblk0p1", sdInfo.m_partition);
    EXPECT_EQ(16000000000ULL, sdInfo.m_size);
    EXPECT_EQ("ext4", sdInfo.m_fsType);
}

// Test creating multiple instances
TEST_F(StorageMinimalInfoTest, MultipleInstances) {
    StorageMinimalInfo info1("/dev/sda1", 1000, "ext4");
    StorageMinimalInfo info2("/dev/sdb1", 2000, "ntfs");
    StorageMinimalInfo info3("/dev/sdc1", 3000, "fat32");

    EXPECT_EQ("/dev/sda1", info1.m_partition);
    EXPECT_EQ("/dev/sdb1", info2.m_partition);
    EXPECT_EQ("/dev/sdc1", info3.m_partition);

    EXPECT_EQ(1000, info1.m_size);
    EXPECT_EQ(2000, info2.m_size);
    EXPECT_EQ(3000, info3.m_size);
}

// Test modifying values directly
TEST_F(StorageMinimalInfoTest, DirectModification) {
    StorageMinimalInfo info("/dev/sda1", 1000, "ext4");

    info.m_partition = "/dev/sdb1";
    info.m_size = 2000;
    info.m_fsType = "ntfs";

    EXPECT_EQ("/dev/sdb1", info.m_partition);
    EXPECT_EQ(2000, info.m_size);
    EXPECT_EQ("ntfs", info.m_fsType);
}
