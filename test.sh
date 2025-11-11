#!/bin/bash
#
# test.sh - Test script for ApraLinuxUtils library
#
# Copyright (c) 2024 Apra Labs
#
# This file is part of ApraUtils.
#
# Licensed under the MIT License.
# See LICENSE file in the project root for full license information.

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
TEST_EXECUTABLE="${BUILD_DIR}/ApraLinuxUtilsTests"

# Default options
VERBOSE=false
FILTER=""
REPEAT=1

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show usage
show_usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Run unit tests for ApraLinuxUtils library.

OPTIONS:
    -h, --help              Show this help message
    -v, --verbose           Verbose test output
    -f, --filter PATTERN    Run only tests matching pattern (e.g., "Range*")
    -r, --repeat N          Repeat tests N times (default: 1)
    -l, --list              List all available tests
    --ctest                 Use CTest instead of direct test execution

EXAMPLES:
    $0                      # Run all tests
    $0 --verbose            # Run tests with verbose output
    $0 --filter "Range*"    # Run only Range tests
    $0 --repeat 10          # Run tests 10 times
    $0 --list               # List all available tests

EOF
}

# Parse command line arguments
USE_CTEST=false
LIST_TESTS=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -r|--repeat)
            REPEAT="$2"
            shift 2
            ;;
        -l|--list)
            LIST_TESTS=true
            shift
            ;;
        --ctest)
            USE_CTEST=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Check if tests are built
if [ ! -f "${TEST_EXECUTABLE}" ]; then
    print_error "Test executable not found: ${TEST_EXECUTABLE}"
    print_info "Build tests first with: ./build.sh --tests"
    exit 1
fi

# List tests if requested
if [ "$LIST_TESTS" = true ]; then
    print_info "Available tests:"
    "${TEST_EXECUTABLE}" --gtest_list_tests
    exit 0
fi

# Print test configuration
print_info "Test Configuration:"
echo "  Test Executable: ${TEST_EXECUTABLE}"
echo "  Verbose: $VERBOSE"
[ -n "$FILTER" ] && echo "  Filter: $FILTER"
echo "  Repeat: $REPEAT times"
echo ""

# Run tests
if [ "$USE_CTEST" = true ]; then
    # Use CTest
    print_info "Running tests with CTest..."
    cd "${BUILD_DIR}"

    CTEST_ARGS=""
    [ "$VERBOSE" = true ] && CTEST_ARGS="$CTEST_ARGS -V"
    [ -n "$FILTER" ] && CTEST_ARGS="$CTEST_ARGS -R $FILTER"
    [ "$REPEAT" -gt 1 ] && CTEST_ARGS="$CTEST_ARGS --repeat until-pass:$REPEAT"

    ctest $CTEST_ARGS || {
        print_error "Tests failed"
        exit 1
    }
else
    # Direct test execution
    print_info "Running tests..."

    TEST_ARGS=""
    [ "$VERBOSE" = true ] && TEST_ARGS="$TEST_ARGS --gtest_print_time=1"
    [ -n "$FILTER" ] && TEST_ARGS="$TEST_ARGS --gtest_filter=$FILTER"
    [ "$REPEAT" -gt 1 ] && TEST_ARGS="$TEST_ARGS --gtest_repeat=$REPEAT"

    "${TEST_EXECUTABLE}" $TEST_ARGS || {
        print_error "Tests failed"
        exit 1
    }
fi

print_success "All tests passed!"

# Show test summary
echo ""
print_info "Test Summary:"
TEST_COUNT=$("${TEST_EXECUTABLE}" --gtest_list_tests 2>/dev/null | grep -E "^\s+[A-Za-z]" | wc -l)
echo "  Total tests: $TEST_COUNT"
[ -n "$FILTER" ] && echo "  Filter applied: $FILTER"
[ "$REPEAT" -gt 1 ] && echo "  Repeated: $REPEAT times"

echo ""
print_success "Testing completed successfully!"
