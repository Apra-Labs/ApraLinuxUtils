#!/bin/bash
#
# coverage.sh - Code coverage report generator for ApraUtils
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
COVERAGE_DIR="${BUILD_DIR}/coverage"
TEST_EXECUTABLE="${BUILD_DIR}/ApraUtilsTests"

# Default options
OPEN_BROWSER=false
COVERAGE_THRESHOLD=90

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

Generate code coverage report for ApraUtils library.

OPTIONS:
    -h, --help              Show this help message
    -o, --open              Open coverage report in browser after generation
    -t, --threshold N       Coverage threshold percentage (default: 90)
    --clean                 Clean previous coverage data before generating

EXAMPLES:
    $0                      # Generate coverage report
    $0 --open               # Generate and open in browser
    $0 --threshold 85       # Set coverage threshold to 85%
    $0 --clean              # Clean and regenerate coverage

REQUIREMENTS:
    - Build with coverage: ./build.sh --coverage
    - lcov installed: sudo apt-get install lcov

EOF
}

# Parse command line arguments
CLEAN_COVERAGE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -o|--open)
            OPEN_BROWSER=true
            shift
            ;;
        -t|--threshold)
            COVERAGE_THRESHOLD="$2"
            shift 2
            ;;
        --clean)
            CLEAN_COVERAGE=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Check for required tools
print_info "Checking for required tools..."

if ! command -v lcov &> /dev/null; then
    print_error "lcov is not installed"
    print_info "Install with: sudo apt-get install lcov"
    exit 1
fi

if ! command -v genhtml &> /dev/null; then
    print_error "genhtml is not installed (part of lcov package)"
    print_info "Install with: sudo apt-get install lcov"
    exit 1
fi

# Check if tests are built with coverage
if [ ! -f "${TEST_EXECUTABLE}" ]; then
    print_error "Test executable not found: ${TEST_EXECUTABLE}"
    print_info "Build with coverage first: ./build.sh --coverage"
    exit 1
fi

# Check if built with coverage flags
if ! nm "${TEST_EXECUTABLE}" | grep -q gcov 2>/dev/null; then
    print_warning "Tests may not be built with coverage enabled"
    print_info "Rebuild with: ./build.sh --coverage"
fi

# Clean previous coverage data if requested
if [ "$CLEAN_COVERAGE" = true ]; then
    print_info "Cleaning previous coverage data..."
    find "${BUILD_DIR}" -name "*.gcda" -delete
    rm -rf "${COVERAGE_DIR}"
    print_success "Coverage data cleaned"
fi

# Create coverage directory
mkdir -p "${COVERAGE_DIR}"

# Run tests to generate coverage data
print_info "Running tests to generate coverage data..."
cd "${BUILD_DIR}"
"${TEST_EXECUTABLE}" || {
    print_error "Tests failed"
    exit 1
}
print_success "Tests completed"

# Capture coverage data
print_info "Capturing coverage data..."
lcov --capture \
    --directory . \
    --output-file "${COVERAGE_DIR}/coverage.info" \
    --rc branch_coverage=1 \
    --ignore-errors mismatch \
    || {
        print_error "Failed to capture coverage data"
        exit 1
    }

# Filter out system headers and test files
print_info "Filtering coverage data..."
lcov --remove "${COVERAGE_DIR}/coverage.info" \
    '/usr/*' \
    '*/tests/*' \
    '*/build/*' \
    '*/googletest/*' \
    '*/_deps/*' \
    --output-file "${COVERAGE_DIR}/coverage_filtered.info" \
    --rc branch_coverage=1 \
    --ignore-errors mismatch,unused \
    || {
        print_error "Failed to filter coverage data"
        exit 1
    }

# Generate HTML report
print_info "Generating HTML coverage report..."
genhtml "${COVERAGE_DIR}/coverage_filtered.info" \
    --output-directory "${COVERAGE_DIR}/html" \
    --title "ApraUtils Code Coverage" \
    --legend \
    --show-details \
    --branch-coverage \
    --rc branch_coverage=1 \
    --ignore-errors mismatch,unused \
    || {
        print_error "Failed to generate HTML report"
        exit 1
    }

print_success "Coverage report generated"

# Display coverage summary
echo ""
print_info "Coverage Summary:"
lcov --summary "${COVERAGE_DIR}/coverage_filtered.info" --rc branch_coverage=1

# Extract coverage percentage
COVERAGE_LINE=$(lcov --summary "${COVERAGE_DIR}/coverage_filtered.info" 2>&1 | grep "lines......" | tail -1)
COVERAGE_PERCENT=$(echo "$COVERAGE_LINE" | grep -oP '\d+\.\d+(?=%)')

echo ""
print_info "Line Coverage: ${COVERAGE_PERCENT}%"
print_info "Coverage Threshold: ${COVERAGE_THRESHOLD}%"

# Check coverage threshold
if (( $(echo "$COVERAGE_PERCENT >= $COVERAGE_THRESHOLD" | bc -l) )); then
    print_success "Coverage threshold met! ✓"
else
    print_warning "Coverage is below threshold"
    print_info "Current: ${COVERAGE_PERCENT}% | Target: ${COVERAGE_THRESHOLD}%"
    DIFF=$(echo "$COVERAGE_THRESHOLD - $COVERAGE_PERCENT" | bc -l)
    print_info "Need ${DIFF}% more coverage"
fi

# Show report location
echo ""
print_info "Coverage report location:"
echo "  HTML Report: ${COVERAGE_DIR}/html/index.html"
echo "  Raw Data: ${COVERAGE_DIR}/coverage_filtered.info"

# Open in browser if requested
if [ "$OPEN_BROWSER" = true ]; then
    print_info "Opening coverage report in browser..."
    if command -v xdg-open &> /dev/null; then
        xdg-open "${COVERAGE_DIR}/html/index.html" &
    elif command -v firefox &> /dev/null; then
        firefox "${COVERAGE_DIR}/html/index.html" &
    elif command -v google-chrome &> /dev/null; then
        google-chrome "${COVERAGE_DIR}/html/index.html" &
    else
        print_warning "Could not detect browser to open report"
        print_info "Manually open: ${COVERAGE_DIR}/html/index.html"
    fi
fi

echo ""
print_success "Coverage analysis completed!"
echo ""
echo "To view the report:"
echo "  xdg-open ${COVERAGE_DIR}/html/index.html"
echo "Or navigate to: file://${COVERAGE_DIR}/html/index.html"
echo ""
