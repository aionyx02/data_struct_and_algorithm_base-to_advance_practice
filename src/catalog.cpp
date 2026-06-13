#include "judge/catalog.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>
#include <variant>

namespace judge {
namespace {

class JsonValue {
public:
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;
    using Storage =
        std::variant<std::nullptr_t, bool, std::int64_t, std::string, Array, Object>;

    explicit JsonValue(Storage storage) : storage_(std::move(storage)) {}

    [[nodiscard]] const std::string& asString() const {
        return std::get<std::string>(storage_);
    }

    [[nodiscard]] std::int64_t asInteger() const {
        return std::get<std::int64_t>(storage_);
    }

    [[nodiscard]] const Array& asArray() const {
        return std::get<Array>(storage_);
    }

    [[nodiscard]] const Object& asObject() const {
        return std::get<Object>(storage_);
    }

private:
    Storage storage_;
};

class JsonParser {
public:
    explicit JsonParser(std::string_view input) : input_(input) {}

    [[nodiscard]] JsonValue parse() {
        skipWhitespace();
        JsonValue value = parseValue();
        skipWhitespace();
        if (position_ != input_.size()) {
            fail("unexpected trailing data");
        }
        return value;
    }

private:
    [[nodiscard]] JsonValue parseValue() {
        if (position_ >= input_.size()) {
            fail("unexpected end of input");
        }

        switch (input_[position_]) {
            case '{':
                return JsonValue(parseObject());
            case '[':
                return JsonValue(parseArray());
            case '"':
                return JsonValue(parseString());
            case 't':
                consumeLiteral("true");
                return JsonValue(true);
            case 'f':
                consumeLiteral("false");
                return JsonValue(false);
            case 'n':
                consumeLiteral("null");
                return JsonValue(nullptr);
            default:
                if (input_[position_] == '-' ||
                    std::isdigit(static_cast<unsigned char>(input_[position_]))) {
                    return JsonValue(parseInteger());
                }
                fail("unexpected token");
        }
    }

    [[nodiscard]] JsonValue::Object parseObject() {
        expect('{');
        skipWhitespace();
        JsonValue::Object object;
        if (consumeIf('}')) {
            return object;
        }

        while (true) {
            skipWhitespace();
            if (position_ >= input_.size() || input_[position_] != '"') {
                fail("object key must be a string");
            }
            std::string key = parseString();
            skipWhitespace();
            expect(':');
            skipWhitespace();
            if (!object.emplace(std::move(key), parseValue()).second) {
                fail("duplicate object key");
            }
            skipWhitespace();
            if (consumeIf('}')) {
                break;
            }
            expect(',');
        }
        return object;
    }

    [[nodiscard]] JsonValue::Array parseArray() {
        expect('[');
        skipWhitespace();
        JsonValue::Array array;
        if (consumeIf(']')) {
            return array;
        }

        while (true) {
            skipWhitespace();
            array.push_back(parseValue());
            skipWhitespace();
            if (consumeIf(']')) {
                break;
            }
            expect(',');
        }
        return array;
    }

    [[nodiscard]] std::string parseString() {
        expect('"');
        std::string value;
        while (position_ < input_.size()) {
            const char current = input_[position_++];
            if (current == '"') {
                return value;
            }
            if (current != '\\') {
                if (static_cast<unsigned char>(current) < 0x20U) {
                    fail("control character in string");
                }
                value.push_back(current);
                continue;
            }

            if (position_ >= input_.size()) {
                fail("unfinished string escape");
            }
            const char escaped = input_[position_++];
            switch (escaped) {
                case '"':
                case '\\':
                case '/':
                    value.push_back(escaped);
                    break;
                case 'b':
                    value.push_back('\b');
                    break;
                case 'f':
                    value.push_back('\f');
                    break;
                case 'n':
                    value.push_back('\n');
                    break;
                case 'r':
                    value.push_back('\r');
                    break;
                case 't':
                    value.push_back('\t');
                    break;
                default:
                    fail("unsupported string escape");
            }
        }
        fail("unterminated string");
    }

    [[nodiscard]] std::int64_t parseInteger() {
        const std::size_t start = position_;
        if (consumeIf('-') && position_ >= input_.size()) {
            fail("incomplete number");
        }
        if (position_ >= input_.size() ||
            !std::isdigit(static_cast<unsigned char>(input_[position_]))) {
            fail("invalid number");
        }
        if (input_[position_] == '0') {
            ++position_;
        } else {
            while (position_ < input_.size() &&
                   std::isdigit(static_cast<unsigned char>(input_[position_]))) {
                ++position_;
            }
        }
        if (position_ < input_.size() &&
            (input_[position_] == '.' || input_[position_] == 'e' ||
             input_[position_] == 'E')) {
            fail("metadata numbers must be integers");
        }
        try {
            return std::stoll(std::string(input_.substr(start, position_ - start)));
        } catch (const std::exception&) {
            fail("integer is out of range");
        }
    }

    void consumeLiteral(std::string_view literal) {
        if (input_.substr(position_, literal.size()) != literal) {
            fail("invalid literal");
        }
        position_ += literal.size();
    }

    void skipWhitespace() {
        while (position_ < input_.size() &&
               std::isspace(static_cast<unsigned char>(input_[position_]))) {
            ++position_;
        }
    }

    void expect(char expected) {
        if (!consumeIf(expected)) {
            fail(std::string("expected '") + expected + "'");
        }
    }

    bool consumeIf(char expected) {
        if (position_ < input_.size() && input_[position_] == expected) {
            ++position_;
            return true;
        }
        return false;
    }

    [[noreturn]] void fail(const std::string& message) const {
        throw std::runtime_error(
            "invalid JSON at byte " + std::to_string(position_) + ": " + message
        );
    }

    std::string_view input_;
    std::size_t position_ = 0;
};

std::string readFile(const std::filesystem::path& path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        throw std::runtime_error("cannot read file: " + path.string());
    }
    std::ostringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}

const JsonValue& requireField(
    const JsonValue::Object& object,
    const std::string& key
) {
    const auto found = object.find(key);
    if (found == object.end()) {
        throw std::runtime_error("problem metadata is missing field: " + key);
    }
    return found->second;
}

std::string requireString(
    const JsonValue::Object& object,
    const std::string& key
) {
    try {
        return requireField(object, key).asString();
    } catch (const std::bad_variant_access&) {
        throw std::runtime_error("problem metadata field must be a string: " + key);
    }
}

int requirePositiveInteger(
    const JsonValue::Object& object,
    const std::string& key
) {
    try {
        const std::int64_t value = requireField(object, key).asInteger();
        if (value <= 0 || value > static_cast<std::int64_t>(INT32_MAX)) {
            throw std::runtime_error(
                "problem metadata field must be a positive integer: " + key
            );
        }
        return static_cast<int>(value);
    } catch (const std::bad_variant_access&) {
        throw std::runtime_error("problem metadata field must be an integer: " + key);
    }
}

std::vector<std::string> requireStringArray(
    const JsonValue::Object& object,
    const std::string& key
) {
    std::vector<std::string> values;
    try {
        for (const JsonValue& value : requireField(object, key).asArray()) {
            values.push_back(value.asString());
        }
    } catch (const std::bad_variant_access&) {
        throw std::runtime_error(
            "problem metadata field must be an array of strings: " + key
        );
    }
    return values;
}

bool isWithin(
    const std::filesystem::path& child,
    const std::filesystem::path& parent
) {
    const std::filesystem::path relative = child.lexically_relative(parent);
    return !relative.empty() && *relative.begin() != "..";
}

Problem loadProblem(
    const std::filesystem::path& metadataPath,
    const std::filesystem::path& problemsRoot
) {
    const JsonValue root = JsonParser(readFile(metadataPath)).parse();
    JsonValue::Object object;
    try {
        object = root.asObject();
    } catch (const std::bad_variant_access&) {
        throw std::runtime_error(
            "problem metadata root must be an object: " + metadataPath.string()
        );
    }

    Problem problem;
    problem.metadata.id = requireString(object, "id");
    problem.metadata.title = requireString(object, "title");
    problem.metadata.stage = requireString(object, "stage");
    problem.metadata.difficulty = requireString(object, "difficulty");
    problem.metadata.judgeMode = requireString(object, "judge_mode");
    problem.metadata.timeLimitMs = requirePositiveInteger(object, "time_limit_ms");
    problem.metadata.memoryLimitMb =
        requirePositiveInteger(object, "memory_limit_mb");
    problem.metadata.outputLimitBytes = static_cast<std::size_t>(
        requirePositiveInteger(object, "output_limit_bytes")
    );
    problem.metadata.expectedTime = requireString(object, "expected_time");
    problem.metadata.expectedSpace = requireString(object, "expected_space");
    problem.metadata.forbiddenSymbols =
        requireStringArray(object, "forbidden_symbols");
    problem.metadata.randomTests =
        requirePositiveInteger(object, "random_tests");
    problem.metadata.operationLimit =
        requirePositiveInteger(object, "operation_limit");
    problem.metadata.testsDirectory = requireString(object, "tests_directory");

    if (problem.metadata.id.empty() ||
        !std::all_of(
            problem.metadata.id.begin(),
            problem.metadata.id.end(),
            [](char character) {
                return std::isalnum(static_cast<unsigned char>(character)) ||
                       character == '-' || character == '_';
            }
        )) {
        throw std::runtime_error("problem ID contains unsupported characters");
    }
    if (problem.metadata.judgeMode != "stdio") {
        throw std::runtime_error(
            "this Judge slice supports only stdio problems: " + problem.metadata.id
        );
    }

    problem.directory = std::filesystem::weakly_canonical(metadataPath.parent_path());
    if (!isWithin(problem.directory, problemsRoot)) {
        throw std::runtime_error("problem directory escapes problems root");
    }
    if (problem.directory.filename().string() != problem.metadata.id) {
        throw std::runtime_error(
            "problem directory name must match ID: " + problem.metadata.id
        );
    }

    problem.statementPath = problem.directory / "statement.md";
    problem.testsPath = std::filesystem::weakly_canonical(
        problem.directory / problem.metadata.testsDirectory
    );
    if (!isWithin(problem.testsPath, problem.directory)) {
        throw std::runtime_error("tests directory escapes problem directory");
    }
    if (!std::filesystem::is_regular_file(problem.statementPath)) {
        throw std::runtime_error(
            "problem statement is missing: " + problem.statementPath.string()
        );
    }
    if (!std::filesystem::is_directory(problem.testsPath)) {
        throw std::runtime_error(
            "problem tests directory is missing: " + problem.testsPath.string()
        );
    }

    std::size_t inputCount = 0;
    for (const auto& entry : std::filesystem::directory_iterator(problem.testsPath)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".in") {
            continue;
        }
        ++inputCount;
        std::filesystem::path expected = entry.path();
        expected.replace_extension(".out");
        if (!std::filesystem::is_regular_file(expected)) {
            throw std::runtime_error(
                "test output is missing for: " + entry.path().string()
            );
        }
    }
    if (inputCount == 0) {
        throw std::runtime_error(
            "problem has no input tests: " + problem.metadata.id
        );
    }

    return problem;
}

}  // namespace

ProblemCatalog::ProblemCatalog(std::filesystem::path projectRoot)
    : projectRoot_(std::filesystem::weakly_canonical(std::move(projectRoot))),
      problemsRoot_(std::filesystem::weakly_canonical(projectRoot_ / "problems")) {
    if (!std::filesystem::is_directory(problemsRoot_)) {
        throw std::runtime_error(
            "problems directory does not exist: " + problemsRoot_.string()
        );
    }
}

const std::filesystem::path& ProblemCatalog::projectRoot() const noexcept {
    return projectRoot_;
}

std::vector<Problem> ProblemCatalog::list() const {
    std::vector<Problem> problems;
    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(problemsRoot_)) {
        if (entry.is_regular_file() && entry.path().filename() == "problem.json") {
            problems.push_back(loadProblem(entry.path(), problemsRoot_));
        }
    }
    std::sort(
        problems.begin(),
        problems.end(),
        [](const Problem& left, const Problem& right) {
            return left.metadata.id < right.metadata.id;
        }
    );
    for (std::size_t index = 1; index < problems.size(); ++index) {
        if (problems[index - 1].metadata.id == problems[index].metadata.id) {
            throw std::runtime_error(
                "duplicate problem ID: " + problems[index].metadata.id
            );
        }
    }
    return problems;
}

Problem ProblemCatalog::findById(const std::string& id) const {
    const std::vector<Problem> problems = list();
    const auto found = std::find_if(
        problems.begin(),
        problems.end(),
        [&id](const Problem& problem) { return problem.metadata.id == id; }
    );
    if (found == problems.end()) {
        throw std::runtime_error("unknown problem ID: " + id);
    }
    return *found;
}

}  // namespace judge
