#include "diff_algorithm.h"
#include "file_handler.h"
#include <cassert>
#include <iostream>
#include <vector>

void testSimpleDiff() {
    std::cout << "Test 1: Simple diff...\n";
    std::vector<std::string> file1 = {"hello", "world"};
    std::vector<std::string> file2 = {"hello", "universe"};

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(!result.operations.empty());
    std::cout << "  ✓ Passed\n";
}

void testEmptyFiles() {
    std::cout << "Test 2: Empty files...\n";
    std::vector<std::string> file1 = {};
    std::vector<std::string> file2 = {};

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(result.operations.empty());
    assert(result.deletions == 0);
    assert(result.insertions == 0);
    std::cout << "  ✓ Passed\n";
}

void testEmptyToContent() {
    std::cout << "Test 3: Empty file to content...\n";
    std::vector<std::string> file1 = {};
    std::vector<std::string> file2 = {"line1", "line2", "line3"};

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(result.insertions == 3);
    std::cout << "  ✓ Passed\n";
}

void testContentToEmpty() {
    std::cout << "Test 4: Content to empty file...\n";
    std::vector<std::string> file1 = {"line1", "line2"};
    std::vector<std::string> file2 = {};

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(result.deletions == 2);
    std::cout << "  ✓ Passed\n";
}

void testIdenticalFiles() {
    std::cout << "Test 5: Identical files...\n";
    std::vector<std::string> file1 = {"line1", "line2", "line3"};
    std::vector<std::string> file2 = {"line1", "line2", "line3"};

    auto result = DiffAlgorithm::compute(file1, file2);
    // Should mostly be EQUAL operations
    int equal_count = 0;
    for (const auto& op : result.operations) {
        if (op.type == OperationType::EQUAL) {
            equal_count++;
        }
    }
    assert(equal_count >= 2);  // At least 2 equal lines
    std::cout << "  ✓ Passed\n";
}

void testComplexDiff() {
    std::cout << "Test 6: Complex diff...\n";
    std::vector<std::string> file1 = {
        "function example() {",
        "  int x = 5;",
        "  return x;",
        "}"
    };
    std::vector<std::string> file2 = {
        "function example() {",
        "  int x = 10;",
        "  int y = 20;",
        "  return x + y;",
        "}"
    };

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(!result.operations.empty());
    std::cout << "  ✓ Passed\n";
}

void testLineNormalization() {
    std::cout << "Test 7: Line ending normalization...\n";
    std::string line_with_cr = "hello\r";
    std::string normalized = FileHandler::normalizeLineEnding(line_with_cr);
    assert(normalized == "hello");

    std::string line_without_cr = "hello";
    assert(FileHandler::normalizeLineEnding(line_without_cr) == "hello");
    std::cout << "  ✓ Passed\n";
}

void testLargeFileDiff() {
    std::cout << "Test 8: Large file handling...\n";
    std::vector<std::string> file1, file2;

    // Create large file (1000 lines)
    for (int i = 0; i < 1000; i++) {
        file1.push_back("line " + std::to_string(i));
    }

    // Create variant with some changes
    file2 = file1;
    file2[500] = "MODIFIED LINE 500";
    file2.insert(file2.begin() + 750, "NEW LINE");

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(!result.operations.empty());
    std::cout << "  ✓ Passed\n";
}

void testMultipleConsecutiveChanges() {
    std::cout << "Test 9: Multiple consecutive changes...\n";
    std::vector<std::string> file1 = {
        "a", "b", "c", "d", "e"
    };
    std::vector<std::string> file2 = {
        "a", "x", "y", "z", "e"
    };

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(!result.operations.empty());
    std::cout << "  ✓ Passed\n";
}

void testSingleLineFiles() {
    std::cout << "Test 10: Single line files...\n";
    std::vector<std::string> file1 = {"hello"};
    std::vector<std::string> file2 = {"world"};

    auto result = DiffAlgorithm::compute(file1, file2);
    assert(!result.operations.empty());
    std::cout << "  ✓ Passed\n";
}

int main() {
    std::cout << "Running diff algorithm tests...\n\n";

    try {
        testSimpleDiff();
        testEmptyFiles();
        testEmptyToContent();
        testContentToEmpty();
        testIdenticalFiles();
        testComplexDiff();
        testLineNormalization();
        testLargeFileDiff();
        testMultipleConsecutiveChanges();
        testSingleLineFiles();

        std::cout << "\n✅ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed: " << e.what() << "\n";
        return 1;
    }
}
