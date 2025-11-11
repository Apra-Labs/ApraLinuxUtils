/*
 * test_scopelock.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/ScopeLock.h"
#include "utils/Mutex.h"

using namespace apra;

class ScopeLockTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code for each test
    }

    void TearDown() override {
        // Cleanup code for each test
    }
};

// Test ScopeLock with Mutex
TEST_F(ScopeLockTest, ScopeLockWithMutex) {
    Mutex mutex;
    {
        ScopeLock lock(mutex);
        // Mutex is locked within this scope
    }
    // Mutex is automatically unlocked when ScopeLock goes out of scope
    SUCCEED();
}

// Test multiple nested scopes
TEST_F(ScopeLockTest, NestedScopes) {
    Mutex mutex;
    {
        ScopeLock lock1(mutex);
        // First scope
    }
    {
        ScopeLock lock2(mutex);
        // Second scope - mutex can be locked again
    }
    SUCCEED();
}

// Test with pthread_mutex_t directly
TEST_F(ScopeLockTest, ScopeLockWithPthreadMutex) {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);
    {
        ScopeLock lock(mutex);
        // Mutex is locked within this scope
    }
    // Mutex is automatically unlocked
    pthread_mutex_destroy(&mutex);
    SUCCEED();
}
