#include "diff_algorithm.h"
#include <algorithm>
#include <unordered_map>

DiffAlgorithm::EditScript DiffAlgorithm::compute(
    const std::vector<std::string>& file1,
    const std::vector<std::string>& file2) {

    int n = file1.size();
    int m = file2.size();

    EditScript result;

    // Handle empty files
    if (n == 0 && m == 0) {
        result.deletions = 0;
        result.insertions = 0;
        return result;
    }

    if (n == 0) {
        result.insertions = m;
        for (int i = 0; i < m; i++) {
            result.operations.push_back({OperationType::INSERT, i + 1, file2[i]});
        }
        return result;
    }

    if (m == 0) {
        result.deletions = n;
        for (int i = 0; i < n; i++) {
            result.operations.push_back({OperationType::DELETE, i + 1, file1[i]});
        }
        return result;
    }

    // Use LCS-based approach: Longest Common Subsequence with backtracking
    // This is simpler and more reliable than Myers' algorithm for clarity
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

    // Build DP table
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (file1[i - 1] == file2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    // Backtrack to find differences
    int i = n, j = m;
    int deletions = 0, insertions = 0;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && file1[i - 1] == file2[j - 1]) {
            // Line is same in both files
            result.operations.insert(result.operations.begin(),
                {OperationType::EQUAL, i, file1[i - 1]});
            i--;
            j--;
        } else if (i > 0 && (j == 0 || dp[i - 1][j] >= dp[i][j - 1])) {
            // Line deleted from file1
            result.operations.insert(result.operations.begin(),
                {OperationType::DELETE, i, file1[i - 1]});
            deletions++;
            i--;
        } else if (j > 0) {
            // Line inserted (present in file2 but not file1)
            result.operations.insert(result.operations.begin(),
                {OperationType::INSERT, j, file2[j - 1]});
            insertions++;
            j--;
        }
    }

    result.deletions = deletions;
    result.insertions = insertions;

    return result;
}
