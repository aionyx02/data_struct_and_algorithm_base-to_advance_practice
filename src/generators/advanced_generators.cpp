#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace judge::generators {
namespace {
GeneratedCase generateAvlSingleRotationTrace(
    std::mt19937_64& random,
    int caseCount
) {
    std::ostringstream input;
    std::ostringstream output;
    input << caseCount << '\n';
    for (int index = 0; index < caseCount; ++index) {
        int low = randomInt(random, -1000, 998);
        int middle = randomInt(random, low + 1, 999);
        int high = randomInt(random, middle + 1, 1000);
        const bool leftLeft = randomInt(random, 0, 1) == 0;
        if (leftLeft) {
            input << "LL " << high << ' ' << middle << ' ' << low << '\n';
        } else {
            input << "RR " << low << ' ' << middle << ' ' << high << '\n';
        }
        output << middle << " | " << middle << ' ' << low << ' ' << high
               << " | 2 1 1\n";
    }
    return {input.str(), output.str()};
}

GeneratedCase generateAvlDoubleRotationTrace(
    std::mt19937_64& random,
    int caseCount
) {
    std::ostringstream input;
    std::ostringstream output;
    input << caseCount << '\n';
    for (int index = 0; index < caseCount; ++index) {
        int low = randomInt(random, -1000, 998);
        int middle = randomInt(random, low + 1, 999);
        int high = randomInt(random, middle + 1, 1000);
        const bool leftRight = randomInt(random, 0, 1) == 0;
        if (leftRight) {
            input << "LR " << high << ' ' << low << ' ' << middle << '\n';
            output << "LEFT_RIGHT";
        } else {
            input << "RL " << low << ' ' << high << ' ' << middle << '\n';
            output << "RIGHT_LEFT";
        }
        output << " | " << middle << " | " << middle << ' ' << low << ' '
               << high << " | 2 1 1\n";
    }
    return {input.str(), output.str()};
}

GeneratedCase generateAvlInsertionTree(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "F61-avl-insertion-tree operation_limit is too small"
        );
    }
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount
          << "\ninsert 30\ninsert 10\ninsert 20\ninsert 40\ninsert 50\n";
    const int keys[5] = {10, 20, 30, 40, 50};
    const int parents[5] = {20, -1, 40, 20, 40};
    const int heights[5] = {1, 3, 1, 2, 1};
    const int balances[5] = {0, -1, 0, 0, 0};
    for (int operation = 5; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 8);
        if (kind == 0) {
            input << "root\n";
            appendLine(output, 20);
        } else if (kind == 1) {
            input << "preorder\n";
            output << "20 10 40 30 50\n";
        } else if (kind == 2) {
            input << "inorder\n";
            output << "10 20 30 40 50\n";
        } else if (kind == 3) {
            input << "size\n";
            appendLine(output, 5);
        } else if (kind == 4) {
            const int key = randomInt(random, 5, 55);
            input << "contains " << key << '\n';
            bool found = false;
            for (int value : keys) found = found || value == key;
            appendLine(output, found ? "true" : "false");
        } else {
            const int index = randomInt(random, 0, 4);
            const int query = randomInt(random, 0, 2);
            const char* command =
                query == 0 ? "parent" : (query == 1 ? "height" : "balance");
            input << command << ' ' << keys[index] << '\n';
            if (query == 0) {
                if (parents[index] < 0) appendLine(output, "NONE");
                else appendLine(output, parents[index]);
            } else if (query == 1) {
                appendLine(output, heights[index]);
            } else {
                appendLine(output, balances[index]);
            }
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateAvlDeletionTree(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 10) {
        throw std::runtime_error(
            "F62-avl-deletion-tree operation_limit is too small"
        );
    }
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount
          << "\ninsert 9\ninsert 5\ninsert 10\ninsert 0\ninsert 6\n"
          << "insert 11\ninsert -1\ninsert 1\ninsert 2\nerase 10\n";
    appendLine(output, "REMOVED");
    const int keys[8] = {-1, 0, 1, 2, 5, 6, 9, 11};
    const int heights[8] = {1, 2, 4, 1, 2, 1, 3, 1};
    const int balances[8] = {0, 1, -1, 0, 0, 0, 1, 0};
    for (int operation = 10; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 7);
        if (kind == 0) {
            input << "root\n";
            appendLine(output, 1);
        } else if (kind == 1) {
            input << "preorder\n";
            output << "1 0 -1 9 5 2 6 11\n";
        } else if (kind == 2) {
            input << "inorder\n";
            output << "-1 0 1 2 5 6 9 11\n";
        } else if (kind == 3) {
            input << "size\n";
            appendLine(output, 8);
        } else if (kind == 4) {
            const int key = randomInt(random, -2, 12);
            input << "contains " << key << '\n';
            bool found = false;
            for (int value : keys) found = found || value == key;
            appendLine(output, found ? "true" : "false");
        } else {
            const int index = randomInt(random, 0, 7);
            const bool heightQuery = randomInt(random, 0, 1) == 0;
            input << (heightQuery ? "height " : "balance ")
                  << keys[index] << '\n';
            appendLine(
                output,
                heightQuery ? heights[index] : balances[index]
            );
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateAvlStructuralValidation(
    std::mt19937_64& random,
    int operationCount
) {
    static_cast<void>(operationCount);
    const int base = randomInt(random, -500, 500);
    std::ostringstream input;
    input << "4 0\n"
          << "0 " << base << " 1 2 -1 3\n"
          << "1 " << base - 20 << " -1 3 0 2\n"
          << "2 " << base + 20 << " -1 -1 0 1\n"
          << "3 " << base + 10 << " -1 -1 1 1\n";
    return {input.str(), "INVALID_ORDER\n"};
}

GeneratedCase generateOrderFourSearchTreeValidation(
    std::mt19937_64& random,
    int operationCount
) {
    static_cast<void>(operationCount);
    const int separator = randomInt(random, -100, 100);
    std::ostringstream input;
    input << "2 0\n"
          << "0 2 " << separator << ' ' << separator + 10
          << " -1 1 -1 -1 -1\n"
          << "1 1 " << separator - 5
          << " -1 -1 -1 -1 -1 -1\n";
    return {input.str(), "INVALID_OCCUPANCY\n"};
}

GeneratedCase generateRedBlackInsertionTree(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "F65-red-black-insertion-tree operation_limit is too small"
        );
    }
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount
          << "\ninsert 41\ninsert 38\ninsert 31\ninsert 12\n"
          << "insert 19\ninsert 8\n";
    const int keys[6] = {8, 12, 19, 31, 38, 41};
    const int parents[6] = {12, 19, 38, 19, -1, 38};
    const bool red[6] = {true, false, true, false, false, false};
    for (int operation = 6; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 8);
        if (kind == 0) {
            input << "root\n";
            appendLine(output, 38);
        } else if (kind == 1) {
            input << "preorder\n";
            output << "38:B 19:R 12:B 8:R 31:B 41:B\n";
        } else if (kind == 2) {
            input << "inorder\n";
            output << "8 12 19 31 38 41\n";
        } else if (kind == 3) {
            input << "black_height\n";
            appendLine(output, 3);
        } else if (kind == 4) {
            input << "validate\n";
            appendLine(output, "VALID");
        } else if (kind == 5) {
            input << "size\n";
            appendLine(output, 6);
        } else if (kind == 6) {
            const int key = randomInt(random, 5, 45);
            input << "contains " << key << '\n';
            bool found = false;
            for (int value : keys) found = found || value == key;
            appendLine(output, found ? "true" : "false");
        } else {
            const int index = randomInt(random, 0, 5);
            const bool colorQuery = kind == 7;
            input << (colorQuery ? "color " : "parent ")
                  << keys[index] << '\n';
            if (colorQuery) {
                appendLine(output, red[index] ? "RED" : "BLACK");
            } else if (parents[index] < 0) {
                appendLine(output, "NONE");
            } else {
                appendLine(output, parents[index]);
            }
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSequenceShiftCostTrace(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F66-sequence-shift-cost-trace operation_limit is too small"
        );
    }
    const int capacity = randomInt(random, 3, 8);
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int index = 0; index < 3; ++index) {
        const int value = randomValue(random);
        const int insertionIndex = index == 0 ? 0 : index - 1;
        input << "insert " << insertionIndex << ' ' << value << '\n';
        appendLine(
            output,
            static_cast<int>(values.size()) - insertionIndex
        );
        values.insert(values.begin() + insertionIndex, value);
    }

    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0: {
                const int index = randomInt(
                    random,
                    -1,
                    static_cast<int>(values.size()) + 1
                );
                const int value = randomValue(random);
                input << "insert " << index << ' ' << value << '\n';
                if (index < 0 ||
                    index > static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else if (static_cast<int>(values.size()) == capacity) {
                    appendLine(output, "FULL");
                } else {
                    appendLine(
                        output,
                        static_cast<int>(values.size()) - index
                    );
                    values.insert(values.begin() + index, value);
                }
                break;
            }
            case 1: {
                const int index = randomInt(
                    random,
                    -1,
                    static_cast<int>(values.size())
                );
                input << "erase " << index << '\n';
                if (index < 0 ||
                    index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    const int removed =
                        values[static_cast<std::size_t>(index)];
                    const int shifts =
                        static_cast<int>(values.size()) - index - 1;
                    output << removed << ' ' << shifts << '\n';
                    values.erase(values.begin() + index);
                }
                break;
            }
            case 2: {
                const int index = randomInt(
                    random,
                    -1,
                    static_cast<int>(values.size())
                );
                input << "get " << index << '\n';
                if (index < 0 ||
                    index >= static_cast<int>(values.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(
                        output,
                        values[static_cast<std::size_t>(index)]
                    );
                }
                break;
            }
            case 3:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 4:
                input << "capacity\n";
                appendLine(output, capacity);
                break;
            case 5:
            case 6:
                input << "values\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    for (std::size_t index = 0; index < values.size(); ++index) {
                        if (index > 0) output << ' ';
                        output << values[index];
                    }
                    output << '\n';
                }
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSwitchableStackRepresentation(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "F67-switchable-stack-representation operation_limit is too small"
        );
    }
    const int capacity = randomInt(random, 2, 8);
    std::vector<int> values;
    bool linked = false;
    const int first = randomValue(random);
    const int second = randomValue(random);
    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount
          << "\npush " << first
          << "\npush " << second
          << "\nswitch linked\ntop\nswitch array\nvalues\n";
    values.push_back(first);
    values.push_back(second);
    appendLine(output, 2);
    appendLine(output, second);
    appendLine(output, 2);
    output << first << ' ' << second << '\n';

    for (int operation = 6; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 8);
        switch (kind) {
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
                if (values.empty()) appendLine(output, "EMPTY");
                else appendLine(output, values.back());
                break;
            case 3:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            case 4:
                input << "representation\n";
                appendLine(output, linked ? "linked" : "array");
                break;
            case 5:
            case 6: {
                const bool targetLinked =
                    kind == 5 ? !linked : linked;
                input << "switch "
                      << (targetLinked ? "linked" : "array") << '\n';
                if (targetLinked == linked) {
                    appendLine(output, "UNCHANGED");
                } else {
                    appendLine(output, static_cast<int>(values.size()));
                    linked = targetLinked;
                }
                break;
            }
            case 7:
                input << "values\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    for (std::size_t index = 0; index < values.size(); ++index) {
                        if (index > 0) output << ' ';
                        output << values[index];
                    }
                    output << '\n';
                }
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateAmortizedGrowthLedger(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "F68-amortized-growth-ledger operation_limit is too small"
        );
    }
    int capacity = 2;
    long long totalCopies = 0;
    std::vector<int> values;
    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    for (int index = 0; index < 3; ++index) {
        const int value = randomValue(random);
        input << "push " << value << '\n';
        int copies = 0;
        if (static_cast<int>(values.size()) == capacity) {
            copies = static_cast<int>(values.size());
            capacity *= 2;
            totalCopies += copies;
        }
        values.push_back(value);
        appendLine(output, copies);
    }
    input << "total_copies\nreserve 7\ncapacity\n";
    appendLine(output, static_cast<int>(totalCopies));
    totalCopies += static_cast<int>(values.size());
    capacity = 7;
    appendLine(output, static_cast<int>(values.size()));
    appendLine(output, capacity);

    for (int operation = 6; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 8)) {
            case 0:
            case 1: {
                const int value = randomValue(random);
                input << "push " << value << '\n';
                int copies = 0;
                if (static_cast<int>(values.size()) == capacity) {
                    copies = static_cast<int>(values.size());
                    capacity *= 2;
                    totalCopies += copies;
                }
                values.push_back(value);
                appendLine(output, copies);
                break;
            }
            case 2:
                input << "pop\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(output, values.back());
                    values.pop_back();
                }
                break;
            case 3: {
                const int requested = randomInt(
                    random,
                    1,
                    capacity + 8
                );
                input << "reserve " << requested << '\n';
                if (requested <= capacity) {
                    appendLine(output, "UNCHANGED");
                } else {
                    totalCopies += static_cast<int>(values.size());
                    capacity = requested;
                    appendLine(output, static_cast<int>(values.size()));
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
            case 6:
                input << "total_copies\n";
                output << totalCopies << '\n';
                break;
            case 7:
                input << "values\n";
                if (values.empty()) {
                    appendLine(output, "EMPTY");
                } else {
                    for (std::size_t index = 0; index < values.size(); ++index) {
                        if (index > 0) output << ' ';
                        output << values[index];
                    }
                    output << '\n';
                }
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFenwickPointPrefix(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "A01-fenwick-point-prefix requires operation_limit of at least 3"
        );
    }
    const int size = randomInt(random, 1, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

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

    auto prefix = [&](int last) {
        long long sum = 0;
        for (int index = 0; index <= last; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };

    const int firstIndex = randomInt(random, 0, size - 1);
    const int firstDelta = randomValue(random);
    input << "add " << firstIndex << ' ' << firstDelta << '\n';
    values[static_cast<std::size_t>(firstIndex)] += firstDelta;
    const int secondIndex = randomInt(random, 0, size - 1);
    const int secondDelta = randomValue(random);
    input << "add " << secondIndex << ' ' << secondDelta << '\n';
    values[static_cast<std::size_t>(secondIndex)] += secondDelta;
    input << "range 0 " << (size - 1) << '\n';
    output << prefix(size - 1) << '\n';

    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int index = randomInt(random, 0, size - 1);
                const int value = randomValue(random);
                input << "add " << index << ' ' << value << '\n';
                values[static_cast<std::size_t>(index)] += value;
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
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 3: {
                const int index = randomInt(random, -2, size + 1);
                input << "prefix " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << prefix(index) << '\n';
                }
                break;
            }
            case 4: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range " << left << ' ' << right << '\n';
                if (left < 0 || left >= size || right < 0 || right >= size ||
                    left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << (prefix(right) - prefix(left - 1)) << '\n';
                }
                break;
            }
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFenwickRangePoint(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A02-fenwick-range-update-point-query requires operation_limit "
            "of at least 4"
        );
    }
    const int size = randomInt(random, 1, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

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

    const int prologueLeft = size == 1 ? 0 : randomInt(random, 0, size - 2);
    const int prologueDelta = randomValue(random);
    input << "range_add " << prologueLeft << ' ' << (size - 1) << ' '
          << prologueDelta << '\n';
    for (int index = prologueLeft; index < size; ++index) {
        values[static_cast<std::size_t>(index)] += prologueDelta;
    }
    input << "get 0\n";
    output << values.front() << '\n';
    input << "get " << (size - 1) << '\n';
    output << values.back() << '\n';
    input << "get " << size << '\n';
    appendLine(output, "OUT_OF_RANGE");

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 3)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "range_add " << left << ' ' << right << ' ' << value
                      << '\n';
                if (left < 0 || left >= size || right < 0 || right >= size ||
                    left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    for (int index = left; index <= right; ++index) {
                        values[static_cast<std::size_t>(index)] += value;
                    }
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "add " << index << ' ' << value << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] += value;
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFenwickDualRangeRange(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A03-fenwick-dual-range-range requires operation_limit of at least 4"
        );
    }
    const int size = randomInt(random, 1, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

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

    auto prefix = [&](int last) {
        long long sum = 0;
        for (int index = 0; index <= last; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };

    const int mid = size / 2;
    const int firstDelta = randomValue(random);
    input << "range_add 0 " << mid << ' ' << firstDelta << '\n';
    for (int index = 0; index <= mid; ++index) {
        values[static_cast<std::size_t>(index)] += firstDelta;
    }
    const int secondDelta = randomValue(random);
    input << "range_add " << mid << ' ' << (size - 1) << ' ' << secondDelta
          << '\n';
    for (int index = mid; index <= size - 1; ++index) {
        values[static_cast<std::size_t>(index)] += secondDelta;
    }
    input << "range_sum 0 " << (size - 1) << '\n';
    output << prefix(size - 1) << '\n';
    input << "prefix " << mid << '\n';
    output << prefix(mid) << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 3)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "range_add " << left << ' ' << right << ' ' << value
                      << '\n';
                if (left < 0 || left >= size || right < 0 || right >= size ||
                    left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    for (int index = left; index <= right; ++index) {
                        values[static_cast<std::size_t>(index)] += value;
                    }
                }
                break;
            }
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range_sum " << left << ' ' << right << '\n';
                if (left < 0 || left >= size || right < 0 || right >= size ||
                    left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << (prefix(right) - prefix(left - 1)) << '\n';
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "prefix " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << prefix(index) << '\n';
                }
                break;
            }
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFenwickPrefixLowerBound(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "A04-fenwick-prefix-lower-bound requires operation_limit of at least 3"
        );
    }
    const int size = randomInt(random, 1, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomInt(random, 0, 30);
        if (index != 0) {
            input << ' ';
        }
        input << values[static_cast<std::size_t>(index)];
    }
    input << '\n';

    auto prefix = [&](int last) {
        long long sum = 0;
        for (int index = 0; index <= last; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };
    auto lowerBound = [&](long long target) {
        long long accumulated = 0;
        for (int index = 0; index < size; ++index) {
            accumulated += values[static_cast<std::size_t>(index)];
            if (accumulated >= target) {
                return index;
            }
        }
        return -1;
    };

    const int bumpIndex = randomInt(random, 0, size - 1);
    const int bump = randomInt(random, 1, 20);
    input << "add " << bumpIndex << ' ' << bump << '\n';
    values[static_cast<std::size_t>(bumpIndex)] += bump;
    const long long boundaryTarget = prefix(bumpIndex);
    input << "lower_bound " << boundaryTarget << '\n';
    {
        const int answer = lowerBound(boundaryTarget);
        if (answer < 0) {
            appendLine(output, "NONE");
        } else {
            output << answer << '\n';
        }
    }

    for (int operation = 2; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 4)) {
            case 0: {
                const int index = randomInt(random, 0, size - 1);
                const int value = randomInt(random, 0, 20);
                input << "add " << index << ' ' << value << '\n';
                values[static_cast<std::size_t>(index)] += value;
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "prefix " << index << '\n';
                if (index < 0 || index >= size) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << prefix(index) << '\n';
                }
                break;
            }
            case 3: {
                const long long target = randomInt(
                    random, -2, static_cast<int>(prefix(size - 1)) + 3
                );
                input << "lower_bound " << target << '\n';
                const int answer = lowerBound(target);
                if (answer < 0) {
                    appendLine(output, "NONE");
                } else {
                    output << answer << '\n';
                }
                break;
            }
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateFenwickOrderStatistics(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A05-fenwick-frequency-order-statistics requires operation_limit "
            "of at least 4"
        );
    }
    const int universe = randomInt(random, 1, 20);
    std::vector<long long> freq(static_cast<std::size_t>(universe));
    long long total = 0;

    std::ostringstream input;
    std::ostringstream output;
    input << universe << ' ' << operationCount << '\n';

    auto kth = [&](long long order) {
        if (order < 1 || order > total) {
            return -1;
        }
        long long accumulated = 0;
        for (int key = 0; key < universe; ++key) {
            accumulated += freq[static_cast<std::size_t>(key)];
            if (accumulated >= order) {
                return key;
            }
        }
        return -1;
    };
    auto less = [&](int bound) {
        long long sum = 0;
        for (int key = 0; key < bound; ++key) {
            sum += freq[static_cast<std::size_t>(key)];
        }
        return sum;
    };

    const int seedKeyA = randomInt(random, 0, universe - 1);
    input << "insert " << seedKeyA << '\n';
    freq[static_cast<std::size_t>(seedKeyA)] += 1;
    total += 1;
    const int seedKeyB = randomInt(random, 0, universe - 1);
    input << "insert " << seedKeyB << '\n';
    freq[static_cast<std::size_t>(seedKeyB)] += 1;
    total += 1;
    input << "kth 1\n";
    output << kth(1) << '\n';
    input << "less " << universe << '\n';
    output << less(universe) << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int key = randomInt(random, -2, universe + 1);
                input << "insert " << key << '\n';
                if (key < 0 || key >= universe) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    freq[static_cast<std::size_t>(key)] += 1;
                    total += 1;
                }
                break;
            }
            case 1: {
                const int key = randomInt(random, -2, universe + 1);
                input << "erase " << key << '\n';
                if (key < 0 || key >= universe) {
                    appendLine(output, "OUT_OF_RANGE");
                } else if (freq[static_cast<std::size_t>(key)] == 0) {
                    appendLine(output, "NOT_FOUND");
                } else {
                    freq[static_cast<std::size_t>(key)] -= 1;
                    total -= 1;
                }
                break;
            }
            case 2: {
                const long long order =
                    randomInt(random, -1, static_cast<int>(total) + 2);
                input << "kth " << order << '\n';
                const int key = kth(order);
                if (key < 0) {
                    appendLine(output, "NONE");
                } else {
                    output << key << '\n';
                }
                break;
            }
            case 3: {
                const int bound = randomInt(random, -2, universe + 1);
                input << "less " << bound << '\n';
                if (bound < 0 || bound > universe) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << less(bound) << '\n';
                }
                break;
            }
            case 4: {
                const int key = randomInt(random, -2, universe + 1);
                input << "count " << key << '\n';
                if (key < 0 || key >= universe) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << freq[static_cast<std::size_t>(key)] << '\n';
                }
                break;
            }
            default:
                input << "size\n";
                output << total << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerAdvancedGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F59-avl-single-rotation-trace", generateAvlSingleRotationTrace);
    registry.emplace("F60-avl-double-rotation-trace", generateAvlDoubleRotationTrace);
    registry.emplace("F61-avl-insertion-tree", generateAvlInsertionTree);
    registry.emplace("F62-avl-deletion-tree", generateAvlDeletionTree);
    registry.emplace("F63-avl-structural-validation", generateAvlStructuralValidation);
    registry.emplace("F64-order-four-search-tree-validation", generateOrderFourSearchTreeValidation);
    registry.emplace("F65-red-black-insertion-tree", generateRedBlackInsertionTree);
    registry.emplace("F66-sequence-shift-cost-trace", generateSequenceShiftCostTrace);
    registry.emplace("F67-switchable-stack-representation", generateSwitchableStackRepresentation);
    registry.emplace("F68-amortized-growth-ledger", generateAmortizedGrowthLedger);
    registry.emplace("A01-fenwick-point-prefix", generateFenwickPointPrefix);
    registry.emplace("A02-fenwick-range-update-point-query", generateFenwickRangePoint);
    registry.emplace("A03-fenwick-dual-range-range", generateFenwickDualRangeRange);
    registry.emplace("A04-fenwick-prefix-lower-bound", generateFenwickPrefixLowerBound);
    registry.emplace("A05-fenwick-frequency-order-statistics", generateFenwickOrderStatistics);
}

}  // namespace judge::generators
