.PHONY: help build release debug test clean run install uninstall format

help:
	@echo "Diff Tool - Build Commands"
	@echo "=========================="
	@echo "  make build         - Build in release mode (default)"
	@echo "  make release       - Build optimized release version"
	@echo "  make debug         - Build with debug symbols"
	@echo "  make test          - Run test suite"
	@echo "  make clean         - Remove all build artifacts"
	@echo "  make run           - Build and run with examples"
	@echo "  make install       - Install to system (requires sudo)"
	@echo "  make uninstall     - Remove from system (requires sudo)"
	@echo "  make rebuild       - Clean and full rebuild"
	@echo "  make format        - Format code with clang-format"

build:
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make

release:
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3" .. && make

debug:
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make

test: build
	@cd build && ctest --verbose

clean:
	@rm -rf build

rebuild: clean build

run: build
	@echo "Running diff tool with example files..."
	@./build/diff examples/file1.txt examples/file2.txt
	@echo ""
	@echo "Running with summary format..."
	@./build/diff -s examples/file1.txt examples/file2.txt

install: build
	@cd build && sudo make install
	@echo "Diff tool installed successfully"

uninstall:
	@cd build && sudo xargs rm < install_manifest.txt 2>/dev/null || true
	@echo "Diff tool uninstalled"

format:
	@echo "Formatting C++ code..."
	@find include src tests -name "*.cpp" -o -name "*.h" | xargs clang-format -i --style=google

# Development shortcuts
test-all: clean debug test
	@echo "All tests passed in debug mode"

quick: debug test

full: clean release test install
	@echo "Full build, test, and install complete"

# Verbose targets
verbose-test:
	@mkdir -p build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make
	@cd build && ctest --verbose --output-on-failure

# Memory checks (requires valgrind)
memcheck: build
	@valgrind --leak-check=full --show-leak-kinds=all ./build/diff examples/file1.txt examples/file2.txt

# Info target
info:
	@echo "Project: Diff Tool"
	@echo "Language: C++17"
	@echo "Build System: CMake"
	@echo ""
	@echo "Files:"
	@find . -type f \( -name "*.cpp" -o -name "*.h" \) -exec wc -l {} + | tail -1
	@echo ""
	@echo "To get started:"
	@echo "  make build    # Build the project"
	@echo "  make test     # Run tests"
	@echo "  make run      # See it in action"
