/*
 * test_mutex.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/Mutex.h"

using namespace apra;

class MutexTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Basic test for Mutex creation
TEST_F(MutexTest, MutexCreation) {
    Mutex mutex;
    // If we get here without crashing, the mutex was created successfully
    SUCCEED();
}

// Test lock and unlock operations
TEST_F(MutexTest, LockUnlock) {
    Mutex mutex;
    mutex.lock();
    // Mutex is now locked
    mutex.unlock();
    // Mutex is now unlocked
    SUCCEED();
}

// Test multiple lock/unlock cycles
TEST_F(MutexTest, MultipleLockUnlockCycles) {
    Mutex mutex;
    for (int i = 0; i < 10; i++) {
        mutex.lock();
        mutex.unlock();
    }
    SUCCEED();
}
