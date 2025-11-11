# Contributing to ApraLinuxUtils

Thank you for considering contributing to ApraLinuxUtils! We welcome contributions from the community and are grateful for your support.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements](#suggesting-enhancements)
  - [Code Contributions](#code-contributions)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Commit Message Guidelines](#commit-message-guidelines)
- [Pull Request Process](#pull-request-process)
- [Testing](#testing)
- [Documentation](#documentation)

## Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code. Please report unacceptable behavior to the project maintainers.

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check the [existing issues](https://github.com/Apra-Labs/ApraLinuxUtils/issues) to avoid duplicates.

When creating a bug report, please include:

- **Clear title and description**: Describe what you expected to happen and what actually happened
- **Steps to reproduce**: Provide specific examples to reproduce the issue
- **Environment details**:
  - OS version and distribution (e.g., Ubuntu 22.04, Raspbian Bullseye)
  - Compiler version (e.g., GCC 11.2)
  - CMake version
  - Hardware platform if relevant
- **Code sample**: Minimal reproducible example if possible
- **Error messages**: Complete error output or logs
- **Screenshots**: If applicable

**Template:**
```markdown
**Description:**
Brief description of the issue

**Steps to Reproduce:**
1. Step 1
2. Step 2
3. ...

**Expected Behavior:**
What should happen

**Actual Behavior:**
What actually happens

**Environment:**
- OS: Ubuntu 22.04
- Compiler: GCC 11.2
- CMake: 3.22.1
- Hardware: Raspberry Pi 4

**Additional Context:**
Any other relevant information
```

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion:

- **Use a clear and descriptive title**
- **Provide a detailed description** of the proposed functionality
- **Explain why this enhancement would be useful** to most ApraLinuxUtils users
- **List any similar features** in other libraries
- **Include code examples** if applicable

### Code Contributions

We actively welcome your pull requests! Here's how to contribute code:

1. Fork the repository
2. Create your feature branch
3. Make your changes
4. Test your changes
5. Commit your changes
6. Push to your fork
7. Submit a pull request

## Development Setup

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y build-essential cmake git libudev-dev

# Additional tools for development
sudo apt-get install -y clang-format clang-tidy doxygen graphviz
```

### Setting Up Your Development Environment

```bash
# 1. Fork and clone the repository
git clone https://github.com/YOUR_USERNAME/ApraLinuxUtils.git
cd ApraLinuxUtils

# 2. Add upstream remote
git remote add upstream https://github.com/Apra-Labs/ApraLinuxUtils.git

# 3. Create a development branch
git checkout -b feature/your-feature-name

# 4. Build the project
mkdir build && cd build
cmake ..
make

# 5. Make your changes and test
# Edit files...
make
# Run tests...

# 6. Commit and push
git add .
git commit -m "Add feature: description"
git push origin feature/your-feature-name
```

### Keeping Your Fork Updated

```bash
git fetch upstream
git checkout main
git merge upstream/main
git push origin main
```

## Coding Standards

### C++ Standards

- **Language Standard**: C++14
- **Compiler Compatibility**: GCC 5.0+, Clang 3.8+
- **Platform**: Linux only

### Code Style

We follow a consistent coding style to maintain readability:

#### Naming Conventions

```cpp
// Namespaces: lowercase
namespace apra {

// Classes: PascalCase
class ProcessThread {

// Public member variables: m_ prefix with camelCase
public:
    uint64_t m_handle;

// Private member variables: m_ prefix with camelCase
private:
    std::string m_functionName;

// Methods: camelCase
    void processMessage();

// Constants: UPPER_CASE
enum THREAD_TYPE {
    FREERUNNING,
    ONLY_MESSAGE
};

// Function parameters: camelCase
void setType(MESSAGE_TYPE messageType);

}  // namespace apra
```

#### Formatting

- **Indentation**: Tabs (as per existing codebase)
- **Braces**: Opening brace on same line for functions, new line for classes
- **Line Length**: Prefer 80-100 characters, max 120
- **Spaces**: Space after keywords, around operators

Example:
```cpp
void MyClass::myFunction(int param1, bool param2)
{
    if (param2)
    {
        doSomething(param1);
    }
    else
    {
        doSomethingElse();
    }
}
```

#### Header Files

```cpp
/*
 * FileName.h
 *
 * Copyright (c) 2024 Apra Labs
 *
 * This file is part of ApraLinuxUtils.
 *
 * Licensed under the MIT License.
 * See LICENSE file in the project root for full license information.
 */

#ifndef INCLUDES_APRA_FILENAME_H_
#define INCLUDES_APRA_FILENAME_H_

// System includes first
#include <string>
#include <vector>

// Local includes second
#include "models/Message.h"

namespace apra
{

class ClassName
{
public:
    ClassName();
    virtual ~ClassName();

private:
    int m_memberVariable;
};

}  // namespace apra

#endif /* INCLUDES_APRA_FILENAME_H_ */
```

### Best Practices

1. **RAII (Resource Acquisition Is Initialization)**: Use RAII for resource management
2. **Const Correctness**: Use `const` wherever appropriate
3. **Error Handling**: Use the GenericError/I2CError classes for error reporting
4. **Thread Safety**: Use Mutex and ScopeLock for thread-safe operations
5. **Memory Management**: Avoid raw pointers; prefer smart pointers when needed
6. **Comments**: Document complex logic; avoid obvious comments

```cpp
// Good
// Calculate the average delay between I2C transactions to prevent bus saturation
uint64_t averageDelay = totalDelay / transactionCount;

// Bad
// Set x to 5
int x = 5;
```

## Commit Message Guidelines

We follow conventional commit message format:

### Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, no code change)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Maintenance tasks

### Examples

```
feat(gpio): add support for GPIO edge detection

Add support for detecting rising, falling, and both edge interrupts
on GPIO pins using the Linux sysfs interface.

Closes #123
```

```
fix(i2c): handle bus busy error correctly

Previously, bus busy errors were not properly handled, causing
transaction failures. This fix adds retry logic with exponential
backoff.

Fixes #456
```

### Guidelines

- Use present tense ("add feature" not "added feature")
- Use imperative mood ("move cursor to..." not "moves cursor to...")
- Limit first line to 72 characters
- Reference issues and pull requests in the footer
- Explain **what** and **why**, not **how**

## Pull Request Process

### Before Submitting

1. ✅ **Test your changes**: Ensure all existing tests pass and add new tests for new features
2. ✅ **Follow coding standards**: Check code style and formatting
3. ✅ **Update documentation**: Update README.md, comments, and docs as needed
4. ✅ **Rebase on latest main**: Ensure your branch is up to date
5. ✅ **Check for conflicts**: Resolve any merge conflicts

### PR Checklist

```markdown
- [ ] Code follows the project's coding standards
- [ ] Self-review of code performed
- [ ] Comments added for complex or unclear code
- [ ] Documentation updated (README, API docs, etc.)
- [ ] No new compiler warnings introduced
- [ ] Tests added/updated and all tests pass
- [ ] Commit messages follow guidelines
- [ ] Changes are focused and atomic (one feature/fix per PR)
```

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Testing
Describe how you tested these changes:
- Test case 1
- Test case 2

## Related Issues
Fixes #(issue number)
Related to #(issue number)

## Additional Notes
Any additional information
```

### Review Process

1. At least one maintainer must approve the PR
2. All CI/CD checks must pass
3. Code review comments must be addressed
4. Once approved, a maintainer will merge the PR

## Testing

### Running Tests

```bash
# Build with tests enabled
cmake -DBUILD_TESTS=ON ..
make

# Run all tests
ctest

# Run specific test
./build/test_gpio
```

### Writing Tests

- Add tests for all new features
- Add tests for all bug fixes
- Ensure tests are deterministic
- Mock hardware interfaces when testing on non-embedded systems
- Use descriptive test names

Example:
```cpp
TEST(GPIOTest, InitializeOutputPin) {
    apra::GPIO gpio(23);
    ASSERT_TRUE(gpio.Init(false));
    // Additional assertions...
}
```

## Documentation

### Code Documentation

- Document all public APIs using Doxygen comments
- Include parameter descriptions
- Include return value descriptions
- Provide usage examples

Example:
```cpp
/**
 * @brief Initializes the GPIO pin for input or output
 *
 * @param isRead true for input mode, false for output mode
 * @return true if initialization successful, false otherwise
 *
 * @example
 * GPIO led(23);
 * led.Init(false);  // Initialize as output
 */
bool Init(bool isRead);
```

### README and Guides

- Update README.md for user-facing changes
- Add examples to examples/ directory
- Update API documentation in docs/

## Questions?

If you have questions about contributing:

- Open a [GitHub Discussion](https://github.com/Apra-Labs/ApraLinuxUtils/discussions)
- Check existing [Issues](https://github.com/Apra-Labs/ApraLinuxUtils/issues)
- Review [Documentation](https://apra-labs.github.io/ApraLinuxUtils/)

## License

By contributing to ApraLinuxUtils, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to ApraLinuxUtils! Your efforts help make this project better for everyone.
