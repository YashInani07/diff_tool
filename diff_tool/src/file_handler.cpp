#include "file_handler.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

std::vector<std::string> FileHandler::readFile(const std::string& filepath) {
    std::vector<std::string> lines;

    if (!fileExists(filepath)) {
        std::cerr << "Error: File not found: " << filepath << "\n";
        return lines;
    }

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << filepath << "\n";
        return lines;
    }

    // Use streaming with buffer for large files
    std::string line;
    while (std::getline(file, line)) {
        // Normalize line endings (handle CRLF from Windows files)
        line = normalizeLineEnding(line);
        lines.push_back(line);
    }

    file.close();
    return lines;
}

bool FileHandler::isBinaryFile(const std::string& filepath) {
    if (!fileExists(filepath)) {
        return false;
    }

    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Read first portion of file to check for binary content
    char buffer[MAX_BINARY_CHECK_SIZE];
    file.read(buffer, MAX_BINARY_CHECK_SIZE);
    std::streamsize bytes_read = file.gcount();
    file.close();

    // Check for null bytes (strong indicator of binary file)
    for (int i = 0; i < bytes_read; i++) {
        if (buffer[i] == '\0') {
            return true;
        }
    }

    // Check for suspicious non-printable characters
    // (excluding common whitespace)
    int suspicious_count = 0;
    for (int i = 0; i < bytes_read; i++) {
        unsigned char c = buffer[i];
        if (c < 0x20 && c != '\t' && c != '\n' && c != '\r' && c != '\f') {
            suspicious_count++;
        }
        // High-bit characters might indicate non-UTF8
        if (c > 0x7F && (c < 0xC0 || c == 0xFF)) {
            // Could be binary or invalid UTF-8 sequence
            suspicious_count++;
        }
    }

    // If more than 30% suspicious characters, treat as binary
    return (bytes_read > 0) && (suspicious_count * 100 / bytes_read > 30);
}

std::string FileHandler::normalizeLineEnding(const std::string& line) {
    if (!line.empty() && line.back() == '\r') {
        return line.substr(0, line.length() - 1);
    }
    return line;
}

bool FileHandler::fileExists(const std::string& filepath) {
    return std::filesystem::exists(filepath) && std::filesystem::is_regular_file(filepath);
}

long FileHandler::getFileSize(const std::string& filepath) {
    if (!fileExists(filepath)) {
        return -1;
    }
    return std::filesystem::file_size(filepath);
}
