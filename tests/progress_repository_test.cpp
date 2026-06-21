#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#include "judge/progress.hpp"

namespace {
void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}
}

int main() {
    const auto unique = std::chrono::steady_clock::now().time_since_epoch().count();
    const std::filesystem::path root =
        std::filesystem::temp_directory_path() /
        ("data-structures-progress-test-" + std::to_string(unique));
    try {
        std::filesystem::create_directories(root);
        const judge::ProgressRepository repository(root);
        require(repository.load().empty(), "new repository must be empty");

        const std::chrono::sys_days day20000{std::chrono::days{20000}};
        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kWrongAnswer,
            std::chrono::milliseconds(80), day20000
        );
        const auto afterWrongAnswer = repository.load();
        require(afterWrongAnswer[0].reviewStreak == 0,
                "non-AC must reset the review streak");
        require(afterWrongAnswer[0].nextReviewDay == 20001,
                "non-AC must schedule one day later");
        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kAccepted,
            std::chrono::milliseconds(40), day20000 + std::chrono::days{1}
        );
        const auto afterFirstAccepted = repository.load();
        require(afterFirstAccepted[0].reviewStreak == 1,
                "first AC must start the review streak");
        require(afterFirstAccepted[0].nextReviewDay == 20008,
                "first AC must schedule seven days later");
        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kAccepted,
            std::chrono::milliseconds(60), day20000 + std::chrono::days{8}
        );
        const auto records = repository.load();
        require(records.size() == 1, "expected one record");
        require(records[0].attempts == 3, "attempt count mismatch");
        require(records[0].acceptedAttempts == 2, "accepted count mismatch");
        require(records[0].bestTimeMs == 40, "best time must retain minimum");
        require(records[0].lastVerdict == "AC", "last verdict mismatch");
        require(records[0].reviewStreak == 2, "review streak mismatch");
        require(records[0].lastAttemptDay == 20008, "last attempt day mismatch");
        require(records[0].nextReviewDay == 20038, "30-day review mismatch");
        require(!std::filesystem::exists(repository.filePath().string() + ".tmp"),
                "temporary file must not remain after save");

        {
            std::ofstream invalid(repository.filePath(), std::ios::trunc);
            invalid << "{\"schema_version\":3,\"records\":[]}";
        }
        bool rejected = false;
        try { static_cast<void>(repository.load()); }
        catch (const std::runtime_error&) { rejected = true; }
        require(rejected, "unsupported schema must be rejected");

        {
            std::ofstream invalid(repository.filePath(), std::ios::trunc);
            invalid << "{\"schema_version\":1,\"unknown\":[]}";
        }
        rejected = false;
        try { static_cast<void>(repository.load()); }
        catch (const std::runtime_error&) { rejected = true; }
        require(rejected, "unknown fields must be rejected");

        {
            std::ofstream legacy(repository.filePath(), std::ios::trunc);
            legacy << R"({"schema_version":1,"records":[{"problem_id":"F03-stack-array","attempts":1,"accepted_attempts":1,"best_time_ms":50,"last_verdict":"AC"}]})";
        }
        const auto legacyRecords = repository.load();
        require(legacyRecords.size() == 1, "schema 1 record must load");
        require(legacyRecords[0].reviewStreak == 0,
                "schema 1 record must start without a review streak");
        require(!legacyRecords[0].lastAttemptDay.has_value(),
                "schema 1 record must not invent an attempt day");
        require(!legacyRecords[0].nextReviewDay.has_value(),
                "schema 1 record must not invent a review day");

        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kAccepted,
            std::chrono::milliseconds(45), day20000
        );
        const auto migratedRecords = repository.load();
        require(migratedRecords[0].reviewStreak == 1,
                "first AC after migration must start the review streak");
        require(migratedRecords[0].nextReviewDay == 20007,
                "first AC after migration must schedule seven days later");
        std::ifstream migrated(repository.filePath());
        const std::string migratedJson{
            std::istreambuf_iterator<char>(migrated),
            std::istreambuf_iterator<char>()
        };
        require(migratedJson.find("\"schema_version\": 2") != std::string::npos,
                "schema 1 must migrate to schema 2 on write");
        migrated.close();
        std::filesystem::remove_all(root);
        return 0;
    } catch (const std::exception& error) {
        std::filesystem::remove_all(root);
        std::cerr << error.what() << '\n';
        return 1;
    }
}
