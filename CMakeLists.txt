cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

project(ApraUtils)

find_package(PkgConfig REQUIRED)

include_directories(AFTER SYSTEM include)
include_directories(${CMAKE_SOURCE_DIR}/includes/)

file(GLOB_RECURSE CORE_CODE_FILES RELATIVE ${CMAKE_SOURCE_DIR} src/**.cpp src/**.c includes/**.h)
SET( CORE_CODE ${CORE_CODE_FILES}) 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(ApraUtils STATIC ${CORE_CODE_FILES})
