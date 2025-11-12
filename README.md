# ApraLinuxUtils

[![Build Status](https://github.com/Apra-Labs/ApraLinuxUtils/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/Apra-Labs/ApraLinuxUtils/actions/workflows/c-cpp.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++14](https://img.shields.io/badge/C++-14-blue.svg)](https://isocpp.org/std/the-standard)
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)](https://www.linux.org/)
[![codecov](https://codecov.io/gh/Apra-Labs/ApraLinuxUtils/branch/main/graph/badge.svg)](https://codecov.io/gh/Apra-Labs/ApraLinuxUtils)

A comprehensive C++ utility library designed for embedded Linux systems, providing hardware interface abstractions and utilities for I2C, GPIO, PWM, USB storage, and multi-threaded applications.

## Features

### Hardware Interfaces
- **I2C Communication**: Asynchronous I2C transaction support with error handling
- **GPIO Control**: Flexible GPIO management with interrupt support
- **PWM Control**: Pulse Width Modulation control for motor and LED applications
- **USB Storage**: USB device detection and management

### System Utilities
- **Thread Management**: Process threading with message passing capabilities
- **Synchronization**: Mutex and scope lock utilities for thread-safe operations
- **File I/O**: Enhanced file operations for embedded systems
- **Error Handling**: Comprehensive error reporting system

### Data Structures
- **Message Passing**: Type-safe message queue system
- **Range Management**: Numeric range handling with reverse support
- **Hex Parsing**: Real number to hexadecimal conversion utilities

## Table of Contents

- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Usage Examples](#usage-examples)
- [API Documentation](#api-documentation)
- [Building from Source](#building-from-source)
- [Contributing](#contributing)
- [License](#license)
- [Support](#support)

## Requirements

### Platform
- **Operating System**: Linux (embedded systems focus)
- **Architecture**: ARM, x86, x86_64

### Dependencies
- **C++ Compiler**: GCC 5.0+ or Clang 3.8+ (C++14 support required)
- **CMake**: 3.10 or higher
- **System Libraries**:
  - `pthread` (POSIX threads)
  - `libudev-dev` (USB device enumeration)
  - Linux kernel headers (I2C, GPIO, PWM interfaces)

### System Access Requirements
The library requires access to the following system interfaces:
- `/sys/class/gpio` (GPIO sysfs interface)
- `/sys/class/pwm` (PWM sysfs interface)
- `/dev/i2c-*` (I2C device nodes)
- `udev` for USB device detection

## Installation

### Using Pre-built Library

Download the latest release from the [Releases](https://github.com/Apra-Labs/ApraLinuxUtils/releases) page.

### From Source

#### Quick Build

```bash
# Clone the repository
git clone https://github.com/Apra-Labs/ApraLinuxUtils.git
cd ApraLinuxUtils

# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y cmake libudev-dev build-essential

# Build the library using the build script
./build.sh

# The static library will be available at build/libApraLinuxUtils.a
```

#### Build with Tests

```bash
# Build with unit tests
./build.sh --tests

# Run the tests
./test.sh
```

#### Build with Code Coverage

```bash
# Install coverage tools
sudo apt-get install -y lcov

# Build with coverage enabled
./build.sh --coverage

# Run tests
./test.sh

# Generate coverage report
./coverage.sh

# Open coverage report in browser
./coverage.sh --open
```

## Quick Start

### Including in Your Project

#### CMake

```cmake
# In your CMakeLists.txt
include_directories(/path/to/ApraLinuxUtils/includes)
link_directories(/path/to/ApraLinuxUtils/build)

add_executable(your_app main.cpp)
target_link_libraries(your_app ApraLinuxUtils pthread udev)
```

#### Manual Compilation

```bash
g++ -std=c++14 your_app.cpp -I/path/to/ApraLinuxUtils/includes \
    -L/path/to/ApraLinuxUtils/build -lApraLinuxUtils -lpthread -ludev -o your_app
```

### Basic Usage

```cpp
#include <ApraUtils.h>

int main() {
    // GPIO example
    apra::GPIO led(23);  // GPIO pin 23
    led.Init(false);     // Initialize as output
    led.SetValue(true);  // Turn on LED

    // I2C example
    apra::I2C_Interface i2c("/dev/i2c-1", "I2C_Thread", 100);
    i2c.start();

    return 0;
}
```

## Usage Examples

Comprehensive examples are available in the [examples/](examples/) directory:

- [GPIO Control](examples/gpio_example.cpp) - Digital I/O and interrupts
- [I2C Communication](examples/i2c_example.cpp) - Reading from I2C sensors
- [PWM Control](examples/pwm_example.cpp) - LED dimming and motor control
- [USB Storage](examples/usb_storage_example.cpp) - Device detection and monitoring
- [Threading](examples/threading_example.cpp) - Multi-threaded applications

## API Documentation

### Core Components

#### GPIO
```cpp
apra::GPIO gpio(pin_number);
gpio.Init(is_read);                           // Initialize pin
gpio.Init4EdgeInterrupt(is_read, edge_type);  // Setup interrupt
gpio.SetValue(value);                         // Set output value
bool value = gpio.GetValue();                 // Read input value
```

#### I2C Interface
```cpp
apra::I2C_Interface i2c(device_path, thread_name, frequency_hz);
i2c.start();                                  // Start I2C thread
i2c.queueMessage(transaction_message);        // Queue I2C operation
i2c.stop();                                   // Stop I2C thread
```

#### PWM
```cpp
apra::PWM pwm(chip_number, channel_number);
pwm.Init();                                   // Initialize PWM
pwm.SetDutyCycle(duty_cycle_ns, period_ns);  // Set duty cycle
pwm.Enable(true);                             // Enable PWM output
```

#### Process Thread
```cpp
class MyThread : public apra::ProcessThread {
protected:
    void ProcessMessage(apra::Message* msg) override {
        // Handle incoming messages
    }

    void Process() override {
        // Main processing loop
    }
};
```

For complete API documentation, see the [API Reference](docs/API.md).

## Building from Source

### Using Build Scripts (Recommended)

ApraUtils provides convenient build scripts for easy compilation:

#### Standard Build
```bash
./build.sh                    # Build release library
./build.sh --debug            # Build debug library
./build.sh --clean            # Clean build
./build.sh -j 8               # Build with 8 parallel jobs
```

#### Build with Tests
```bash
./build.sh --tests            # Build with unit tests
./test.sh                     # Run all tests
./test.sh --verbose           # Run tests with verbose output
./test.sh --filter "Range*"   # Run specific tests
./test.sh --list              # List all available tests
```

#### Build with Code Coverage
```bash
./build.sh --coverage         # Build with coverage enabled
./test.sh                     # Run tests
./coverage.sh                 # Generate coverage report
./coverage.sh --open          # Generate and open in browser
./coverage.sh --threshold 90  # Check 90% coverage threshold
```

### Build Options (Manual CMake)

For manual builds or integration with existing build systems:

```bash
# Standard build
mkdir build && cd build
cmake ..
make

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Build with tests
cmake -DBUILD_TESTS=ON ..
make
ctest

# Build with coverage
cmake -DBUILD_TESTS=ON -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
make
./ApraUtils_tests
```

### Build Script Options

**build.sh options:**
- `-h, --help` - Show help message
- `-d, --debug` - Build in Debug mode
- `-t, --tests` - Build with unit tests
- `-c, --coverage` - Enable code coverage
- `-j N, --jobs N` - Number of parallel jobs
- `--clean` - Clean build directory first
- `--install` - Install after building

**test.sh options:**
- `-h, --help` - Show help message
- `-v, --verbose` - Verbose test output
- `-f PATTERN, --filter PATTERN` - Run specific tests
- `-r N, --repeat N` - Repeat tests N times
- `-l, --list` - List all tests
- `--ctest` - Use CTest runner

**coverage.sh options:**
- `-h, --help` - Show help message
- `-o, --open` - Open report in browser
- `-t N, --threshold N` - Set coverage threshold
- `--clean` - Clean previous coverage data

## Project Structure

```
ApraLinuxUtils/
├── includes/           # Public header files
│   ├── constants/     # Enum definitions
│   ├── controllers/   # Controller classes
│   ├── models/        # Data models
│   └── utils/         # Utility classes
├── src/               # Implementation files
│   ├── constants/
│   ├── controllers/
│   ├── models/
│   └── utils/
├── tests/             # Unit tests
│   ├── unit/          # Unit test files
│   ├── mocks/         # Mock objects for testing
│   └── main_test.cpp  # Test entry point
├── examples/          # Usage examples
├── .github/
│   ├── workflows/     # CI/CD configurations
│   └── ISSUE_TEMPLATE/  # Issue templates
├── build.sh           # Build script
├── test.sh            # Test script
├── coverage.sh        # Coverage script
├── CMakeLists.txt     # Build configuration
└── LICENSE            # MIT License
```

## Contributing

We welcome contributions! Please see our [Contributing Guidelines](CONTRIBUTING.md) for details on:
- Code style and standards
- Development workflow
- Submitting pull requests
- Reporting bugs

## Code of Conduct

This project adheres to a [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## Versioning

We use [Semantic Versioning](https://semver.org/). For available versions, see the [tags on this repository](https://github.com/Apra-Labs/ApraLinuxUtils/tags).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Copyright (c) 2024 Apra Labs

## Support

- **Documentation**: [GitHub Pages](https://apra-labs.github.io/ApraLinuxUtils/) (coming soon)
- **Issues**: [GitHub Issues](https://github.com/Apra-Labs/ApraLinuxUtils/issues)
- **Discussions**: [GitHub Discussions](https://github.com/Apra-Labs/ApraLinuxUtils/discussions)

## Acknowledgments

- Built for embedded Linux systems
- Designed for reliability and performance in production environments
- Used in various Apra Labs products and solutions

## Roadmap

- [ ] Unit test coverage
- [ ] Doxygen API documentation
- [ ] Additional hardware interface support (SPI, UART)
- [ ] Package distribution (apt, Conan, vcpkg)
- [ ] Comprehensive examples and tutorials
- [ ] Performance benchmarking suite

---

**Made with ❤️ by [Apra Labs](https://github.com/Apra-Labs)**
