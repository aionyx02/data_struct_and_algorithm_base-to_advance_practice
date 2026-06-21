#pragma once

#include <chrono>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "judge/judge_service.hpp"

namespace judge {

struct ProgressRecord {
    std::string problemId;
    int attempts = 0;
    int acceptedAttempts = 0;
    std::optional<long long> bestTimeMs;
    std::string lastVerdict;
    int reviewStreak = 0;
    std::optional<int> lastAttemptDay;
    std::optional<int> nextReviewDay;
};

class ProgressRepository {
public:
    explicit ProgressRepository(std::filesystem::path projectRoot);

    [[nodiscard]] std::vector<ProgressRecord> load() const;
    void recordAttempt(
        const std::string& problemId,
        Verdict verdict,
        std::chrono::milliseconds elapsed,
        std::chrono::sys_days attemptDay =
            std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    ) const;
    [[nodiscard]] const std::filesystem::path& filePath() const;

private:
    void save(const std::vector<ProgressRecord>& records) const;

    std::filesystem::path projectRoot_;
    std::filesystem::path filePath_;
};

}  // namespace judge
