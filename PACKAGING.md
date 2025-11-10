# Packaging Guide for ApraUtils

This guide provides instructions for packaging ApraUtils for various distribution channels.

## Table of Contents

- [Prerequisites](#prerequisites)
- [CMake Install Targets](#cmake-install-targets)
- [Debian Package (.deb)](#debian-package-deb)
- [RPM Package (.rpm)](#rpm-package-rpm)
- [Conan Package](#conan-package)
- [vcpkg Package](#vcpkg-package)

---

## Prerequisites

Before packaging, ensure you have:
- CMake 3.10 or higher
- GCC 5.0+ or Clang 3.8+
- libudev-dev installed
- Linux kernel headers
- Git

---

## CMake Install Targets

ApraUtils uses CMake for building. To add install targets, update `CMakeLists.txt`:

```cmake
# Installation configuration
include(GNUInstallDirs)

# Install library
install(TARGETS ApraUtils
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/includes/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ApraUtils
    FILES_MATCHING PATTERN "*.h"
)

# Install CMake package configuration
install(FILES ApraUtilsConfig.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ApraUtils
)

# Create pkg-config file
configure_file(ApraUtils.pc.in ApraUtils.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/ApraUtils.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)
```

### Building with Install Target

```bash
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
sudo make install
```

---

## Debian Package (.deb)

### Method 1: Using CPack

1. **Add CPack configuration to CMakeLists.txt:**

```cmake
# CPack configuration for Debian packaging
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "libaprautils")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "Apra Labs <contact@apralabs.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Hardware interface library for embedded Linux")
set(CPACK_PACKAGE_DESCRIPTION "ApraUtils is a comprehensive C++ library for embedded Linux hardware interfaces including GPIO, I2C, PWM, and USB storage management.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libudev1, libc6 (>= 2.27)")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Apra-Labs/ApraUtils")

include(CPack)
```

2. **Build the package:**

```bash
mkdir build && cd build
cmake ..
make
cpack -G DEB
```

The `.deb` package will be created in the build directory.

### Method 2: Using debuild

1. **Create debian directory structure:**

```bash
mkdir -p debian/source
```

2. **Create `debian/control`:**

```
Source: libaprautils
Section: libs
Priority: optional
Maintainer: Apra Labs <contact@apralabs.com>
Build-Depends: debhelper (>= 11), cmake (>= 3.10), libudev-dev
Standards-Version: 4.5.0
Homepage: https://github.com/Apra-Labs/ApraUtils

Package: libaprautils1
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}, libudev1
Description: Hardware interface library for embedded Linux
 ApraUtils is a comprehensive C++ library for embedded Linux hardware
 interfaces including GPIO, I2C, PWM, and USB storage management.

Package: libaprautils-dev
Section: libdevel
Architecture: any
Depends: libaprautils1 (= ${binary:Version}), ${misc:Depends}
Description: Development files for ApraUtils
 Development files (headers) for ApraUtils library.
```

3. **Create `debian/rules`:**

```makefile
#!/usr/bin/make -f

%:
	dh $@ --buildsystem=cmake

override_dh_auto_configure:
	dh_auto_configure -- -DCMAKE_BUILD_TYPE=Release
```

4. **Create `debian/changelog`:**

```
libaprautils (1.0.0-1) unstable; urgency=low

  * Initial release

 -- Apra Labs <contact@apralabs.com>  Sun, 10 Nov 2025 00:00:00 +0000
```

5. **Create `debian/compat`:**

```
11
```

6. **Create `debian/source/format`:**

```
3.0 (native)
```

7. **Build the package:**

```bash
debuild -us -uc -b
```

### Installing the .deb Package

```bash
sudo dpkg -i libaprautils1_1.0.0-1_amd64.deb libaprautils-dev_1.0.0-1_amd64.deb
```

---

## RPM Package (.rpm)

### Using CPack

1. **Add RPM configuration to CMakeLists.txt:**

```cmake
# CPack configuration for RPM packaging
set(CPACK_GENERATOR "RPM")
set(CPACK_RPM_PACKAGE_NAME "libaprautils")
set(CPACK_RPM_PACKAGE_VERSION "1.0.0")
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_URL "https://github.com/Apra-Labs/ApraUtils")
set(CPACK_RPM_PACKAGE_DESCRIPTION "ApraUtils is a comprehensive C++ library for embedded Linux hardware interfaces including GPIO, I2C, PWM, and USB storage management.")
set(CPACK_RPM_PACKAGE_REQUIRES "systemd-libs >= 219")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")

include(CPack)
```

2. **Build the package:**

```bash
mkdir build && cd build
cmake ..
make
cpack -G RPM
```

### Manual RPM Creation

1. **Create RPM spec file `aprautils.spec`:**

```spec
Name:           libaprautils
Version:        1.0.0
Release:        1%{?dist}
Summary:        Hardware interface library for embedded Linux

License:        MIT
URL:            https://github.com/Apra-Labs/ApraUtils
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.10
BuildRequires:  gcc-c++
BuildRequires:  systemd-devel
Requires:       systemd-libs >= 219

%description
ApraUtils is a comprehensive C++ library for embedded Linux hardware
interfaces including GPIO, I2C, PWM, and USB storage management.

%package devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
Development files (headers) for ApraUtils library.

%prep
%setup -q

%build
mkdir build
cd build
%cmake ..
make %{?_smp_mflags}

%install
cd build
%make_install

%files
%license LICENSE
%doc README.md
%{_libdir}/libApraUtils.a

%files devel
%{_includedir}/ApraUtils/

%changelog
* Sun Nov 10 2025 Apra Labs <contact@apralabs.com> - 1.0.0-1
- Initial RPM release
```

2. **Build the RPM:**

```bash
rpmbuild -ba aprautils.spec
```

### Installing the .rpm Package

```bash
sudo rpm -ivh libaprautils-1.0.0-1.x86_64.rpm libaprautils-devel-1.0.0-1.x86_64.rpm
```

---

## Conan Package

Conan is a popular C/C++ package manager.

### Creating a Conan Package

1. **Create `conanfile.py` in the project root:**

```python
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.files import copy

class ApraUtilsConan(ConanFile):
    name = "aprautils"
    version = "1.0.0"
    license = "MIT"
    author = "Apra Labs <contact@apralabs.com>"
    url = "https://github.com/Apra-Labs/ApraUtils"
    description = "Hardware interface library for embedded Linux"
    topics = ("embedded", "linux", "gpio", "i2c", "pwm", "hardware")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    exports_sources = "CMakeLists.txt", "src/*", "includes/*", "LICENSE"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE", src=self.source_folder, dst=self.package_folder)
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["ApraUtils"]
        self.cpp_info.system_libs = ["pthread", "udev"]
        self.cpp_info.set_property("cmake_find_mode", "both")
        self.cpp_info.set_property("cmake_file_name", "ApraUtils")
        self.cpp_info.set_property("cmake_target_name", "ApraUtils::ApraUtils")
        self.cpp_info.set_property("pkg_config_name", "ApraUtils")

    def validate(self):
        if self.settings.os != "Linux":
            raise ConanInvalidConfiguration("ApraUtils only supports Linux")
```

2. **Create the package:**

```bash
conan create . --build=missing
```

3. **Upload to Conan Center (optional):**

First, create a fork of [conan-center-index](https://github.com/conan-io/conan-center-index).

```bash
# Clone conan-center-index
git clone https://github.com/conan-io/conan-center-index.git
cd conan-center-index

# Create recipe directory
mkdir -p recipes/aprautils/all

# Copy conanfile.py and create config.yml
cp /path/to/conanfile.py recipes/aprautils/all/
```

Create `recipes/aprautils/config.yml`:

```yaml
versions:
  "1.0.0":
    folder: all
```

Submit a pull request to Conan Center Index.

### Using ApraUtils from Conan

In your `conanfile.txt`:

```ini
[requires]
aprautils/1.0.0

[generators]
CMakeDeps
CMakeToolchain
```

Or in your `conanfile.py`:

```python
def requirements(self):
    self.requires("aprautils/1.0.0")
```

---

## vcpkg Package

vcpkg is Microsoft's C/C++ package manager.

### Adding to vcpkg

1. **Fork vcpkg repository:**

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
```

2. **Create port directory:**

```bash
mkdir -p ports/aprautils
```

3. **Create `ports/aprautils/portfile.cmake`:**

```cmake
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO Apra-Labs/ApraUtils
    REF v1.0.0
    SHA512 <sha512-of-tarball>
    HEAD_REF main
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/ApraUtils)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

vcpkg_copy_pdbs()
```

4. **Create `ports/aprautils/vcpkg.json`:**

```json
{
  "name": "aprautils",
  "version": "1.0.0",
  "description": "Hardware interface library for embedded Linux",
  "homepage": "https://github.com/Apra-Labs/ApraUtils",
  "license": "MIT",
  "supports": "linux",
  "dependencies": [
    {
      "name": "vcpkg-cmake",
      "host": true
    },
    {
      "name": "vcpkg-cmake-config",
      "host": true
    }
  ]
}
```

5. **Update versions database:**

```bash
cd vcpkg
git add ports/aprautils
./vcpkg x-add-version aprautils
```

6. **Test the port:**

```bash
./vcpkg install aprautils
```

7. **Submit to vcpkg:**

Create a pull request to the [vcpkg repository](https://github.com/microsoft/vcpkg).

### Using ApraUtils from vcpkg

```bash
# Install vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh

# Install ApraUtils
./vcpkg install aprautils

# Use in CMake
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

In your `CMakeLists.txt`:

```cmake
find_package(ApraUtils CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE ApraUtils::ApraUtils)
```

---

## Additional Files Needed

### ApraUtilsConfig.cmake

Create `ApraUtilsConfig.cmake.in`:

```cmake
@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/ApraUtilsTargets.cmake")

check_required_components(ApraUtils)
```

### ApraUtils.pc.in (pkg-config)

Create `ApraUtils.pc.in`:

```
prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${prefix}/@CMAKE_INSTALL_LIBDIR@
includedir=${prefix}/@CMAKE_INSTALL_INCLUDEDIR@

Name: ApraUtils
Description: Hardware interface library for embedded Linux
Version: @PROJECT_VERSION@
Requires:
Libs: -L${libdir} -lApraUtils
Libs.private: -lpthread -ludev
Cflags: -I${includedir}/ApraUtils
```

---

## Testing Packages

### Test .deb Package

```bash
# Install
sudo dpkg -i libaprautils1_1.0.0-1_amd64.deb libaprautils-dev_1.0.0-1_amd64.deb

# Verify installation
dpkg -L libaprautils1
dpkg -L libaprautils-dev

# Test compilation
g++ -o test test.cpp -lApraUtils -lpthread -ludev

# Remove
sudo apt-get remove libaprautils1 libaprautils-dev
```

### Test .rpm Package

```bash
# Install
sudo rpm -ivh libaprautils-1.0.0-1.x86_64.rpm libaprautils-devel-1.0.0-1.x86_64.rpm

# Verify installation
rpm -ql libaprautils
rpm -ql libaprautils-devel

# Test compilation
g++ -o test test.cpp -lApraUtils -lpthread -ludev

# Remove
sudo rpm -e libaprautils libaprautils-devel
```

### Test Conan Package

```bash
# Create test project
mkdir test_conan && cd test_conan

# Create conanfile.txt
cat > conanfile.txt << EOF
[requires]
aprautils/1.0.0

[generators]
CMakeDeps
CMakeToolchain
EOF

# Install dependencies
conan install . --build=missing

# Test in CMake project
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/generators/conan_toolchain.cmake
cmake --build build
```

### Test vcpkg Package

```bash
# Install package
vcpkg install aprautils

# Create test CMake project
cat > CMakeLists.txt << EOF
cmake_minimum_required(VERSION 3.10)
project(test)

find_package(ApraUtils CONFIG REQUIRED)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE ApraUtils::ApraUtils)
EOF

# Build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

---

## Continuous Integration for Packaging

Add to `.github/workflows/packaging.yml`:

```yaml
name: Packaging

on:
  push:
    tags:
      - 'v*'

jobs:
  package-deb:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Build .deb package
        run: |
          mkdir build && cd build
          cmake -DCMAKE_INSTALL_PREFIX=/usr ..
          make
          cpack -G DEB
      - name: Upload .deb artifact
        uses: actions/upload-artifact@v4
        with:
          name: debian-package
          path: build/*.deb

  package-rpm:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install RPM tools
        run: sudo apt-get install -y rpm
      - name: Build .rpm package
        run: |
          mkdir build && cd build
          cmake ..
          make
          cpack -G RPM
      - name: Upload .rpm artifact
        uses: actions/upload-artifact@v4
        with:
          name: rpm-package
          path: build/*.rpm
```

---

## Best Practices

1. **Version Management**: Use semantic versioning (MAJOR.MINOR.PATCH)
2. **Dependencies**: Clearly document all dependencies in package metadata
3. **Testing**: Test packages on clean systems before distribution
4. **Documentation**: Include installation instructions in package descriptions
5. **Licensing**: Ensure LICENSE file is included in all packages
6. **Changelog**: Keep CHANGELOG.md updated for each release
7. **Signing**: Sign packages for production distributions (GPG for .deb, RPM signing)

---

## Support

For packaging issues or questions:
- Open an issue: https://github.com/Apra-Labs/ApraUtils/issues
- Check documentation: https://github.com/Apra-Labs/ApraUtils
- Contact: Apra Labs development team

---

## References

- [CMake Install Documentation](https://cmake.org/cmake/help/latest/command/install.html)
- [Debian Packaging Guide](https://www.debian.org/doc/manuals/maint-guide/)
- [RPM Packaging Guide](https://rpm-packaging-guide.github.io/)
- [Conan Documentation](https://docs.conan.io/)
- [vcpkg Documentation](https://vcpkg.io/en/docs/)
- [CPack Documentation](https://cmake.org/cmake/help/latest/module/CPack.html)
