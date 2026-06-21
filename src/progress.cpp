#include "judge/progress.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string_view>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace judge {
namespace {

constexpr int kSchemaVersion = 1;

std::string readText(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("cannot read progress file: " + path.string());
    std::ostringstream content;
    content << input.rdbuf();
    return content.str();
}

std::string escapeJson(std::string_view value) {
    std::string result;
    for (char character : value) {
        if (character == '"' || character == '\\') result.push_back('\\');
        result.push_back(character);
    }
    return result;
}

class Parser {
public:
    explicit Parser(std::string_view input) : input_(input) {}

    std::vector<ProgressRecord> parse() {
        expect('{');
        expectKey("schema_version");
        const int version = integer();
        if (version != kSchemaVersion) {
            throw error("unsupported schema_version " + std::to_string(version));
        }
        expect(',');
        expectKey("records");
        expect('[');
        std::vector<ProgressRecord> records;
        if (!consume(']')) {
            do { records.push_back(record()); } while (consume(','));
            expect(']');
        }
        expect('}');
        whitespace();
        if (position_ != input_.size()) throw error("trailing content");
        return records;
    }

private:
    ProgressRecord record() {
        ProgressRecord value;
        expect('{');
        expectKey("problem_id"); value.problemId = string(); expect(',');
        expectKey("attempts"); value.attempts = nonNegativeInteger(); expect(',');
        expectKey("accepted_attempts");
        value.acceptedAttempts = nonNegativeInteger(); expect(',');
        expectKey("best_time_ms");
        if (consumeLiteral("null")) value.bestTimeMs.reset();
        else value.bestTimeMs = nonNegativeInteger();
        expect(',');
        expectKey("last_verdict"); value.lastVerdict = string();
        expect('}');
        if (value.problemId.empty()) throw error("problem_id cannot be empty");
        if (value.acceptedAttempts > value.attempts) {
            throw error("accepted_attempts exceeds attempts");
        }
        return value;
    }

    void whitespace() {
        while (position_ < input_.size() &&
               std::isspace(static_cast<unsigned char>(input_[position_]))) ++position_;
    }
    bool consume(char expected) {
        whitespace();
        if (position_ < input_.size() && input_[position_] == expected) {
            ++position_; return true;
        }
        return false;
    }
    void expect(char expected) {
        if (!consume(expected)) throw error(std::string("expected '") + expected + "'");
    }
    bool consumeLiteral(std::string_view value) {
        whitespace();
        if (input_.substr(position_, value.size()) == value) {
            position_ += value.size(); return true;
        }
        return false;
    }
    void expectKey(std::string_view expected) {
        const std::string actual = string();
        if (actual != expected) throw error("expected field " + std::string(expected));
        expect(':');
    }
    std::string string() {
        whitespace(); expect('"');
        std::string result;
        while (position_ < input_.size()) {
            char character = input_[position_++];
            if (character == '"') return result;
            if (character == '\\') {
                if (position_ >= input_.size()) throw error("incomplete escape");
                character = input_[position_++];
                if (character != '"' && character != '\\') {
                    throw error("unsupported string escape");
                }
            }
            result.push_back(character);
        }
        throw error("unterminated string");
    }
    int integer() {
        whitespace();
        bool negative = false;
        if (position_ < input_.size() && input_[position_] == '-') {
            negative = true; ++position_;
        }
        if (position_ >= input_.size() || !std::isdigit(static_cast<unsigned char>(input_[position_]))) {
            throw error("expected integer");
        }
        long long value = 0;
        while (position_ < input_.size() &&
               std::isdigit(static_cast<unsigned char>(input_[position_]))) {
            value = value * 10 + (input_[position_++] - '0');
            if (value > 2147483647LL) throw error("integer out of range");
        }
        return static_cast<int>(negative ? -value : value);
    }
    int nonNegativeInteger() {
        const int value = integer();
        if (value < 0) throw error("expected non-negative integer");
        return value;
    }
    std::runtime_error error(const std::string& detail) const {
        return std::runtime_error(
            "invalid progress file at byte " + std::to_string(position_) + ": " + detail
        );
    }

    std::string_view input_;
    std::size_t position_ = 0;
};

std::string serialize(const std::vector<ProgressRecord>& records) {
    std::ostringstream output;
    output << "{\n  \"schema_version\": 1,\n  \"records\": [";
    for (std::size_t index = 0; index < records.size(); ++index) {
        const ProgressRecord& record = records[index];
        output << (index == 0 ? "\n" : ",\n")
               << "    {\"problem_id\": \"" << escapeJson(record.problemId)
               << "\", \"attempts\": " << record.attempts
               << ", \"accepted_attempts\": " << record.acceptedAttempts
               << ", \"best_time_ms\": ";
        if (record.bestTimeMs.has_value()) output << *record.bestTimeMs;
        else output << "null";
        output << ", \"last_verdict\": \"" << escapeJson(record.lastVerdict) << "\"}";
    }
    output << (records.empty() ? "" : "\n") << "  ]\n}\n";
    return output.str();
}

void replaceFile(const std::filesystem::path& temporary, const std::filesystem::path& target) {
#ifdef _WIN32
    if (!MoveFileExW(
            temporary.c_str(),
            target.c_str(),
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
        )) {
        throw std::runtime_error(
            "cannot atomically replace progress file (Windows error " +
            std::to_string(GetLastError()) + ")"
        );
    }
#else
    std::filesystem::rename(temporary, target);
#endif
}

}  // namespace

ProgressRepository::ProgressRepository(std::filesystem::path projectRoot)
    : projectRoot_(std::filesystem::weakly_canonical(std::move(projectRoot))),
      filePath_(projectRoot_ / ".judge" / "progress.json") {}

std::vector<ProgressRecord> ProgressRepository::load() const {
    if (!std::filesystem::exists(filePath_)) return {};
    std::vector<ProgressRecord> records = Parser(readText(filePath_)).parse();
    std::sort(records.begin(), records.end(), [](const auto& left, const auto& right) {
        return left.problemId < right.problemId;
    });
    if (std::adjacent_find(records.begin(), records.end(), [](const auto& left, const auto& right) {
            return left.problemId == right.problemId;
        }) != records.end()) {
        throw std::runtime_error("invalid progress file: duplicate problem_id");
    }
    return records;
}

void ProgressRepository::recordAttempt(
    const std::string& problemId,
    Verdict verdict,
    std::chrono::milliseconds elapsed
) const {
    std::vector<ProgressRecord> records = load();
    auto position = std::lower_bound(records.begin(), records.end(), problemId,
        [](const ProgressRecord& record, const std::string& id) {
            return record.problemId < id;
        });
    if (position == records.end() || position->problemId != problemId) {
        position = records.insert(position, ProgressRecord{
            .problemId = problemId,
            .attempts = 0,
            .acceptedAttempts = 0,
            .bestTimeMs = std::nullopt,
            .lastVerdict = {},
        });
    }
    ++position->attempts;
    position->lastVerdict = toString(verdict);
    if (verdict == Verdict::kAccepted) {
        ++position->acceptedAttempts;
        const long long milliseconds = elapsed.count();
        if (!position->bestTimeMs.has_value() || milliseconds < *position->bestTimeMs) {
            position->bestTimeMs = milliseconds;
        }
    }
    save(records);
}

const std::filesystem::path& ProgressRepository::filePath() const { return filePath_; }

void ProgressRepository::save(const std::vector<ProgressRecord>& records) const {
    std::filesystem::create_directories(filePath_.parent_path());
    const std::filesystem::path temporary = filePath_.string() + ".tmp";
    try {
        std::ofstream output(temporary, std::ios::binary | std::ios::trunc);
        if (!output) throw std::runtime_error("cannot create progress temporary file");
        output << serialize(records);
        output.flush();
        if (!output) throw std::runtime_error("cannot flush progress temporary file");
        output.close();
        replaceFile(temporary, filePath_);
    } catch (...) {
        std::error_code ignored;
        std::filesystem::remove(temporary, ignored);
        throw;
    }
}

}  // namespace judge
