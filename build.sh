#!/bin/bash
#
# build.sh - Build script for ApraUtils library
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

# Default options
BUILD_TYPE="Release"
BUILD_TESTS=OFF
ENABLE_COVERAGE=OFF
CLEAN_BUILD=false
INSTALL=false
JOBS=$(nproc 2>/dev/null || echo 4)

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

Build script for ApraUtils library.

OPTIONS:
    -h, --help              Show this help message
    -d, --debug             Build in Debug mode (default: Release)
    -t, --tests             Build with unit tests
    -c, --coverage          Enable code coverage (implies --tests and --debug)
    -j, --jobs N            Number of parallel build jobs (default: $(nproc))
    --clean                 Clean build directory before building
    --install               Install library after building (requires sudo)

EXAMPLES:
    $0                      # Build release library
    $0 --debug              # Build debug library
    $0 --tests              # Build with tests
    $0 --coverage           # Build with coverage enabled
    $0 --clean --tests      # Clean build with tests
    $0 -j 8                 # Build with 8 parallel jobs

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_usage
            exit 0
            ;;
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -t|--tests)
            BUILD_TESTS=ON
            shift
            ;;
        -c|--coverage)
            BUILD_TESTS=ON
            ENABLE_COVERAGE=ON
            BUILD_TYPE="Debug"
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --install)
            INSTALL=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Check if running on Linux
if [[ "$OSTYPE" != "linux-gnu"* ]]; then
    print_error "ApraUtils is designed for Linux systems only"
    print_info "Current OS: $OSTYPE"
    exit 1
fi

# Check for required tools
print_info "Checking for required tools..."

if ! command -v cmake &> /dev/null; then
    print_error "CMake is not installed"
    print_info "Install with: sudo apt-get install cmake"
    exit 1
fi

if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    print_error "No C++ compiler found (g++ or clang++)"
    print_info "Install with: sudo apt-get install build-essential"
    exit 1
fi

# Check for libudev-dev
if ! pkg-config --exists libudev 2>/dev/null; then
    print_warning "libudev-dev may not be installed"
    print_info "Install with: sudo apt-get install libudev-dev"
fi

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
    print_info "Cleaning build directory..."
    rm -rf "${BUILD_DIR}"
    print_success "Build directory cleaned"
fi

# Create build directory
print_info "Creating build directory..."
mkdir -p "${BUILD_DIR}"

# Print build configuration
print_info "Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Build Tests: $BUILD_TESTS"
echo "  Code Coverage: $ENABLE_COVERAGE"
echo "  Parallel Jobs: $JOBS"
echo "  Build Directory: $BUILD_DIR"
echo ""

# Configure with CMake
print_info "Configuring with CMake..."
cd "${BUILD_DIR}"

cmake .. \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DBUILD_TESTS="${BUILD_TESTS}" \
    -DENABLE_COVERAGE="${ENABLE_COVERAGE}" \
    || {
        print_error "CMake configuration failed"
        exit 1
    }

print_success "CMake configuration completed"

# Build
print_info "Building ApraUtils..."
make -j"${JOBS}" || {
    print_error "Build failed"
    exit 1
}

print_success "Build completed successfully"

# Show build artifacts
print_info "Build artifacts:"
if [ -f "${BUILD_DIR}/libApraUtils.a" ]; then
    echo "  Library: ${BUILD_DIR}/libApraUtils.a"
    ls -lh "${BUILD_DIR}/libApraUtils.a"
fi

if [ "$BUILD_TESTS" = "ON" ] && [ -f "${BUILD_DIR}/ApraUtils_tests" ]; then
    echo "  Tests: ${BUILD_DIR}/ApraUtils_tests"
    ls -lh "${BUILD_DIR}/ApraUtils_tests"
fi

# Install if requested
if [ "$INSTALL" = true ]; then
    print_info "Installing ApraUtils..."
    if [ "$EUID" -ne 0 ]; then
        print_warning "Installation requires root privileges"
        sudo make install || {
            print_error "Installation failed"
            exit 1
        }
    else
        make install || {
            print_error "Installation failed"
            exit 1
        }
    fi
    print_success "Installation completed"
fi

# Print next steps
echo ""
print_success "ApraUtils build completed!"
echo ""
echo "Next steps:"
if [ "$BUILD_TESTS" = "ON" ]; then
    echo "  - Run tests: ./test.sh"
    if [ "$ENABLE_COVERAGE" = "ON" ]; then
        echo "  - Generate coverage: ./coverage.sh"
    fi
else
    echo "  - Run tests: ./build.sh --tests && ./test.sh"
fi
echo "  - View library: ls -lh ${BUILD_DIR}/libApraUtils.a"
echo "  - Install: ./build.sh --install"
echo ""
