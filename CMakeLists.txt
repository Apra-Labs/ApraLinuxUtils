cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

project(ApraUtils)

# Platform check - this library is Linux-only
if(NOT UNIX OR APPLE)
    message(FATAL_ERROR
        "\n"
        "========================================================================\n"
        "  ApraUtils is designed for embedded Linux systems only.\n"
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

add_library(ApraUtils STATIC ${CORE_CODE_FILES})
