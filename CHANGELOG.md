# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Unit test coverage
- Additional hardware interface support (SPI, UART)
- Package distribution (apt, Conan, vcpkg)
- Performance benchmarking suite
- Comprehensive Doxygen documentation
- Code coverage reporting

## [1.0.0] - Initial Open Source Release

### Added
- **Core Hardware Interfaces**
  - GPIO control with interrupt support (RISING, FALLING, BOTH edges)
  - I2C asynchronous transaction management with error handling
  - PWM control with nanosecond precision
  - USB storage device detection and management

- **Utilities**
  - ProcessThread for multi-threaded applications with message passing
  - Mutex and ScopeLock for thread synchronization
  - FileIO utilities for embedded systems
  - RealHexParser for numeric conversions
  - Utils class with common utility functions
  - ScopeFunction for function tracing
  - Macro definitions for timing and utilities

- **Data Models**
  - Message and I2CMessage for inter-thread communication
  - I2CTransactionMessage for complex I2C operations
  - GenericError and I2CError for comprehensive error reporting
  - Range for numeric range management
  - StorageMinimalInfo for USB storage information

- **Constants and Enums**
  - I2CMessageType (READ, WRITE, READ_COMPARE)
  - MessageType (REQUEST_ONLY, REQUEST_RESPONSE)
  - ThreadType (FREERUNNING, MESSAGE_AND_FREERUNNING, ONLY_MESSAGE)
  - StorageType (FAT32, NTFS, EXT4)
  - StorageState (INSERTED, MOUNTED, SAFE_EJECT, UNSAFE_EJECT)
  - EventCallbacks for I2C transaction callbacks

- **Build System**
  - CMake build configuration (CMakeLists.txt)
  - C++14 standard support
  - Static library output (libApraLinuxUtils.a)
  - GitHub Actions CI/CD pipeline

- **Documentation**
  - Comprehensive README.md with badges and examples
  - CONTRIBUTING.md with development guidelines
  - CODE_OF_CONDUCT.md (Contributor Covenant v2.1)
  - SECURITY.md with vulnerability reporting process
  - MIT License

- **Examples**
  - GPIO control examples (gpio_example.cpp)
  - I2C communication examples (i2c_example.cpp)
  - PWM control examples (pwm_example.cpp)
  - USB storage management examples (usb_storage_example.cpp)
  - Multi-threading examples (threading_example.cpp)
  - Examples README with usage instructions

- **GitHub Templates**
  - Bug report issue template
  - Feature request issue template
  - Pull request template

- **API Documentation Setup**
  - Doxygen configuration (Doxyfile)
  - Source code documentation structure

### Changed
- Updated all source file headers with Apra Labs copyright
- Standardized author attribution to "Apra Labs"
- Cleaned up auto-generated TODO comments
- Improved code organization and structure

### Fixed
- Fixed missing platform check in I2CInterface
- Added missing header includes (cstdint, stdexcept, time.h)
- Corrected protected member access in I2CInterface
- Improved error handling in I2C bus operations

### Platform Support
- **Operating Systems**: Linux (embedded systems focus)
- **Architectures**: ARM, x86, x86_64
- **Tested On**: Ubuntu 22.04, Raspberry Pi OS
- **Compiler Support**: GCC 5.0+, Clang 3.8+

### Dependencies
- pthread (POSIX threads)
- libudev-dev (USB device enumeration)
- Linux kernel headers (I2C, GPIO, PWM interfaces)
- CMake 3.10+

### Breaking Changes
- None (initial release)

### Security
- No known vulnerabilities
- Proper input validation in all public APIs
- Thread-safe operations using provided synchronization primitives

### Performance
- Asynchronous I2C transactions for non-blocking operations
- Efficient GPIO interrupt handling
- Optimized memory usage with RAII patterns

### Known Issues
- None currently reported

### Contributors
- Apra Labs development team

---

## Version History Format

### [Version] - YYYY-MM-DD

#### Added
- New features and capabilities

#### Changed
- Changes to existing functionality

#### Deprecated
- Soon-to-be removed features

#### Removed
- Removed features

#### Fixed
- Bug fixes

#### Security
- Security fixes and improvements

---

## Links

- [Repository](https://github.com/Apra-Labs/ApraLinuxUtils)
- [Issues](https://github.com/Apra-Labs/ApraLinuxUtils/issues)
- [Pull Requests](https://github.com/Apra-Labs/ApraLinuxUtils/pulls)
- [Releases](https://github.com/Apra-Labs/ApraLinuxUtils/releases)

---

**Note**: This project follows [Semantic Versioning](https://semver.org/):
- MAJOR version for incompatible API changes
- MINOR version for backwards-compatible new functionality
- PATCH version for backwards-compatible bug fixes
