#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>

#include "judge/judge_service.hpp"
#include "judge/problem.hpp"

namespace judge {

struct StressReport {
    JudgeReport judge;
    std::uint64_t seed = 0;
    int generatedCases = 0;
    int operationsPerCase = 0;
    std::optional<std::uint64_t> failingCaseSeed;
    std::string failingInput;
};

class StressService {
public:
    explicit StressService(std::filesystem::path projectRoot);

    [[nodiscard]] StressReport run(
        const Problem& problem,
        const std::filesystem::path& sourcePath,
        std::uint64_t seed,
        int caseCount
    ) const;

private:
    std::filesystem::path projectRoot_;
    std::filesystem::path stressRoot_;
};

}  // namespace judge
