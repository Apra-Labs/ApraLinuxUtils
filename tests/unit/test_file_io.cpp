/*
 * test_file_io.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/FileIO.h"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

using namespace apra;

/**
 * FileIO Tests
 *
 * Tests for file and directory existence checking utilities
 */
class FileIOTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test files and directories in /tmp
        testDir = "/tmp/aprautils_test_dir";
        testFile = "/tmp/aprautils_test_file.txt";
        testSubDir = "/tmp/aprautils_test_dir/subdir";
        testFileInDir = "/tmp/aprautils_test_dir/file.txt";

        // Clean up any existing test artifacts
        TearDown();

        // Create test directory
        mkdir(testDir.c_str(), 0755);

        // Create test file
        std::ofstream ofs(testFile);
        ofs << "test content";
        ofs.close();

        // Create subdirectory
        mkdir(testSubDir.c_str(), 0755);

        // Create file in directory
        std::ofstream ofs2(testFileInDir);
        ofs2 << "test content in dir";
        ofs2.close();
    }

    void TearDown() override {
        // Clean up test files and directories
        unlink(testFileInDir.c_str());
        unlink(testFile.c_str());
        rmdir(testSubDir.c_str());
        rmdir(testDir.c_str());
    }

    std::string testDir;
    std::string testFile;
    std::string testSubDir;
    std::string testFileInDir;
};

// Test file existence checking
TEST_F(FileIOTest, FileExists) {
    EXPECT_TRUE(FileIO::isFileExist(testFile));
}

TEST_F(FileIOTest, FileDoesNotExist) {
    EXPECT_FALSE(FileIO::isFileExist("/tmp/non_existent_file_xyz123.txt"));
}

TEST_F(FileIOTest, FileInDirectory) {
    EXPECT_TRUE(FileIO::isFileExist(testFileInDir));
}

TEST_F(FileIOTest, DirectoryNotAFile) {
    // Directory should not be reported as a file
    EXPECT_FALSE(FileIO::isFileExist(testDir));
}

TEST_F(FileIOTest, EmptyPathFile) {
    EXPECT_FALSE(FileIO::isFileExist(""));
}

TEST_F(FileIOTest, RelativePathFile) {
    // Test with relative path
    std::string currentDir = "test_relative_file.txt";
    std::ofstream ofs(currentDir);
    ofs << "test";
    ofs.close();

    EXPECT_TRUE(FileIO::isFileExist(currentDir));

    unlink(currentDir.c_str());
}

// Test directory existence checking
TEST_F(FileIOTest, DirectoryExists) {
    EXPECT_TRUE(FileIO::isDirectoryExist(testDir));
}

TEST_F(FileIOTest, DirectoryDoesNotExist) {
    EXPECT_FALSE(FileIO::isDirectoryExist("/tmp/non_existent_dir_xyz123"));
}

TEST_F(FileIOTest, SubDirectoryExists) {
    EXPECT_TRUE(FileIO::isDirectoryExist(testSubDir));
}

TEST_F(FileIOTest, FileNotADirectory) {
    // File should not be reported as a directory
    EXPECT_FALSE(FileIO::isDirectoryExist(testFile));
}

TEST_F(FileIOTest, EmptyPathDirectory) {
    EXPECT_FALSE(FileIO::isDirectoryExist(""));
}

TEST_F(FileIOTest, RootDirectory) {
    // Root directory should exist
    EXPECT_TRUE(FileIO::isDirectoryExist("/"));
}

TEST_F(FileIOTest, TmpDirectory) {
    // /tmp should exist on all Linux systems
    EXPECT_TRUE(FileIO::isDirectoryExist("/tmp"));
}

TEST_F(FileIOTest, HomeDirectory) {
    // Home directory should exist
    const char* home = getenv("HOME");
    if (home != nullptr) {
        EXPECT_TRUE(FileIO::isDirectoryExist(home));
    }
}

TEST_F(FileIOTest, CurrentDirectory) {
    // Current directory should exist
    EXPECT_TRUE(FileIO::isDirectoryExist("."));
}

TEST_F(FileIOTest, ParentDirectory) {
    // Parent directory should exist
    EXPECT_TRUE(FileIO::isDirectoryExist(".."));
}

// Test edge cases
TEST_F(FileIOTest, SpecialCharactersInFileName) {
    std::string specialFile = "/tmp/test file with spaces.txt";
    std::ofstream ofs(specialFile);
    ofs << "test";
    ofs.close();

    EXPECT_TRUE(FileIO::isFileExist(specialFile));

    unlink(specialFile.c_str());
}

TEST_F(FileIOTest, VeryLongPath) {
    // Test with a reasonably long path
    std::string longPath = "/tmp/";
    for (int i = 0; i < 10; i++) {
        longPath += "very_long_directory_name_";
    }
    longPath += "file.txt";

    // This should not exist and should not crash
    EXPECT_FALSE(FileIO::isFileExist(longPath));
}

TEST_F(FileIOTest, PathWithTrailingSlash) {
    std::string pathWithSlash = testDir + "/";
    EXPECT_TRUE(FileIO::isDirectoryExist(pathWithSlash));
}

TEST_F(FileIOTest, SymlinkToFile) {
    std::string linkPath = "/tmp/aprautils_test_symlink";
    symlink(testFile.c_str(), linkPath.c_str());

    EXPECT_TRUE(FileIO::isFileExist(linkPath));

    unlink(linkPath.c_str());
}

TEST_F(FileIOTest, SymlinkToDirectory) {
    std::string linkPath = "/tmp/aprautils_test_dir_symlink";
    symlink(testDir.c_str(), linkPath.c_str());

    EXPECT_TRUE(FileIO::isDirectoryExist(linkPath));

    unlink(linkPath.c_str());
}

TEST_F(FileIOTest, BrokenSymlink) {
    std::string brokenLink = "/tmp/aprautils_test_broken_link";
    std::string targetPath = "/tmp/non_existent_target_xyz123";

    symlink(targetPath.c_str(), brokenLink.c_str());

    // Broken symlink should not be reported as existing file or directory
    EXPECT_FALSE(FileIO::isFileExist(brokenLink));
    EXPECT_FALSE(FileIO::isDirectoryExist(brokenLink));

    unlink(brokenLink.c_str());
}

TEST_F(FileIOTest, PermissionDeniedDirectory) {
    std::string restrictedDir = "/tmp/aprautils_restricted_dir";
    mkdir(restrictedDir.c_str(), 0000);  // No permissions

    // Should still be able to check existence (but not access contents)
    EXPECT_TRUE(FileIO::isDirectoryExist(restrictedDir));

    chmod(restrictedDir.c_str(), 0755);  // Restore permissions for cleanup
    rmdir(restrictedDir.c_str());
}

TEST_F(FileIOTest, HiddenFile) {
    std::string hiddenFile = "/tmp/.aprautils_hidden_file";
    std::ofstream ofs(hiddenFile);
    ofs << "hidden";
    ofs.close();

    EXPECT_TRUE(FileIO::isFileExist(hiddenFile));

    unlink(hiddenFile.c_str());
}

TEST_F(FileIOTest, HiddenDirectory) {
    std::string hiddenDir = "/tmp/.aprautils_hidden_dir";
    mkdir(hiddenDir.c_str(), 0755);

    EXPECT_TRUE(FileIO::isDirectoryExist(hiddenDir));

    rmdir(hiddenDir.c_str());
}
