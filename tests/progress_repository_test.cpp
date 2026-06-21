#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

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

        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kWrongAnswer,
            std::chrono::milliseconds(80)
        );
        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kAccepted,
            std::chrono::milliseconds(40)
        );
        repository.recordAttempt(
            "F03-stack-array", judge::Verdict::kAccepted,
            std::chrono::milliseconds(60)
        );
        const auto records = repository.load();
        require(records.size() == 1, "expected one record");
        require(records[0].attempts == 3, "attempt count mismatch");
        require(records[0].acceptedAttempts == 2, "accepted count mismatch");
        require(records[0].bestTimeMs == 40, "best time must retain minimum");
        require(records[0].lastVerdict == "AC", "last verdict mismatch");
        require(!std::filesystem::exists(repository.filePath().string() + ".tmp"),
                "temporary file must not remain after save");

        {
            std::ofstream invalid(repository.filePath(), std::ios::trunc);
            invalid << "{\"schema_version\":2,\"records\":[]}";
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
        std::filesystem::remove_all(root);
        return 0;
    } catch (const std::exception& error) {
        std::filesystem::remove_all(root);
        std::cerr << error.what() << '\n';
        return 1;
    }
}
