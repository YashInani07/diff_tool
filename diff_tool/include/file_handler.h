#pragma once

#include <vector>
#include <string>
#include <fstream>

class FileHandler {
public:
    /**
     * Read file with streaming I/O and line buffering
     * Handles large files efficiently
     */
    static std::vector<std::string> readFile(const std::string& filepath);

    /**
     * Detect if file is binary
     * Checks for null bytes and non-UTF8 sequences
     */
    static bool isBinaryFile(const std::string& filepath);

    /**
     * Normalize line endings (CRLF -> LF)
     */
    static std::string normalizeLineEnding(const std::string& line);

    /**
     * Check if file exists
     */
    static bool fileExists(const std::string& filepath);

    /**
     * Get file size in bytes
     */
    static long getFileSize(const std::string& filepath);

private:
    static constexpr size_t BUFFER_SIZE = 65536;  // 64KB buffer
    static constexpr size_t MAX_BINARY_CHECK_SIZE = 512;  // Check first 512 bytes for binary
};
