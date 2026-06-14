#include "judge/stress_service.hpp"

#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "judge/case_generator.hpp"
#include "judge/judge_service.hpp"

namespace judge {
namespace {
bool isWithin(
    const std::filesystem::path& child,
    const std::filesystem::path& parent
) {
    const std::filesystem::path relative = child.lexically_relative(parent);
    return !relative.empty() && *relative.begin() != "..";
}

std::filesystem::path createStressDirectory(
    const std::filesystem::path& stressRoot
) {
    static std::atomic<unsigned long long> sequence = 0;
    const auto timestamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    const std::filesystem::path directory =
        stressRoot /
        ("stress-" + std::to_string(timestamp) + "-" +
         std::to_string(sequence.fetch_add(1)));
    std::filesystem::create_directories(directory);
    return std::filesystem::weakly_canonical(directory);
}

class StressDirectoryGuard {
public:
    StressDirectoryGuard(
        std::filesystem::path directory,
        std::filesystem::path stressRoot
    )
        : directory_(std::move(directory)),
          stressRoot_(std::move(stressRoot)) {}

    ~StressDirectoryGuard() {
        if (!isWithin(directory_, stressRoot_)) {
            return;
        }
        std::error_code error;
        std::filesystem::remove_all(directory_, error);
    }

private:
    std::filesystem::path directory_;
    std::filesystem::path stressRoot_;
};

void writeFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot write generated stress case");
    }
    stream << content;
    if (!stream) {
        throw std::runtime_error("cannot finish writing generated stress case");
    }
}

std::string caseName(int index) {
    std::ostringstream name;
    name << "random-" << std::setw(6) << std::setfill('0') << index;
    return name.str();
}

}  // namespace

StressService::StressService(std::filesystem::path projectRoot)
    : projectRoot_(std::filesystem::weakly_canonical(std::move(projectRoot))),
      stressRoot_(projectRoot_ / ".judge" / "stress") {
    std::filesystem::create_directories(stressRoot_);
    stressRoot_ = std::filesystem::weakly_canonical(stressRoot_);
    if (!isWithin(stressRoot_, projectRoot_)) {
        throw std::runtime_error("stress directory escapes project root");
    }
}

StressReport StressService::run(
    const Problem& problem,
    const std::filesystem::path& sourcePath,
    std::uint64_t seed,
    int caseCount
) const {
    if (caseCount <= 0 || caseCount > 10000) {
        throw std::runtime_error("stress case count must be between 1 and 10000");
    }

    const std::filesystem::path directory =
        createStressDirectory(stressRoot_);
    StressDirectoryGuard cleanup(directory, stressRoot_);

    std::mt19937_64 masterRandom(seed);
    std::vector<std::uint64_t> caseSeeds;
    std::vector<std::string> caseInputs;
    caseSeeds.reserve(static_cast<std::size_t>(caseCount));
    caseInputs.reserve(static_cast<std::size_t>(caseCount));

    for (int index = 0; index < caseCount; ++index) {
        const std::uint64_t caseSeed = masterRandom();
        const GeneratedCase generated =
            generateCase(
                problem.metadata.id,
                caseSeed,
                problem.metadata.operationLimit
            );
        const std::string name = caseName(index);
        writeFile(directory / (name + ".in"), generated.input);
        writeFile(directory / (name + ".out"), generated.expectedOutput);
        caseSeeds.push_back(caseSeed);
        caseInputs.push_back(generated.input);
    }

    Problem generatedProblem = problem;
    generatedProblem.testsPath = directory;
    JudgeReport judge =
        JudgeService(projectRoot_).judge(generatedProblem, sourcePath);

    StressReport report{
        .judge = std::move(judge),
        .seed = seed,
        .generatedCases = caseCount,
        .operationsPerCase = problem.metadata.operationLimit,
        .failingCaseSeed = std::nullopt,
        .failingInput = {},
    };

    if (report.judge.verdict == Verdict::kAccepted) {
        report.judge.detail =
            std::to_string(caseCount) + " random cases passed (" +
            std::to_string(
                static_cast<long long>(caseCount) *
                problem.metadata.operationLimit
            ) +
            " operations)";
        return report;
    }

    if (!report.judge.tests.empty()) {
        const std::size_t failingIndex = report.judge.tests.size() - 1;
        if (failingIndex < caseSeeds.size()) {
            report.failingCaseSeed = caseSeeds[failingIndex];
            report.failingInput = caseInputs[failingIndex];
        }
    }
    return report;
}

}  // namespace judge
