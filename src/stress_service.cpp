#include "judge/stress_service.hpp"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <utility>
#include <vector>

namespace judge {
namespace {

struct GeneratedCase {
    std::string input;
    std::string expectedOutput;
};

int randomInt(std::mt19937_64& random, int minimum, int maximum) {
    return std::uniform_int_distribution<int>(minimum, maximum)(random);
}

int randomValue(std::mt19937_64& random) {
    return randomInt(random, -1000, 1000);
}

void appendLine(std::ostringstream& output, const std::string& value) {
    output << value << '\n';
}

void appendLine(std::ostringstream& output, int value) {
    output << value << '\n';
}

GeneratedCase generateArrayAdt(std::mt19937_64& random, int operationCount) {
    const int size = randomInt(random, 0, 20);
    std::vector<int> values(static_cast<std::size_t>(size));

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
        if (index != 0) {
            input << ' ';
        }
        input << values[static_cast<std::size_t>(index)];
    }
    input << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 2: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "swap " << left << ' ' << right << '\n';
                if (left < 0 || left >= size || right < 0 || right >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    std::swap(
                        values[static_cast<std::size_t>(left)],
                        values[static_cast<std::size_t>(right)]
                    );
                }
                break;
            }
            case 3:
                input << "front\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.front());
                }
                break;
            case 4:
                input << "back\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            default:
                input << "size\n";
                appendLine(output, size);
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateDynamicArray(
    std::mt19937_64& random,
    int operationCount
) {
    int capacity = randomInt(random, 1, 8);
    std::vector<int> values;
    values.reserve(static_cast<std::size_t>(capacity));

    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int value = randomValue(random);
                input << "push " << value << '\n';
                if (static_cast<int>(values.size()) == capacity) {
                    capacity *= 2;
                    values.reserve(static_cast<std::size_t>(capacity));
                }
                values.push_back(value);
                break;
            }
            case 1:
                input << "pop\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                    values.pop_back();
                }
                break;
            case 2: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 3: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 4:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 5:
                input << "capacity\n";
                appendLine(output, capacity);
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateStack(std::mt19937_64& random, int operationCount) {
    const int capacity = randomInt(random, 1, 12);
    std::vector<int> values;
    values.reserve(static_cast<std::size_t>(capacity));

    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 4)) {
            case 0: {
                const int value = randomValue(random);
                input << "push " << value << '\n';
                if (static_cast<int>(values.size()) == capacity) {
                    appendLine(output, "FULL");
                } else {
                    values.push_back(value);
                }
                break;
            }
            case 1:
                input << "pop\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                    values.pop_back();
                }
                break;
            case 2:
                input << "top\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            case 3:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            default:
                input << "empty\n";
                appendLine(output, values.empty() ? "true" : "false");
                break;
        }
    }
    return {input.str(), output.str()};
}

void queueOperation(
    std::mt19937_64& random,
    int capacity,
    std::deque<int>& values,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 5)) {
        case 0: {
            const int value = randomValue(random);
            input << "enqueue " << value << '\n';
            if (static_cast<int>(values.size()) == capacity) {
                appendLine(output, "FULL");
            } else {
                values.push_back(value);
            }
            break;
        }
        case 1:
            input << "dequeue\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
                values.pop_front();
            }
            break;
        case 2:
            input << "front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
            }
            break;
        case 3:
            input << "back\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.back());
            }
            break;
        case 4:
            input << "size\n";
            appendLine(output, static_cast<int>(values.size()));
            break;
        default:
            input << "empty\n";
            appendLine(output, values.empty() ? "true" : "false");
            break;
    }
}

GeneratedCase generateCircularQueue(
    std::mt19937_64& random,
    int operationCount
) {
    constexpr int kMaximumPreludeOperations = 11;
    if (operationCount < kMaximumPreludeOperations) {
        throw std::runtime_error(
            "F04-circular-queue requires operation_limit of at least 11"
        );
    }

    const int capacity = randomInt(random, 1, 8);
    std::deque<int> values;

    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int index = 0; index < capacity; ++index) {
        const int value = randomValue(random);
        input << "enqueue " << value << '\n';
        values.push_back(value);
    }
    input << "dequeue\n";
    appendLine(output, values.front());
    values.pop_front();
    const int replacement = randomValue(random);
    input << "enqueue " << replacement << '\n';
    values.push_back(replacement);
    input << "back\n";
    appendLine(output, values.back());

    const int preludeOperations = capacity + 3;
    for (int operation = preludeOperations;
         operation < operationCount;
         ++operation) {
        queueOperation(random, capacity, values, input, output);
    }
    return {input.str(), output.str()};
}

void dequeOperation(
    std::mt19937_64& random,
    int capacity,
    std::deque<int>& values,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 7)) {
        case 0: {
            const int value = randomValue(random);
            input << "push_front " << value << '\n';
            if (static_cast<int>(values.size()) == capacity) {
                appendLine(output, "FULL");
            } else {
                values.push_front(value);
            }
            break;
        }
        case 1: {
            const int value = randomValue(random);
            input << "push_back " << value << '\n';
            if (static_cast<int>(values.size()) == capacity) {
                appendLine(output, "FULL");
            } else {
                values.push_back(value);
            }
            break;
        }
        case 2:
            input << "pop_front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
                values.pop_front();
            }
            break;
        case 3:
            input << "pop_back\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.back());
                values.pop_back();
            }
            break;
        case 4:
            input << "front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
            }
            break;
        case 5:
            input << "back\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.back());
            }
            break;
        case 6:
            input << "size\n";
            appendLine(output, static_cast<int>(values.size()));
            break;
        default:
            input << "empty\n";
            appendLine(output, values.empty() ? "true" : "false");
            break;
    }
}

GeneratedCase generateArrayDeque(
    std::mt19937_64& random,
    int operationCount
) {
    const int capacity = randomInt(random, 1, 8);
    std::deque<int> values;

    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';
    for (int operation = 0; operation < operationCount; ++operation) {
        dequeOperation(random, capacity, values, input, output);
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSinglyLinkedList(
    std::mt19937_64& random,
    int operationCount
) {
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 9)) {
            case 0: {
                const int value = randomValue(random);
                input << "push_front " << value << '\n';
                values.insert(values.begin(), value);
                break;
            }
            case 1: {
                const int value = randomValue(random);
                input << "push_back " << value << '\n';
                values.push_back(value);
                break;
            }
            case 2: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 2);
                const int value = randomValue(random);
                input << "insert " << index << ' ' << value << '\n';
                if (index < 0 || index > static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values.insert(values.begin() + index, value);
                }
                break;
            }
            case 3: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "erase " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                    values.erase(values.begin() + index);
                }
                break;
            }
            case 4: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 5:
                input << "front\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.front());
                }
                break;
            case 6:
                input << "back\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            case 7:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 8:
                input << "empty\n";
                appendLine(output, values.empty() ? "true" : "false");
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateLinkedStack(
    std::mt19937_64& random,
    int operationCount
) {
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int value = randomValue(random);
                input << "push " << value << '\n';
                values.push_back(value);
                break;
            }
            case 1:
                input << "pop\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                    values.pop_back();
                }
                break;
            case 2:
                input << "top\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            case 3:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 4:
                input << "empty\n";
                appendLine(output, values.empty() ? "true" : "false");
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

void linkedQueueOperation(
    std::mt19937_64& random,
    std::deque<int>& values,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 6)) {
        case 0: {
            const int value = randomValue(random);
            input << "enqueue " << value << '\n';
            values.push_back(value);
            break;
        }
        case 1:
            input << "dequeue\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
                values.pop_front();
            }
            break;
        case 2:
            input << "front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
            }
            break;
        case 3:
            input << "back\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.back());
            }
            break;
        case 4:
            input << "size\n";
            appendLine(output, static_cast<int>(values.size()));
            break;
        case 5:
            input << "empty\n";
            appendLine(output, values.empty() ? "true" : "false");
            break;
        default:
            input << "clear\n";
            values.clear();
            break;
    }
}

GeneratedCase generateLinkedQueue(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F08-linked-queue requires operation_limit of at least 3"
        );
    }

    std::deque<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    const int value = randomValue(random);
    input << "enqueue " << value << '\n';
    values.push_back(value);
    input << "dequeue\n";
    appendLine(output, values.front());
    values.pop_front();
    input << "back\n";
    appendLine(output, "EMPTY");

    for (int operation = 3; operation < operationCount; ++operation) {
        linkedQueueOperation(random, values, input, output);
    }
    return {input.str(), output.str()};
}

GeneratedCase generateDoublyLinkedList(
    std::mt19937_64& random,
    int operationCount
) {
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 11)) {
            case 0: {
                const int value = randomValue(random);
                input << "push_front " << value << '\n';
                values.insert(values.begin(), value);
                break;
            }
            case 1: {
                const int value = randomValue(random);
                input << "push_back " << value << '\n';
                values.push_back(value);
                break;
            }
            case 2:
                input << "pop_front\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.front());
                    values.erase(values.begin());
                }
                break;
            case 3:
                input << "pop_back\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                    values.pop_back();
                }
                break;
            case 4: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 2);
                const int value = randomValue(random);
                input << "insert " << index << ' ' << value << '\n';
                if (index < 0 || index > static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values.insert(values.begin() + index, value);
                }
                break;
            }
            case 5: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "erase " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                    values.erase(values.begin() + index);
                }
                break;
            }
            case 6: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 7:
                input << "front\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.front());
                }
                break;
            case 8:
                input << "back\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            case 9:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 10:
                input << "empty\n";
                appendLine(output, values.empty() ? "true" : "false");
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

void circularListOperation(
    std::mt19937_64& random,
    std::deque<int>& values,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 7)) {
        case 0: {
            const int value = randomValue(random);
            input << "push_front " << value << '\n';
            values.push_front(value);
            break;
        }
        case 1: {
            const int value = randomValue(random);
            input << "push_back " << value << '\n';
            values.push_back(value);
            break;
        }
        case 2:
            input << "pop_front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
                values.pop_front();
            }
            break;
        case 3:
            input << "front\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.front());
            }
            break;
        case 4:
            input << "back\n";
            if (values.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, values.back());
            }
            break;
        case 5:
            input << "rotate\n";
            if (values.size() > 1) {
                values.push_back(values.front());
                values.pop_front();
            }
            break;
        case 6:
            input << "size\n";
            appendLine(output, static_cast<int>(values.size()));
            break;
        default:
            input << "empty\n";
            appendLine(output, values.empty() ? "true" : "false");
            break;
    }
}

GeneratedCase generateCircularLinkedList(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "F10-circular-linked-list requires operation_limit of at least 5"
        );
    }

    std::deque<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    const int first = randomValue(random);
    const int second = randomValue(random);
    input << "push_back " << first << '\n';
    input << "push_back " << second << '\n';
    values.push_back(first);
    values.push_back(second);
    input << "rotate\n";
    values.push_back(values.front());
    values.pop_front();
    input << "front\n";
    appendLine(output, values.front());
    input << "back\n";
    appendLine(output, values.back());

    for (int operation = 5; operation < operationCount; ++operation) {
        circularListOperation(random, values, input, output);
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSinglyListReverse(
    std::mt19937_64& random,
    int operationCount
) {
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    for (int operation = 0; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0: {
                const int value = randomValue(random);
                input << "push_front " << value << '\n';
                values.insert(values.begin(), value);
                break;
            }
            case 1: {
                const int value = randomValue(random);
                input << "push_back " << value << '\n';
                values.push_back(value);
                break;
            }
            case 2:
                input << "reverse\n";
                std::reverse(values.begin(), values.end());
                break;
            case 3: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 4:
                input << "front\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.front());
                }
                break;
            case 5:
                input << "back\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                }
                break;
            case 6:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "empty\n";
                    appendLine(output, values.empty() ? "true" : "false");
                } else {
                    input << "clear\n";
                    values.clear();
                }
                break;
        }
    }
    return {input.str(), output.str()};
}

void concatenateOperation(
    std::mt19937_64& random,
    std::deque<int>& a,
    std::deque<int>& b,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 15)) {
        case 0: {
            const int value = randomValue(random);
            input << "push_a " << value << '\n';
            a.push_back(value);
            break;
        }
        case 1: {
            const int value = randomValue(random);
            input << "push_b " << value << '\n';
            b.push_back(value);
            break;
        }
        case 2:
            input << "concat_ab\n";
            a.insert(a.end(), b.begin(), b.end());
            b.clear();
            break;
        case 3:
            input << "concat_ba\n";
            b.insert(b.end(), a.begin(), a.end());
            a.clear();
            break;
        case 4:
            input << "pop_a\n";
            if (a.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, a.front());
                a.pop_front();
            }
            break;
        case 5:
            input << "pop_b\n";
            if (b.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, b.front());
                b.pop_front();
            }
            break;
        case 6:
            input << "front_a\n";
            if (a.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, a.front());
            }
            break;
        case 7:
            input << "back_a\n";
            if (a.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, a.back());
            }
            break;
        case 8:
            input << "front_b\n";
            if (b.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, b.front());
            }
            break;
        case 9:
            input << "back_b\n";
            if (b.empty()) {
                appendLine(output, "EMPTY");
            } else {
                appendLine(output, b.back());
            }
            break;
        case 10:
            input << "size_a\n";
            appendLine(output, static_cast<int>(a.size()));
            break;
        case 11:
            input << "size_b\n";
            appendLine(output, static_cast<int>(b.size()));
            break;
        case 12:
            input << "empty_a\n";
            appendLine(output, a.empty() ? "true" : "false");
            break;
        case 13:
            input << "empty_b\n";
            appendLine(output, b.empty() ? "true" : "false");
            break;
        case 14:
            input << "clear_a\n";
            a.clear();
            break;
        default:
            input << "clear_b\n";
            b.clear();
            break;
    }
}

GeneratedCase generateSinglyListConcatenate(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F12-singly-list-concatenate requires operation_limit of at least 8"
        );
    }
    std::deque<int> a;
    std::deque<int> b;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    const int first = randomValue(random);
    const int second = randomValue(random);
    input << "push_a " << first << '\n';
    a.push_back(first);
    input << "push_b " << second << '\n';
    b.push_back(second);
    input << "concat_ab\n";
    a.insert(a.end(), b.begin(), b.end());
    b.clear();
    const int third = randomValue(random);
    input << "push_b " << third << '\n';
    b.push_back(third);
    input << "back_a\n";
    appendLine(output, a.back());
    input << "empty_b\n";
    appendLine(output, "false");
    input << "concat_ba\n";
    b.insert(b.end(), a.begin(), a.end());
    a.clear();
    input << "empty_a\n";
    appendLine(output, "true");

    for (int operation = 8; operation < operationCount; ++operation) {
        concatenateOperation(random, a, b, input, output);
    }
    return {input.str(), output.str()};
}

void splitOperation(
    std::mt19937_64& random,
    std::vector<int>& a,
    std::vector<int>& b,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 11)) {
        case 0: {
            const int value = randomValue(random);
            input << "push_a " << value << '\n';
            a.push_back(value);
            break;
        }
        case 1: {
            const int value = randomValue(random);
            input << "push_b " << value << '\n';
            b.push_back(value);
            break;
        }
        case 2:
        case 3: {
            const bool fromA = randomInt(random, 0, 1) == 0;
            std::vector<int>& source = fromA ? a : b;
            std::vector<int>& target = fromA ? b : a;
            const int index =
                randomInt(random, -2, static_cast<int>(source.size()) + 2);
            input << (fromA ? "split_a " : "split_b ") << index << '\n';
            if (index < 0 || index > static_cast<int>(source.size())) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (!target.empty()) {
                appendLine(output, "TARGET_NOT_EMPTY");
            } else {
                target.assign(source.begin() + index, source.end());
                source.erase(source.begin() + index, source.end());
            }
            break;
        }
        case 4:
            input << "front_a\n";
            if (a.empty()) appendLine(output, "EMPTY");
            else appendLine(output, a.front());
            break;
        case 5:
            input << "back_a\n";
            if (a.empty()) appendLine(output, "EMPTY");
            else appendLine(output, a.back());
            break;
        case 6:
            input << "front_b\n";
            if (b.empty()) appendLine(output, "EMPTY");
            else appendLine(output, b.front());
            break;
        case 7:
            input << "back_b\n";
            if (b.empty()) appendLine(output, "EMPTY");
            else appendLine(output, b.back());
            break;
        case 8:
            input << "size_a\n";
            appendLine(output, static_cast<int>(a.size()));
            break;
        case 9:
            input << "size_b\n";
            appendLine(output, static_cast<int>(b.size()));
            break;
        case 10:
            if (randomInt(random, 0, 1) == 0) {
                input << "empty_a\n";
                appendLine(output, a.empty() ? "true" : "false");
            } else {
                input << "clear_a\n";
                a.clear();
            }
            break;
        default:
            if (randomInt(random, 0, 1) == 0) {
                input << "empty_b\n";
                appendLine(output, b.empty() ? "true" : "false");
            } else {
                input << "clear_b\n";
                b.clear();
            }
            break;
    }
}

GeneratedCase generateSinglyListSplit(
    std::mt19937_64& random,
    int operationCount
) {
    std::vector<int> a;
    std::vector<int> b;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    for (int operation = 0; operation < operationCount; ++operation) {
        splitOperation(random, a, b, input, output);
    }
    return {input.str(), output.str()};
}

void spliceOperation(
    std::mt19937_64& random,
    std::vector<int>& a,
    std::vector<int>& b,
    std::ostringstream& input,
    std::ostringstream& output
) {
    switch (randomInt(random, 0, 13)) {
        case 0:
        case 1:
        case 2:
        case 3: {
            const bool listA = randomInt(random, 0, 1) == 0;
            const bool front = randomInt(random, 0, 1) == 0;
            const int value = randomValue(random);
            input << (front ? "push_front_" : "push_back_")
                  << (listA ? 'a' : 'b') << ' ' << value << '\n';
            std::vector<int>& list = listA ? a : b;
            if (front) list.insert(list.begin(), value);
            else list.push_back(value);
            break;
        }
        case 4:
        case 5: {
            const bool toA = randomInt(random, 0, 1) == 0;
            std::vector<int>& destination = toA ? a : b;
            std::vector<int>& source = toA ? b : a;
            const int index =
                randomInt(random, -2, static_cast<int>(destination.size()) + 2);
            input << (toA ? "splice_b_to_a " : "splice_a_to_b ")
                  << index << '\n';
            if (index < 0 || index > static_cast<int>(destination.size())) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                destination.insert(
                    destination.begin() + index,
                    source.begin(),
                    source.end()
                );
                source.clear();
            }
            break;
        }
        case 6:
        case 7:
        case 8:
        case 9: {
            const bool listA = randomInt(random, 0, 1) == 0;
            const bool front = randomInt(random, 0, 1) == 0;
            input << (front ? "pop_front_" : "pop_back_")
                  << (listA ? 'a' : 'b') << '\n';
            std::vector<int>& list = listA ? a : b;
            if (list.empty()) {
                appendLine(output, "EMPTY");
            } else if (front) {
                appendLine(output, list.front());
                list.erase(list.begin());
            } else {
                appendLine(output, list.back());
                list.pop_back();
            }
            break;
        }
        case 10:
        case 11: {
            const bool listA = randomInt(random, 0, 1) == 0;
            std::vector<int>& list = listA ? a : b;
            const int index =
                randomInt(random, -2, static_cast<int>(list.size()) + 1);
            input << (listA ? "get_a " : "get_b ") << index << '\n';
            if (index < 0 || index >= static_cast<int>(list.size())) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(output, list[static_cast<std::size_t>(index)]);
            }
            break;
        }
        case 12:
            if (randomInt(random, 0, 1) == 0) {
                input << "size_a\n";
                appendLine(output, static_cast<int>(a.size()));
            } else {
                input << "empty_a\n";
                appendLine(output, a.empty() ? "true" : "false");
            }
            break;
        default:
            if (randomInt(random, 0, 1) == 0) {
                input << "size_b\n";
                appendLine(output, static_cast<int>(b.size()));
            } else {
                input << "empty_b\n";
                appendLine(output, b.empty() ? "true" : "false");
            }
            break;
    }
}

GeneratedCase generateDoublyListSplice(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 7) {
        throw std::runtime_error(
            "F14-doubly-list-splice requires operation_limit of at least 7"
        );
    }
    std::vector<int> a;
    std::vector<int> b;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    const int first = randomValue(random);
    const int second = randomValue(random);
    const int third = randomValue(random);
    const int fourth = randomValue(random);
    input << "push_back_a " << first << '\n';
    input << "push_back_a " << fourth << '\n';
    input << "push_back_b " << second << '\n';
    input << "push_back_b " << third << '\n';
    a = {first, fourth};
    b = {second, third};
    input << "splice_b_to_a 1\n";
    a.insert(a.begin() + 1, b.begin(), b.end());
    b.clear();
    input << "pop_back_a\n";
    appendLine(output, a.back());
    a.pop_back();
    input << "pop_back_a\n";
    appendLine(output, a.back());
    a.pop_back();

    for (int operation = 7; operation < operationCount; ++operation) {
        spliceOperation(random, a, b, input, output);
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFixedNodePoolList(
    std::mt19937_64& random,
    int operationCount
) {
    const int capacity = randomInt(random, 1, 8);
    if (operationCount < capacity + 4) {
        throw std::runtime_error(
            "F15-fixed-node-pool-list operation_limit is too small"
        );
    }
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int index = 0; index < capacity; ++index) {
        const int value = randomValue(random);
        input << "insert " << index << ' ' << value << '\n';
        values.push_back(value);
    }
    input << "erase 0\n";
    appendLine(output, values.front());
    values.erase(values.begin());
    const int replacement = randomValue(random);
    input << "insert " << values.size() << ' ' << replacement << '\n';
    values.push_back(replacement);
    input << "free\n";
    appendLine(output, 0);
    input << "back\n";
    appendLine(output, values.back());

    for (int operation = capacity + 4;
         operation < operationCount;
         ++operation) {
        switch (randomInt(random, 0, 8)) {
            case 0:
            case 1: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 2);
                const int value = randomValue(random);
                input << "insert " << index << ' ' << value << '\n';
                if (index < 0 || index > static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else if (static_cast<int>(values.size()) == capacity) {
                    appendLine(output, "FULL");
                } else {
                    values.insert(values.begin() + index, value);
                }
                break;
            }
            case 2: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "erase " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                    values.erase(values.begin() + index);
                }
                break;
            }
            case 3: {
                const int index =
                    randomInt(random, -2, static_cast<int>(values.size()) + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, values[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 4:
                input << "front\n";
                if (values.empty()) appendLine(output, "EMPTY");
                else appendLine(output, values.front());
                break;
            case 5:
                input << "back\n";
                if (values.empty()) appendLine(output, "EMPTY");
                else appendLine(output, values.back());
                break;
            case 6:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 7:
                input << "free\n";
                appendLine(output, capacity - static_cast<int>(values.size()));
                break;
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "empty\n";
                    appendLine(output, values.empty() ? "true" : "false");
                } else {
                    input << "clear\n";
                    values.clear();
                }
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateCase(
    const std::string& problemId,
    std::uint64_t seed,
    int operationCount
) {
    std::mt19937_64 random(seed);
    if (problemId == "F01-array-adt") {
        return generateArrayAdt(random, operationCount);
    }
    if (problemId == "F02-dynamic-array") {
        return generateDynamicArray(random, operationCount);
    }
    if (problemId == "F03-stack-array") {
        return generateStack(random, operationCount);
    }
    if (problemId == "F04-circular-queue") {
        return generateCircularQueue(random, operationCount);
    }
    if (problemId == "F05-array-deque") {
        return generateArrayDeque(random, operationCount);
    }
    if (problemId == "F06-singly-linked-list") {
        return generateSinglyLinkedList(random, operationCount);
    }
    if (problemId == "F07-linked-stack") {
        return generateLinkedStack(random, operationCount);
    }
    if (problemId == "F08-linked-queue") {
        return generateLinkedQueue(random, operationCount);
    }
    if (problemId == "F09-doubly-linked-list") {
        return generateDoublyLinkedList(random, operationCount);
    }
    if (problemId == "F10-circular-linked-list") {
        return generateCircularLinkedList(random, operationCount);
    }
    if (problemId == "F11-singly-list-reverse") {
        return generateSinglyListReverse(random, operationCount);
    }
    if (problemId == "F12-singly-list-concatenate") {
        return generateSinglyListConcatenate(random, operationCount);
    }
    if (problemId == "F13-singly-list-split") {
        return generateSinglyListSplit(random, operationCount);
    }
    if (problemId == "F14-doubly-list-splice") {
        return generateDoublyListSplice(random, operationCount);
    }
    if (problemId == "F15-fixed-node-pool-list") {
        return generateFixedNodePoolList(random, operationCount);
    }
    throw std::runtime_error(
        "stress generator is not available for problem: " + problemId
    );
}

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
