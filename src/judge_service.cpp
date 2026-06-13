#include "judge/judge_service.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <system_error>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace judge {
namespace {

std::string readFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot read file: " + path.string());
    }
    std::ostringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

std::string pathToUtf8(const std::filesystem::path& path) {
#ifdef _WIN32
    const std::wstring& value = path.native();
    if (value.empty()) {
        return {};
    }
    const int required = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr
    );
    if (required <= 0) {
        throw std::runtime_error("failed to encode filesystem path as UTF-8");
    }
    std::string result(static_cast<std::size_t>(required), '\0');
    if (WideCharToMultiByte(
            CP_UTF8,
            0,
            value.data(),
            static_cast<int>(value.size()),
            result.data(),
            required,
            nullptr,
            nullptr
        ) != required) {
        throw std::runtime_error("failed to encode filesystem path as UTF-8");
    }
    return result;
#else
    const std::u8string value = path.u8string();
    return std::string(
        reinterpret_cast<const char*>(value.data()),
        value.size()
    );
#endif
}

bool isWithin(
    const std::filesystem::path& child,
    const std::filesystem::path& parent
) {
    const std::filesystem::path relative = child.lexically_relative(parent);
    return !relative.empty() && *relative.begin() != "..";
}

std::string normalizeOutput(std::string value) {
    value.erase(
        std::remove(value.begin(), value.end(), '\r'),
        value.end()
    );
    while (!value.empty() &&
           (value.back() == ' ' || value.back() == '\t' || value.back() == '\n')) {
        value.pop_back();
    }
    return value;
}

std::string summarizeOutput(const std::string& value) {
    constexpr std::size_t kSummaryLimit = 240;
    std::string normalized = normalizeOutput(value);
    if (normalized.size() <= kSummaryLimit) {
        return normalized;
    }
    normalized.resize(kSummaryLimit);
    return normalized + "...";
}

std::filesystem::path createRuntimeDirectory(
    const std::filesystem::path& runtimeRoot
) {
    static std::atomic<unsigned long long> sequence = 0;
    const auto timestamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    const std::filesystem::path directory =
        runtimeRoot /
        ("run-" + std::to_string(timestamp) + "-" +
         std::to_string(sequence.fetch_add(1)));
    std::filesystem::create_directories(directory);
    return std::filesystem::weakly_canonical(directory);
}

class RuntimeDirectoryGuard {
public:
    RuntimeDirectoryGuard(
        std::filesystem::path directory,
        std::filesystem::path runtimeRoot
    )
        : directory_(std::move(directory)),
          runtimeRoot_(std::move(runtimeRoot)) {}

    ~RuntimeDirectoryGuard() {
        if (!isWithin(directory_, runtimeRoot_)) {
            return;
        }
        std::error_code error;
        std::filesystem::remove_all(directory_, error);
    }

private:
    std::filesystem::path directory_;
    std::filesystem::path runtimeRoot_;
};

JudgeReport failureReport(
    const Problem& problem,
    Verdict verdict,
    std::string detail
) {
    return JudgeReport{
        .problemId = problem.metadata.id,
        .verdict = verdict,
        .detail = std::move(detail),
        .tests = {},
    };
}

std::string configuredCompiler() {
#ifdef _WIN32
    char* value = nullptr;
    std::size_t length = 0;
    if (_dupenv_s(&value, &length, "CXX") == 0 && value != nullptr) {
        std::string compiler(value);
        std::free(value);
        if (!compiler.empty()) {
            return compiler;
        }
    }
    std::free(value);
#else
    const char* value = std::getenv("CXX");
    if (value != nullptr && *value != '\0') {
        return value;
    }
#endif
    return "g++";
}

}  // namespace

JudgeService::JudgeService(
    std::filesystem::path projectRoot,
    ProcessRunner runner
)
    : projectRoot_(std::filesystem::weakly_canonical(std::move(projectRoot))),
      runtimeRoot_(projectRoot_ / ".judge" / "runtime"),
      runner_(std::move(runner)) {
    std::filesystem::create_directories(runtimeRoot_);
    runtimeRoot_ = std::filesystem::weakly_canonical(runtimeRoot_);
    if (!isWithin(runtimeRoot_, projectRoot_)) {
        throw std::runtime_error("runtime directory escapes project root");
    }
}

JudgeReport JudgeService::judge(
    const Problem& problem,
    const std::filesystem::path& sourcePath
) const {
    const std::filesystem::path source =
        std::filesystem::weakly_canonical(sourcePath);
    if (!isWithin(source, projectRoot_)) {
        return failureReport(
            problem,
            Verdict::kRuntimeError,
            "source file must be inside the project root"
        );
    }
    if (!std::filesystem::is_regular_file(source) || source.extension() != ".cpp") {
        return failureReport(
            problem,
            Verdict::kRuntimeError,
            "source path must name an existing .cpp file"
        );
    }

    const std::string sourceText = readFile(source);
    for (const std::string& forbidden : problem.metadata.forbiddenSymbols) {
        if (!forbidden.empty() && sourceText.find(forbidden) != std::string::npos) {
            return failureReport(
                problem,
                Verdict::kForbiddenApi,
                "forbidden symbol found: " + forbidden
            );
        }
    }

    const std::filesystem::path runDirectory =
        createRuntimeDirectory(runtimeRoot_);
    RuntimeDirectoryGuard cleanup(runDirectory, runtimeRoot_);
#ifdef _WIN32
    const std::filesystem::path executablePath = runDirectory / "submission.exe";
#else
    const std::filesystem::path executablePath = runDirectory / "submission";
#endif

    const std::string compiler = configuredCompiler();

    ProcessRequest compileRequest{
        .executable = compiler,
        .arguments = {
            "-std=c++20",
            "-O2",
            "-pipe",
            pathToUtf8(source),
            "-o",
            pathToUtf8(executablePath),
        },
        .workingDirectory = runDirectory,
        .standardInput = {},
        .timeout = std::chrono::seconds(20),
        .outputLimitBytes = 128 * 1024,
    };
    const ProcessResult compileResult = runner_.run(compileRequest);
    if (!compileResult.launched) {
        return failureReport(
            problem,
            Verdict::kCompileError,
            "compiler could not start: " + compileResult.launchError
        );
    }
    if (compileResult.timedOut) {
        return failureReport(
            problem,
            Verdict::kCompileError,
            "compilation exceeded 20 seconds"
        );
    }
    if (compileResult.outputLimitExceeded) {
        return failureReport(
            problem,
            Verdict::kCompileError,
            "compiler output limit exceeded"
        );
    }
    if (compileResult.exitCode != 0) {
        return failureReport(
            problem,
            Verdict::kCompileError,
            summarizeOutput(compileResult.standardError)
        );
    }

    std::vector<std::filesystem::path> inputPaths;
    for (const auto& entry :
         std::filesystem::directory_iterator(problem.testsPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".in") {
            inputPaths.push_back(entry.path());
        }
    }
    std::sort(inputPaths.begin(), inputPaths.end());

    JudgeReport report{
        .problemId = problem.metadata.id,
        .verdict = Verdict::kAccepted,
        .detail = "all tests passed",
        .tests = {},
    };

    for (const std::filesystem::path& inputPath : inputPaths) {
        std::filesystem::path outputPath = inputPath;
        outputPath.replace_extension(".out");
        const ProcessResult runResult = runner_.run(ProcessRequest{
            .executable = pathToUtf8(executablePath),
            .arguments = {},
            .workingDirectory = runDirectory,
            .standardInput = readFile(inputPath),
            .timeout =
                std::chrono::milliseconds(problem.metadata.timeLimitMs),
            .outputLimitBytes = problem.metadata.outputLimitBytes,
        });

        TestResult test{
            .name = inputPath.stem().string(),
            .verdict = Verdict::kAccepted,
            .detail = {},
            .elapsed = runResult.elapsed,
        };
        if (!runResult.launched) {
            test.verdict = Verdict::kRuntimeError;
            test.detail = "submission could not start: " + runResult.launchError;
        } else if (runResult.timedOut) {
            test.verdict = Verdict::kTimeLimitExceeded;
            test.detail =
                "time limit exceeded (" +
                std::to_string(problem.metadata.timeLimitMs) + " ms)";
        } else if (runResult.outputLimitExceeded) {
            test.verdict = Verdict::kRuntimeError;
            test.detail =
                "output limit exceeded (" +
                std::to_string(problem.metadata.outputLimitBytes) + " bytes)";
        } else if (runResult.exitCode != 0) {
            test.verdict = Verdict::kRuntimeError;
            test.detail =
                "process exited with code " + std::to_string(runResult.exitCode);
        } else {
            const std::string expected = normalizeOutput(readFile(outputPath));
            const std::string actual = normalizeOutput(runResult.standardOutput);
            if (actual != expected) {
                test.verdict = Verdict::kWrongAnswer;
                test.detail =
                    "expected '" + summarizeOutput(expected) +
                    "', got '" + summarizeOutput(actual) + "'";
            }
        }

        report.tests.push_back(test);
        if (test.verdict != Verdict::kAccepted) {
            report.verdict = test.verdict;
            report.detail = test.name + ": " + test.detail;
            return report;
        }
    }
    return report;
}

std::string toString(Verdict verdict) {
    switch (verdict) {
        case Verdict::kAccepted:
            return "AC";
        case Verdict::kWrongAnswer:
            return "WA";
        case Verdict::kTimeLimitExceeded:
            return "TLE";
        case Verdict::kMemoryLimitExceeded:
            return "MLE";
        case Verdict::kRuntimeError:
            return "RE";
        case Verdict::kCompileError:
            return "CE";
        case Verdict::kForbiddenApi:
            return "API";
        case Verdict::kInvariantViolation:
            return "INV";
        case Verdict::kComplexityWarning:
            return "CX";
    }
    return "RE";
}

}  // namespace judge
