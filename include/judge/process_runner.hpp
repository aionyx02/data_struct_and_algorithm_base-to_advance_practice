#pragma once

#include <chrono>
#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

namespace judge {

struct ProcessRequest {
    std::string executable;
    std::vector<std::string> arguments;
    std::filesystem::path workingDirectory;
    std::string standardInput;
    std::chrono::milliseconds timeout{1000};
    std::size_t outputLimitBytes = 64 * 1024;
};

struct ProcessResult {
    bool launched = false;
    bool timedOut = false;
    bool outputLimitExceeded = false;
    int exitCode = -1;
    std::chrono::milliseconds elapsed{0};
    std::string standardOutput;
    std::string standardError;
    std::string launchError;
};

class ProcessRunner {
public:
    [[nodiscard]] ProcessResult run(const ProcessRequest& request) const;
};

}  // namespace judge
