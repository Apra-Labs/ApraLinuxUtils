cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

project(ApraLinuxUtils)

# Options for building tests and coverage
option(BUILD_TESTS "Build unit tests" OFF)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

# Platform check - this library is Linux-only
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR
        "\n"
        "========================================================================\n"
        "  ApraLinuxUtils is designed for embedded Linux systems only.\n"
        "  \n"
        "  This library requires:\n"
        "    - Linux kernel headers (I2C, GPIO, PWM)\n"
        "    - POSIX threads (pthread)\n"
        "    - Linux-specific system interfaces\n"
        "  \n"
        "  Supported platforms: Linux only\n"
        "  Current platform: ${CMAKE_SYSTEM_NAME}\n"
        "========================================================================\n"
    )
endif()

find_package(PkgConfig REQUIRED)

include_directories(AFTER SYSTEM include)
include_directories(${CMAKE_SOURCE_DIR}/includes/)

file(GLOB_RECURSE CORE_CODE_FILES RELATIVE ${CMAKE_SOURCE_DIR} src/**.cpp src/**.c includes/**.h)
SET( CORE_CODE ${CORE_CODE_FILES}) 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Coverage flags
if(ENABLE_COVERAGE)
    message(STATUS "Coverage reporting enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

add_library(ApraLinuxUtils STATIC ${CORE_CODE_FILES})

# Google Test setup
if(BUILD_TESTS)
    message(STATUS "Building unit tests")

    # Download and configure Google Test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # Enable testing
    enable_testing()

    # Collect all test files
    file(GLOB_RECURSE TEST_SOURCES tests/*.cpp)

    # Create test executable
    add_executable(ApraLinuxUtilsTests ${TEST_SOURCES})

    # Link against ApraLinuxUtils library and Google Test
    target_link_libraries(ApraLinuxUtilsTests
        ApraLinuxUtils
        gtest
        gtest_main
        pthread
    )

    # Include directories for tests
    target_include_directories(ApraLinuxUtilsTests PRIVATE
        ${CMAKE_SOURCE_DIR}/includes
        ${CMAKE_SOURCE_DIR}/tests
    )

    # Add tests to CTest
    include(GoogleTest)
    gtest_discover_tests(ApraLinuxUtilsTests)
endif()
