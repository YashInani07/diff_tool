#include "diff_algorithm.h"
#include "formatters.h"
#include "file_handler.h"
#include <iostream>
#include <memory>
#include <cstring>

enum class OutputFormat {
    UNIFIED,
    CONTEXT,
    SIDE_BY_SIDE,
    SUMMARY
};

struct Options {
    std::string file1;
    std::string file2;
    OutputFormat format = OutputFormat::UNIFIED;
    int context_lines = 3;
    int column_width = 40;
    bool verbose = false;
};

void printUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS] FILE1 FILE2\n\n"
        << "Options:\n"
        << "  -u, --unified          Output unified diff format (default)\n"
        << "  -c, --context          Output context diff format\n"
        << "  -y, --side-by-side     Output side-by-side diff\n"
        << "  -s, --summary          Output summary only\n"
        << "  -C, --context-lines N  Number of context lines (default: 3)\n"
        << "  -W, --width N          Column width for side-by-side (default: 40)\n"
        << "  -v, --verbose          Verbose output\n"
        << "  -h, --help             Show this help message\n";
}

bool parseArguments(int argc, char* argv[], Options& opts) {
    if (argc < 3) {
        printUsage(argv[0]);
        return false;
    }

    int file_count = 0;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return false;
        } else if (arg == "-u" || arg == "--unified") {
            opts.format = OutputFormat::UNIFIED;
        } else if (arg == "-c" || arg == "--context") {
            opts.format = OutputFormat::CONTEXT;
        } else if (arg == "-y" || arg == "--side-by-side") {
            opts.format = OutputFormat::SIDE_BY_SIDE;
        } else if (arg == "-s" || arg == "--summary") {
            opts.format = OutputFormat::SUMMARY;
        } else if (arg == "-C" || arg == "--context-lines") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -C requires an argument\n";
                return false;
            }
            opts.context_lines = std::atoi(argv[++i]);
        } else if (arg == "-W" || arg == "--width") {
            if (i + 1 >= argc) {
                std::cerr << "Error: -W requires an argument\n";
                return false;
            }
            opts.column_width = std::atoi(argv[++i]);
        } else if (arg == "-v" || arg == "--verbose") {
            opts.verbose = true;
        } else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option: " << arg << "\n";
            return false;
        } else {
            // Positional argument (file)
            if (file_count == 0) {
                opts.file1 = arg;
            } else if (file_count == 1) {
                opts.file2 = arg;
            } else {
                std::cerr << "Error: Too many positional arguments\n";
                return false;
            }
            file_count++;
        }
    }

    if (file_count != 2) {
        std::cerr << "Error: Two files are required\n";
        printUsage(argv[0]);
        return false;
    }

    return true;
}

int main(int argc, char* argv[]) {
    Options opts;

    if (!parseArguments(argc, argv, opts)) {
        return 1;
    }

    // Check files exist
    if (!FileHandler::fileExists(opts.file1)) {
        std::cerr << "Error: File not found: " << opts.file1 << "\n";
        return 1;
    }

    if (!FileHandler::fileExists(opts.file2)) {
        std::cerr << "Error: File not found: " << opts.file2 << "\n";
        return 1;
    }

    // Check for binary files
    if (FileHandler::isBinaryFile(opts.file1) || FileHandler::isBinaryFile(opts.file2)) {
        std::cerr << "Error: One or both files appear to be binary\n";
        return 1;
    }

    if (opts.verbose) {
        std::cerr << "Reading file 1: " << opts.file1 << " ("
            << FileHandler::getFileSize(opts.file1) << " bytes)\n";
        std::cerr << "Reading file 2: " << opts.file2 << " ("
            << FileHandler::getFileSize(opts.file2) << " bytes)\n";
    }

    // Read files
    auto lines1 = FileHandler::readFile(opts.file1);
    auto lines2 = FileHandler::readFile(opts.file2);

    if (opts.verbose) {
        std::cerr << "File 1: " << lines1.size() << " lines\n";
        std::cerr << "File 2: " << lines2.size() << " lines\n";
    }

    // Compute diff
    if (opts.verbose) {
        std::cerr << "Computing diff...\n";
    }

    auto diff_script = DiffAlgorithm::compute(lines1, lines2);

    if (opts.verbose) {
        std::cerr << "Diff complete. Changes: " << diff_script.operations.size() << "\n";
    }

    // Choose formatter
    std::unique_ptr<DiffFormatter> formatter;

    switch (opts.format) {
        case OutputFormat::UNIFIED:
            formatter = std::make_unique<UnifiedDiffFormatter>(opts.context_lines);
            break;
        case OutputFormat::CONTEXT:
            formatter = std::make_unique<ContextDiffFormatter>(opts.context_lines);
            break;
        case OutputFormat::SIDE_BY_SIDE:
            formatter = std::make_unique<SideBySideDiffFormatter>(opts.column_width);
            break;
        case OutputFormat::SUMMARY:
            formatter = std::make_unique<SummaryFormatter>();
            break;
    }

    // Format and print
    std::string output = formatter->format(lines1, lines2, diff_script, opts.file1, opts.file2);
    std::cout << output;

    return 0;
}
