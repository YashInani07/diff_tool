#pragma once

#include "diff_algorithm.h"
#include <string>
#include <vector>

class DiffFormatter {
public:
    virtual ~DiffFormatter() = default;
    virtual std::string format(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const DiffAlgorithm::EditScript& script,
        const std::string& filename1,
        const std::string& filename2
    ) const = 0;
};

/**
 * Unified diff format (like `diff -u`)
 * Shows changes with context lines
 */
class UnifiedDiffFormatter : public DiffFormatter {
public:
    explicit UnifiedDiffFormatter(int context_lines = 3)
        : context_lines_(context_lines) {}

    std::string format(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const DiffAlgorithm::EditScript& script,
        const std::string& filename1,
        const std::string& filename2
    ) const override;

private:
    int context_lines_;
};

/**
 * Context diff format (like `diff -c`)
 * Shows context with clear separators
 */
class ContextDiffFormatter : public DiffFormatter {
public:
    explicit ContextDiffFormatter(int context_lines = 3)
        : context_lines_(context_lines) {}

    std::string format(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const DiffAlgorithm::EditScript& script,
        const std::string& filename1,
        const std::string& filename2
    ) const override;

private:
    int context_lines_;
};

/**
 * Side-by-side diff format
 * Shows both files adjacent for easy visual comparison
 */
class SideBySideDiffFormatter : public DiffFormatter {
public:
    explicit SideBySideDiffFormatter(int column_width = 40)
        : column_width_(column_width) {}

    std::string format(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const DiffAlgorithm::EditScript& script,
        const std::string& filename1,
        const std::string& filename2
    ) const override;

private:
    int column_width_;
    std::string padString(const std::string& str, int width) const;
};

/**
 * Summary formatter
 * Shows just statistics about differences
 */
class SummaryFormatter : public DiffFormatter {
public:
    std::string format(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const DiffAlgorithm::EditScript& script,
        const std::string& filename1,
        const std::string& filename2
    ) const override;
};
