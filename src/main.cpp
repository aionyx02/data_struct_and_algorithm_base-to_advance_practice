#include <algorithm>
#include <charconv>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "judge/catalog.hpp"
#include "judge/judge_service.hpp"
#include "judge/progress.hpp"
#include "judge/stress_service.hpp"

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shellapi.h>
#endif

#ifndef DATA_STRUCTURES_PROJECT_ROOT
#define DATA_STRUCTURES_PROJECT_ROOT "."
#endif

namespace {

std::filesystem::path pathFromUtf8(std::string_view value) {
    std::u8string utf8;
    utf8.reserve(value.size());
    for (const char character : value) {
        utf8.push_back(static_cast<char8_t>(
            static_cast<unsigned char>(character)
        ));
    }
    return std::filesystem::path(utf8);
}

struct ParsedArguments {
    std::vector<std::string> positional;
    std::filesystem::path projectRoot =
        pathFromUtf8(DATA_STRUCTURES_PROJECT_ROOT);
    std::optional<std::uint64_t> seed;
    std::optional<int> caseCount;
    bool recordProgress = true;
};

std::uint64_t parseSeed(std::string_view value) {
    std::uint64_t parsed = 0;
    const auto [end, error] =
        std::from_chars(value.data(), value.data() + value.size(), parsed);
    if (error != std::errc{} || end != value.data() + value.size()) {
        throw std::runtime_error("--seed requires an unsigned 64-bit integer");
    }
    return parsed;
}

int parseCaseCount(std::string_view value) {
    int parsed = 0;
    const auto [end, error] =
        std::from_chars(value.data(), value.data() + value.size(), parsed);
    if (error != std::errc{} || end != value.data() + value.size() ||
        parsed <= 0 || parsed > 10000) {
        throw std::runtime_error("--cases must be between 1 and 10000");
    }
    return parsed;
}

std::vector<std::string> collectArguments(int argc, char* argv[]) {
#ifdef _WIN32
    int wideArgumentCount = 0;
    wchar_t** wideArguments =
        CommandLineToArgvW(GetCommandLineW(), &wideArgumentCount);
    if (wideArguments == nullptr) {
        throw std::runtime_error("failed to read the Windows command line");
    }

    std::vector<std::string> arguments;
    arguments.reserve(static_cast<std::size_t>(wideArgumentCount));
    for (int index = 0; index < wideArgumentCount; ++index) {
        const std::wstring_view value(wideArguments[index]);
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
            LocalFree(wideArguments);
            throw std::runtime_error("failed to decode a Windows command argument");
        }
        std::string utf8(static_cast<std::size_t>(required), '\0');
        WideCharToMultiByte(
            CP_UTF8,
            0,
            value.data(),
            static_cast<int>(value.size()),
            utf8.data(),
            required,
            nullptr,
            nullptr
        );
        arguments.push_back(std::move(utf8));
    }
    LocalFree(wideArguments);
    return arguments;
#else
    return std::vector<std::string>(argv, argv + argc);
#endif
}

ParsedArguments parseArguments(const std::vector<std::string>& arguments) {
    ParsedArguments parsed;
    for (std::size_t index = 1; index < arguments.size(); ++index) {
        const std::string& argument = arguments[index];
        if (argument == "--root") {
            if (index + 1 >= arguments.size()) {
                throw std::runtime_error("--root requires a path");
            }
            parsed.projectRoot = pathFromUtf8(arguments[++index]);
            continue;
        }
        if (argument == "--seed") {
            if (index + 1 >= arguments.size()) {
                throw std::runtime_error("--seed requires a value");
            }
            parsed.seed = parseSeed(arguments[++index]);
            continue;
        }
        if (argument == "--cases") {
            if (index + 1 >= arguments.size()) {
                throw std::runtime_error("--cases requires a value");
            }
            parsed.caseCount = parseCaseCount(arguments[++index]);
            continue;
        }
        if (argument == "--no-progress") {
            parsed.recordProgress = false;
            continue;
        }
        parsed.positional.push_back(argument);
    }
    return parsed;
}

void rejectStressOptions(const ParsedArguments& arguments) {
    if (arguments.seed.has_value() || arguments.caseCount.has_value()) {
        throw std::runtime_error(
            "--seed and --cases can only be used with the stress command"
        );
    }
}

void rejectNoProgressOption(const ParsedArguments& arguments) {
    if (!arguments.recordProgress) {
        throw std::runtime_error("--no-progress can only be used with the test command");
    }
}

std::uint64_t createSeed() {
    std::random_device randomDevice;
    const std::uint64_t clockValue = static_cast<std::uint64_t>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    return (static_cast<std::uint64_t>(randomDevice()) << 32U) ^
           static_cast<std::uint64_t>(randomDevice()) ^ clockValue;
}

void printUsage() {
    std::cout
        << "Usage:\n"
        << "  algo list [--root <project-root>]\n"
        << "  algo show <problem-id> [--root <project-root>]\n"
        << "  algo test <problem-id> <source.cpp> [--no-progress]"
           " [--root <project-root>]\n"
        << "  algo progress [problem-id] [--root <project-root>]\n"
        << "  algo stress <problem-id> <source.cpp> [--seed <uint64>]"
           " [--cases <count>] [--root <project-root>]\n";
}

void printProblem(const judge::Problem& problem) {
    const auto& metadata = problem.metadata;
    std::cout << metadata.id << " | " << metadata.title
              << " | " << metadata.stage
              << " | " << metadata.difficulty << '\n';
}

void printProgress(const judge::ProgressRecord& record) {
    std::cout << record.problemId
              << " | attempts " << record.attempts
              << " | accepted " << record.acceptedAttempts
              << " | best ";
    if (record.bestTimeMs.has_value()) std::cout << *record.bestTimeMs << " ms";
    else std::cout << '-';
    std::cout << " | last "
              << (record.lastVerdict.empty() ? "-" : record.lastVerdict)
              << '\n';
}

int runCommand(const ParsedArguments& arguments) {
    if (arguments.positional.empty()) {
        printUsage();
        return 2;
    }

    const judge::ProblemCatalog catalog(arguments.projectRoot);
    const std::string& command = arguments.positional[0];

    if (command == "list") {
        rejectStressOptions(arguments);
        rejectNoProgressOption(arguments);
        for (const judge::Problem& problem : catalog.list()) {
            printProblem(problem);
        }
        return 0;
    }

    if (command == "show") {
        rejectStressOptions(arguments);
        rejectNoProgressOption(arguments);
        if (arguments.positional.size() != 2) {
            throw std::runtime_error("show requires exactly one problem ID");
        }
        const judge::Problem problem = catalog.findById(arguments.positional[1]);
        printProblem(problem);
        std::cout << "Mode: " << problem.metadata.judgeMode << '\n'
                  << "Time limit: " << problem.metadata.timeLimitMs << " ms\n"
                  << "Memory limit: " << problem.metadata.memoryLimitMb
                  << " MB (declared; not enforced in this slice)\n"
                  << "Expected: " << problem.metadata.expectedTime << ", "
                  << problem.metadata.expectedSpace << '\n'
                  << "Stress defaults: " << problem.metadata.randomTests
                  << " cases x " << problem.metadata.operationLimit
                  << " operations\n"
                  << "Statement: " << problem.statementPath.string() << '\n';
        return 0;
    }

    if (command == "progress") {
        rejectStressOptions(arguments);
        rejectNoProgressOption(arguments);
        if (arguments.positional.size() > 2) {
            throw std::runtime_error("progress accepts at most one problem ID");
        }
        const judge::ProgressRepository repository(arguments.projectRoot);
        const std::vector<judge::ProgressRecord> records = repository.load();
        if (arguments.positional.size() == 2) {
            const std::string& problemId = arguments.positional[1];
            static_cast<void>(catalog.findById(problemId));
            const auto position = std::find_if(
                records.begin(), records.end(),
                [&](const judge::ProgressRecord& record) {
                    return record.problemId == problemId;
                }
            );
            if (position == records.end()) {
                printProgress(judge::ProgressRecord{.problemId = problemId});
            } else {
                printProgress(*position);
            }
        } else if (records.empty()) {
            std::cout << "No progress recorded.\n";
        } else {
            for (const judge::ProgressRecord& record : records) printProgress(record);
        }
        return 0;
    }

    if (command == "test") {
        rejectStressOptions(arguments);
        if (arguments.positional.size() != 3) {
            throw std::runtime_error("test requires a problem ID and a C++ source path");
        }
        const judge::Problem problem = catalog.findById(arguments.positional[1]);
        const judge::JudgeService service(arguments.projectRoot);
        const judge::JudgeReport report =
            service.judge(
                problem,
                pathFromUtf8(arguments.positional[2])
            );

        std::chrono::milliseconds elapsed{0};
        for (const judge::TestResult& test : report.tests) elapsed += test.elapsed;
        if (arguments.recordProgress) {
            judge::ProgressRepository(arguments.projectRoot).recordAttempt(
                report.problemId,
                report.verdict,
                elapsed
            );
        }

        std::cout << "Problem: " << report.problemId << '\n'
                  << "Verdict: " << judge::toString(report.verdict) << '\n';
        if (!report.detail.empty()) {
            std::cout << "Detail: " << report.detail << '\n';
        }
        for (const judge::TestResult& test : report.tests) {
            std::cout << "  [" << judge::toString(test.verdict) << "] "
                      << test.name << " (" << test.elapsed.count() << " ms)";
            if (!test.detail.empty()) {
                std::cout << ": " << test.detail;
            }
            std::cout << '\n';
        }
        return report.verdict == judge::Verdict::kAccepted ? 0 : 1;
    }

    if (command == "stress") {
        rejectNoProgressOption(arguments);
        if (arguments.positional.size() != 3) {
            throw std::runtime_error(
                "stress requires a problem ID and a C++ source path"
            );
        }
        const judge::Problem problem = catalog.findById(arguments.positional[1]);
        const std::uint64_t seed = arguments.seed.value_or(createSeed());
        const int caseCount =
            arguments.caseCount.value_or(problem.metadata.randomTests);
        const judge::StressReport report =
            judge::StressService(arguments.projectRoot).run(
                problem,
                pathFromUtf8(arguments.positional[2]),
                seed,
                caseCount
            );

        std::cout << "Problem: " << report.judge.problemId << '\n'
                  << "Seed: " << report.seed << '\n'
                  << "Cases: " << report.generatedCases << '\n'
                  << "Operations per case: " << report.operationsPerCase << '\n'
                  << "Verdict: " << judge::toString(report.judge.verdict) << '\n'
                  << "Detail: " << report.judge.detail << '\n';

        if (report.failingCaseSeed.has_value()) {
            const judge::TestResult& failed = report.judge.tests.back();
            std::cout
                << "Failing case: " << failed.name << '\n'
                << "Case seed: " << *report.failingCaseSeed << '\n'
                << "Replay: algo stress " << report.judge.problemId << " \""
                << arguments.positional[2] << "\" --seed " << report.seed
                << " --cases " << report.generatedCases << '\n'
                << "Failing input:\n"
                << report.failingInput;
        }
        return report.judge.verdict == judge::Verdict::kAccepted ? 0 : 1;
    }

    throw std::runtime_error("unknown command: " + command);
}

}  // namespace

int main(int argc, char* argv[]) {
    try {
        return runCommand(parseArguments(collectArguments(argc, argv)));
    } catch (const std::exception& error) {
        std::cerr << "algo: " << error.what() << '\n';
        return 2;
    }
}
