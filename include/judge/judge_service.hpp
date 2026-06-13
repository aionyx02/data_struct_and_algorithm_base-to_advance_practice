#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "judge/problem.hpp"
#include "judge/process_runner.hpp"

namespace judge {

enum class Verdict {
    kAccepted,
    kWrongAnswer,
    kTimeLimitExceeded,
    kMemoryLimitExceeded,
    kRuntimeError,
    kCompileError,
    kForbiddenApi,
    kInvariantViolation,
    kComplexityWarning,
};

struct TestResult {
    std::string name;
    Verdict verdict = Verdict::kRuntimeError;
    std::string detail;
    std::chrono::milliseconds elapsed{0};
};

struct JudgeReport {
    std::string problemId;
    Verdict verdict = Verdict::kRuntimeError;
    std::string detail;
    std::vector<TestResult> tests;
};

class JudgeService {
public:
    JudgeService(std::filesystem::path projectRoot, ProcessRunner runner = {});

    [[nodiscard]] JudgeReport judge(
        const Problem& problem,
        const std::filesystem::path& sourcePath
    ) const;

private:
    std::filesystem::path projectRoot_;
    std::filesystem::path runtimeRoot_;
    ProcessRunner runner_;
};

[[nodiscard]] std::string toString(Verdict verdict);

}  // namespace judge
