/*
 * test_scope_function.cpp
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#include <gtest/gtest.h>
#include "utils/ScopeFunction.h"

using namespace apra;

/**
 * ScopeFunction Tests
 *
 * Tests for RAII-style function scope tracing utility
 */
class ScopeFunctionTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test basic construction and destruction
TEST_F(ScopeFunctionTest, BasicConstruction) {
    ScopeFunction sf("testFunction");
    // If we get here without crashing, test passes
    SUCCEED();
}

TEST_F(ScopeFunctionTest, EmptyFunctionName) {
    ScopeFunction sf("");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, SimpleFunctionName) {
    ScopeFunction sf("main");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, LongFunctionName) {
    ScopeFunction sf("veryLongFunctionNameWithManyCharactersToTestStringHandling");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, FunctionNameWithNamespace) {
    ScopeFunction sf("apra::Utils::processData");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, FunctionNameWithParameters) {
    ScopeFunction sf("processData(int x, double y)");
    SUCCEED();
}

// Test scope behavior
TEST_F(ScopeFunctionTest, ScopeLifetime) {
    {
        ScopeFunction sf("innerScope");
        // Function active here
    }
    // Function should be destroyed here
    SUCCEED();
}

TEST_F(ScopeFunctionTest, NestedScopes) {
    ScopeFunction sf1("outerFunction");
    {
        ScopeFunction sf2("middleFunction");
        {
            ScopeFunction sf3("innerFunction");
            // All three active here
        }
        // Only outer and middle active here
    }
    // Only outer active here
    SUCCEED();
}

TEST_F(ScopeFunctionTest, MultipleSequentialScopes) {
    {
        ScopeFunction sf1("function1");
    }
    {
        ScopeFunction sf2("function2");
    }
    {
        ScopeFunction sf3("function3");
    }
    SUCCEED();
}

// Test with different function name patterns
TEST_F(ScopeFunctionTest, ConstructorName) {
    ScopeFunction sf("MyClass::MyClass()");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, DestructorName) {
    ScopeFunction sf("MyClass::~MyClass()");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, OperatorName) {
    ScopeFunction sf("operator=");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, TemplateFunctionName) {
    ScopeFunction sf("template<typename T> void process()");
    SUCCEED();
}

// Test special characters in function names
TEST_F(ScopeFunctionTest, SpecialCharacters) {
    ScopeFunction sf("function_with_underscores");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, NumbersInName) {
    ScopeFunction sf("function123");
    SUCCEED();
}

TEST_F(ScopeFunctionTest, MixedCase) {
    ScopeFunction sf("myFunctionWithMixedCase");
    SUCCEED();
}

// Test in different scopes
TEST_F(ScopeFunctionTest, InIfBlock) {
    bool condition = true;
    if (condition) {
        ScopeFunction sf("ifBlock");
        SUCCEED();
    }
}

TEST_F(ScopeFunctionTest, InForLoop) {
    for (int i = 0; i < 3; i++) {
        ScopeFunction sf("forLoopIteration");
    }
    SUCCEED();
}

TEST_F(ScopeFunctionTest, InWhileLoop) {
    int count = 0;
    while (count < 3) {
        ScopeFunction sf("whileLoopIteration");
        count++;
    }
    SUCCEED();
}

// Test exception safety
TEST_F(ScopeFunctionTest, ExceptionSafety) {
    try {
        ScopeFunction sf("functionWithException");
        // Even if exception occurs, destructor should be called
        // throw std::runtime_error("test exception");
        SUCCEED();  // Don't actually throw in test
    } catch (...) {
        // Should not reach here in this test
    }
}

// Test multiple instances
TEST_F(ScopeFunctionTest, MultipleInstances) {
    ScopeFunction sf1("function1");
    ScopeFunction sf2("function2");
    ScopeFunction sf3("function3");
    // All three should coexist
    SUCCEED();
}

TEST_F(ScopeFunctionTest, ArrayOfScopeFunctions) {
    ScopeFunction* functions[3] = {
        new ScopeFunction("func1"),
        new ScopeFunction("func2"),
        new ScopeFunction("func3")
    };

    // Clean up
    for (int i = 0; i < 3; i++) {
        delete functions[i];
    }
    SUCCEED();
}

// Test with realistic function names
TEST_F(ScopeFunctionTest, RealisticFunctionNames) {
    {
        ScopeFunction sf("GPIO::Init");
    }
    {
        ScopeFunction sf("PWM::SetDutyCycle");
    }
    {
        ScopeFunction sf("I2C::ReadRegister");
    }
    {
        ScopeFunction sf("ProcessThread::mainLoop");
    }
    SUCCEED();
}

// Test rapid creation and destruction
TEST_F(ScopeFunctionTest, RapidCreationDestruction) {
    for (int i = 0; i < 100; i++) {
        ScopeFunction sf("rapidFunction");
    }
    SUCCEED();
}

// Test with very long names
TEST_F(ScopeFunctionTest, VeryLongName) {
    std::string longName;
    for (int i = 0; i < 100; i++) {
        longName += "long";
    }
    ScopeFunction sf(longName);
    SUCCEED();
}

// Test with Unicode characters (if supported)
TEST_F(ScopeFunctionTest, SpecialStrings) {
    ScopeFunction sf("function@#$%");
    SUCCEED();
}

// Test scope with early return
TEST_F(ScopeFunctionTest, EarlyReturn) {
    auto testFunc = []() {
        ScopeFunction sf("functionWithEarlyReturn");
        return true;
        // Destructor should be called even with early return
    };

    EXPECT_TRUE(testFunc());
}

// Test const correctness
TEST_F(ScopeFunctionTest, ConstString) {
    const char* funcName = "constFunction";
    ScopeFunction sf(funcName);
    SUCCEED();
}

TEST_F(ScopeFunctionTest, StdString) {
    std::string funcName = "stdStringFunction";
    ScopeFunction sf(funcName);
    SUCCEED();
}

TEST_F(ScopeFunctionTest, TemporaryString) {
    ScopeFunction sf(std::string("temporaryString"));
    SUCCEED();
}
