cmake_minimum_required(VERSION 3.10)
project(DiffTool VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wpedantic")

# Main executable
add_executable(diff
    src/main.cpp
    src/diff_algorithm.cpp
    src/formatters.cpp
    src/file_handler.cpp
)

target_include_directories(diff PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Test executable (optional)
enable_testing()
add_executable(diff_test
    tests/test_diff.cpp
    src/diff_algorithm.cpp
    src/formatters.cpp
    src/file_handler.cpp
)
target_include_directories(diff_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME DiffTests COMMAND diff_test)
