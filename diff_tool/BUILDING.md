# Building the Diff Tool

## Prerequisites

### Linux/macOS
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake git

# macOS
brew install cmake

# Fedora/RHEL
sudo dnf install gcc-c++ cmake make git
```

### Windows
- Download and install [MinGW-w64](https://www.mingw-w64.org/) or [MSVC](https://visualstudio.microsoft.com/)
- Download and install [CMake](https://cmake.org/download/)
- Use a Git client or [Git for Windows](https://git-scm.com/download/win)

## Build Instructions

### Standard Build

```bash
# Clone the repository
git clone https://github.com/yourusername/diff-tool.git
cd diff-tool

# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
make

# Run tests
make test
# or
ctest --verbose
```

### Debug Build

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
ctest --verbose
```

### Release Build

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Verification

Test the build with the example files:

```bash
./diff ../examples/file1.txt ../examples/file2.txt

# Try different formats
./diff -c ../examples/file1.txt ../examples/file2.txt
./diff -y ../examples/file1.txt ../examples/file2.txt
./diff -s ../examples/file1.txt ../examples/file2.txt
```

## Installation

### System-wide Install (Linux/macOS)

```bash
cd build
sudo make install
# diff will be installed to /usr/local/bin/
which diff
```

### Uninstall

```bash
cd build
sudo xargs rm < install_manifest.txt
```

## Compiler Support

| Compiler | Minimum Version | Status |
|----------|-----------------|--------|
| GCC      | 7.0             | ✅ Supported |
| Clang    | 5.0             | ✅ Supported |
| MSVC     | 2017            | ✅ Supported |

## Troubleshooting

### CMake not found
```bash
# Install CMake
# Ubuntu: sudo apt-get install cmake
# macOS: brew install cmake
```

### Compiler not found
```bash
# Install build tools
# Ubuntu: sudo apt-get install build-essential
# macOS: xcode-select --install
```

### Permission denied on make test
```bash
chmod +x diff
chmod +x diff_test
make test
```

### Build fails with "not an object file"
```bash
# Clean and rebuild
cd build
rm -rf *
cmake ..
make clean
make
```

## Static Linking

To create a static executable:

```bash
cd build
cmake -DCMAKE_EXE_LINKER_FLAGS="-static" ..
make
```

## Cross-Compilation

### For ARM (e.g., Raspberry Pi)

```bash
cd build
cmake -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ ..
make
```

## Profiling

### With gprof

```bash
cd build
cmake -DCMAKE_CXX_FLAGS="-pg" ..
make
./diff examples/file1.txt examples/file2.txt
gprof ./diff gmon.out | less
```

## Memory Analysis

### With Valgrind

```bash
cd build
valgrind --leak-check=full --show-leak-kinds=all ./diff ../examples/file1.txt ../examples/file2.txt
```

## Code Coverage

```bash
cd build
cmake -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage" ..
make
ctest
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage_html
open coverage_html/index.html  # or browse to it
```

## Documentation Generation

If Doxygen is installed:

```bash
cd build
cmake -DBUILD_DOCS=ON ..
make docs
# Documentation will be in build/docs/html/
```

## Docker Build (Optional)

Create a `Dockerfile`:

```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git

WORKDIR /app
COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

ENTRYPOINT ["/app/build/diff"]
```

Build and run:

```bash
docker build -t diff-tool .
docker run diff-tool file1.txt file2.txt
```
