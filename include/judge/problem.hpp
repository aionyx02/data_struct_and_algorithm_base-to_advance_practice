#pragma once

#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace judge {

struct ProblemMetadata {
    std::string id;
    std::string title;
    std::string stage;
    std::string difficulty;
    std::string judgeMode;
    int timeLimitMs = 0;
    int memoryLimitMb = 0;
    std::size_t outputLimitBytes = 0;
    std::string expectedTime;
    std::string expectedSpace;
    std::vector<std::string> forbiddenSymbols;
    int randomTests = 0;
    int operationLimit = 0;
    std::string testsDirectory;
};

struct Problem {
    ProblemMetadata metadata;
    std::filesystem::path directory;
    std::filesystem::path statementPath;
    std::filesystem::path testsPath;
};

}  // namespace judge
