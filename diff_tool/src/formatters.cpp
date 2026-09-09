#include "formatters.h"
#include <sstream>
#include <iomanip>
#include <ctime>

std::string UnifiedDiffFormatter::format(
    const std::vector<std::string>& file1,
    const std::vector<std::string>& file2,
    const DiffAlgorithm::EditScript& script,
    const std::string& filename1,
    const std::string& filename2) const {

    std::ostringstream oss;

    // Header
    oss << "--- " << filename1 << "\n";
    oss << "+++ " << filename2 << "\n";

    if (script.operations.empty()) {
        return oss.str();
    }

    // Build diff blocks with context
    std::vector<std::vector<DiffOperation>> blocks;
    std::vector<DiffOperation> current_block;
    int last_change_line = -100;

    for (const auto& op : script.operations) {
        int current_line = op.line_num;

        // Start new block if too far from last change
        if (current_line - last_change_line > 2 * context_lines_ + 1) {
            if (!current_block.empty()) {
                blocks.push_back(current_block);
                current_block.clear();
            }
        }

        // Add context before this change
        int start_line = std::max(0, current_line - context_lines_);

        // Add all operations with their context
        current_block.push_back(op);
        last_change_line = current_line;
    }

    if (!current_block.empty()) {
        blocks.push_back(current_block);
    }

    // Format each block
    for (const auto& block : blocks) {
        int start_line1 = 1, start_line2 = 1;
        int added = 0, deleted = 0;

        for (const auto& op : block) {
            if (op.type == OperationType::DELETE) {
                deleted++;
            } else if (op.type == OperationType::INSERT) {
                added++;
            }
        }

        oss << "@@ -" << start_line1 << "," << (deleted + context_lines_)
            << " +" << start_line2 << "," << (added + context_lines_) << " @@\n";

        for (const auto& op : block) {
            switch (op.type) {
                case OperationType::EQUAL:
                    oss << " " << op.content << "\n";
                    break;
                case OperationType::DELETE:
                    oss << "-" << op.content << "\n";
                    break;
                case OperationType::INSERT:
                    oss << "+" << op.content << "\n";
                    break;
            }
        }
    }

    return oss.str();
}

std::string ContextDiffFormatter::format(
    const std::vector<std::string>& file1,
    const std::vector<std::string>& file2,
    const DiffAlgorithm::EditScript& script,
    const std::string& filename1,
    const std::string& filename2) const {

    std::ostringstream oss;

    // Header with timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    oss << "*** " << filename1 << "\t"
        << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";
    oss << "--- " << filename2 << "\t"
        << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "\n";

    if (script.operations.empty()) {
        return oss.str();
    }

    // Format changes
    for (const auto& op : script.operations) {
        switch (op.type) {
            case OperationType::EQUAL:
                oss << "  " << op.content << "\n";
                break;
            case OperationType::DELETE:
                oss << "< " << op.content << "\n";
                break;
            case OperationType::INSERT:
                oss << "> " << op.content << "\n";
                break;
        }
    }

    oss << "---\n";
    return oss.str();
}

std::string SideBySideDiffFormatter::padString(
    const std::string& str,
    int width) const {

    if (str.length() >= width) {
        return str.substr(0, width);
    }
    std::string result = str;
    result.resize(width, ' ');
    return result;
}

std::string SideBySideDiffFormatter::format(
    const std::vector<std::string>& file1,
    const std::vector<std::string>& file2,
    const DiffAlgorithm::EditScript& script,
    const std::string& filename1,
    const std::string& filename2) const {

    std::ostringstream oss;

    // Header
    oss << padString(filename1, column_width_) << " | " << filename2 << "\n";
    oss << std::string(column_width_, '-') << "-+-" << std::string(column_width_, '-') << "\n";

    int line1 = 0, line2 = 0;

    for (const auto& op : script.operations) {
        switch (op.type) {
            case OperationType::EQUAL: {
                std::string left = line1 < file1.size() ? file1[line1] : "";
                oss << padString(left, column_width_) << " | " << left << "\n";
                line1++;
                line2++;
                break;
            }
            case OperationType::DELETE:
                oss << padString(op.content, column_width_) << " < \n";
                line1++;
                break;
            case OperationType::INSERT:
                oss << padString("", column_width_) << " > " << op.content << "\n";
                line2++;
                break;
        }
    }

    return oss.str();
}

std::string SummaryFormatter::format(
    const std::vector<std::string>& file1,
    const std::vector<std::string>& file2,
    const DiffAlgorithm::EditScript& script,
    const std::string& filename1,
    const std::string& filename2) const {

    std::ostringstream oss;

    int additions = 0, deletions = 0, unchanged = 0;

    for (const auto& op : script.operations) {
        switch (op.type) {
            case OperationType::EQUAL:
                unchanged++;
                break;
            case OperationType::DELETE:
                deletions++;
                break;
            case OperationType::INSERT:
                additions++;
                break;
        }
    }

    oss << "File: " << filename1 << " -> " << filename2 << "\n";
    oss << "Lines unchanged: " << unchanged << "\n";
    oss << "Lines added:     +" << additions << "\n";
    oss << "Lines deleted:   -" << deletions << "\n";
    oss << "Total diff:      " << (additions + deletions) << "\n";

    if (additions + deletions == 0) {
        oss << "Files are identical.\n";
    } else {
        double similarity = (double)unchanged / (file1.size() + file2.size()) * 100;
        oss << "Similarity:      " << std::fixed << std::setprecision(2) << similarity << "%\n";
    }

    return oss.str();
}
