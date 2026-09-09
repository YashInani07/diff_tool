#pragma once

#include <vector>
#include <string>
#include <utility>

enum class OperationType {
    EQUAL,
    INSERT,
    DELETE
};

struct DiffOperation {
    OperationType type;
    int line_num;
    std::string content;
};

class DiffAlgorithm {
public:
    struct EditScript {
        std::vector<DiffOperation> operations;
        int deletions;
        int insertions;
    };

    /**
     * Compute Myers' diff algorithm between two files
     * Time complexity: O(N + M + D²) where D is edit distance
     * Space complexity: O(D²)
     */
    static EditScript compute(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2
    );

private:
    struct Snake {
        int x, y;  // positions in both sequences
        int diag;  // diagonal index
    };

    struct FrontierPoint {
        int x, y;
        std::vector<DiffOperation> path;
    };

    static EditScript backtrack(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2,
        const std::vector<std::vector<int>>& dp
    );

    static std::vector<DiffOperation> extractOperations(
        const std::vector<std::string>& file1,
        const std::vector<std::string>& file2
    );
};
