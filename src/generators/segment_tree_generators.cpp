#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cstdlib>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

GeneratedCase generatePointRangeSum(
    std::mt19937_64& random,
    int operationCount,
    const std::string& problemId
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            problemId + " requires operation_limit of at least 5"
        );
    }
    const int size = randomInt(random, 2, 20);
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

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto rangeSum = [&](int left, int right) {
        long long sum = 0;
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };

    const int replacement = randomValue(random);
    input << "set 0 " << replacement << '\n';
    values.front() = replacement;
    input << "range 0 " << (size - 1) << '\n';
    output << rangeSum(0, size - 1) << '\n';
    const int lastDelta = randomValue(random);
    input << "add " << (size - 1) << ' ' << lastDelta << '\n';
    values.back() += lastDelta;
    input << "range " << (size - 1) << ' ' << (size - 1) << '\n';
    output << values.back() << '\n';
    input << "all\n";
    output << rangeSum(0, size - 1) << '\n';

    for (int operation = 5; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "add " << index << ' ' << delta << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] += delta;
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 3: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(left, right) << '\n';
                }
                break;
            }
            case 4:
                input << "all\n";
                output << rangeSum(0, size - 1) << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreeRecursiveSum(
    std::mt19937_64& random,
    int operationCount
) {
    return generatePointRangeSum(
        random, operationCount, "A09-segment-tree-recursive-sum"
    );
}

GeneratedCase generateSegmentTreeIterativeSum(
    std::mt19937_64& random,
    int operationCount
) {
    return generatePointRangeSum(
        random, operationCount, "A10-segment-tree-iterative-sum"
    );
}

GeneratedCase generateSegmentTreeMinMax(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A11-segment-tree-min-max-monoid requires operation_limit of at "
            "least 4"
        );
    }
    const int size = randomInt(random, 2, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomInt(random, 1, 100);
        if (index != 0) {
            input << ' ';
        }
        input << values[static_cast<std::size_t>(index)];
    }
    input << '\n';

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto bounds = [&](int left, int right) {
        long long minimum = values[static_cast<std::size_t>(left)];
        long long maximum = minimum;
        for (int index = left + 1; index <= right; ++index) {
            minimum = std::min(
                minimum, values[static_cast<std::size_t>(index)]
            );
            maximum = std::max(
                maximum, values[static_cast<std::size_t>(index)]
            );
        }
        return std::pair{minimum, maximum};
    };

    input << "minimum 1 " << (size - 1) << '\n';
    output << bounds(1, size - 1).first << '\n';
    const int negative = -randomInt(random, 1, 100);
    input << "set 0 " << negative << '\n';
    values.front() = negative;
    input << "bounds 0 " << (size - 1) << '\n';
    {
        const auto [minimum, maximum] = bounds(0, size - 1);
        output << minimum << ' ' << maximum << '\n';
    }
    input << "all\n";
    {
        const auto [minimum, maximum] = bounds(0, size - 1);
        output << minimum << ' ' << maximum << '\n';
    }

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 2:
            case 3:
            case 4: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const char* command = operation % 3 == 0
                    ? "minimum"
                    : (operation % 3 == 1 ? "maximum" : "bounds");
                input << command << ' ' << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    const auto [minimum, maximum] = bounds(left, right);
                    if (command == std::string("minimum")) {
                        output << minimum << '\n';
                    } else if (command == std::string("maximum")) {
                        output << maximum << '\n';
                    } else {
                        output << minimum << ' ' << maximum << '\n';
                    }
                }
                break;
            }
            case 5: {
                input << "all\n";
                const auto [minimum, maximum] = bounds(0, size - 1);
                output << minimum << ' ' << maximum << '\n';
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

GeneratedCase generateSegmentTreeGcd(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A12-segment-tree-gcd-monoid requires operation_limit of at least 4"
        );
    }
    const int size = randomInt(random, 2, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] =
            static_cast<long long>(randomInt(random, -20, 20)) * 6;
    }
    values[1] = 12;
    for (int index = 0; index < size; ++index) {
        if (index != 0) {
            input << ' ';
        }
        input << values[static_cast<std::size_t>(index)];
    }
    input << '\n';

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto rangeGcd = [&](int left, int right) {
        long long result = 0;
        for (int index = left; index <= right; ++index) {
            result = std::gcd(
                result, std::llabs(values[static_cast<std::size_t>(index)])
            );
        }
        return result;
    };

    input << "gcd 1 " << (size - 1) << '\n';
    output << rangeGcd(1, size - 1) << '\n';
    input << "set 0 0\n";
    values.front() = 0;
    input << "add " << (size - 1) << " 6\n";
    values.back() += 6;
    input << "all\n";
    output << rangeGcd(0, size - 1) << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "add " << index << ' ' << delta << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] += delta;
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 3: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "gcd " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeGcd(left, right) << '\n';
                }
                break;
            }
            case 4:
                input << "all\n";
                output << rangeGcd(0, size - 1) << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreeLazyAdd(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "A13-segment-tree-lazy-range-add requires operation_limit of at "
            "least 5"
        );
    }
    const int size = randomInt(random, 2, 20);
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

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto rangeSum = [&](int left, int right) {
        long long sum = 0;
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };
    auto addRange = [&](int left, int right, long long delta) {
        for (int index = left; index <= right; ++index) {
            values[static_cast<std::size_t>(index)] += delta;
        }
    };

    const int fullDelta = randomInt(random, 1, 20);
    input << "range_add 0 " << (size - 1) << ' ' << fullDelta << '\n';
    addRange(0, size - 1, fullDelta);
    input << "range 0 0\n";
    output << values.front() << '\n';
    input << "range 0 " << (size - 1) << '\n';
    output << rangeSum(0, size - 1) << '\n';
    const int middle = size / 2;
    const int suffixDelta = -randomInt(random, 1, 20);
    input << "range_add " << middle << ' ' << (size - 1) << ' '
          << suffixDelta << '\n';
    addRange(middle, size - 1, suffixDelta);
    input << "get " << middle << '\n';
    output << values[static_cast<std::size_t>(middle)] << '\n';

    for (int operation = 5; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "range_add " << left << ' ' << right << ' ' << delta
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    addRange(left, right, delta);
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "add " << index << ' ' << delta << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] += delta;
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 3: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 4: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(left, right) << '\n';
                }
                break;
            }
            case 5:
                input << "all\n";
                output << rangeSum(0, size - 1) << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreeLazyAssign(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "A14-segment-tree-lazy-range-assign requires operation_limit of at "
            "least 5"
        );
    }
    const int size = randomInt(random, 2, 20);
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

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto rangeSum = [&](int left, int right) {
        long long sum = 0;
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };
    auto assignRange = [&](int left, int right, long long value) {
        for (int index = left; index <= right; ++index) {
            values[static_cast<std::size_t>(index)] = value;
        }
    };

    const int fullValue = randomValue(random);
    input << "range_assign 0 " << (size - 1) << ' ' << fullValue << '\n';
    assignRange(0, size - 1, fullValue);
    input << "range 0 0\n";
    output << values.front() << '\n';
    input << "range 0 " << (size - 1) << '\n';
    output << rangeSum(0, size - 1) << '\n';
    const int middle = size / 2;
    const int suffixValue = randomValue(random);
    input << "range_assign " << middle << ' ' << (size - 1) << ' '
          << suffixValue << '\n';
    assignRange(middle, size - 1, suffixValue);
    input << "get " << middle << '\n';
    output << values[static_cast<std::size_t>(middle)] << '\n';

    for (int operation = 5; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "range_assign " << left << ' ' << right << ' '
                      << value << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    assignRange(left, right, value);
                }
                break;
            }
            case 1: {
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 2: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 3: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(left, right) << '\n';
                }
                break;
            }
            case 4:
                input << "all\n";
                output << rangeSum(0, size - 1) << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

}  // namespace

void registerSegmentTreeGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace(
        "A09-segment-tree-recursive-sum",
        generateSegmentTreeRecursiveSum
    );
    registry.emplace(
        "A10-segment-tree-iterative-sum",
        generateSegmentTreeIterativeSum
    );
    registry.emplace(
        "A11-segment-tree-min-max-monoid",
        generateSegmentTreeMinMax
    );
    registry.emplace(
        "A12-segment-tree-gcd-monoid",
        generateSegmentTreeGcd
    );
    registry.emplace(
        "A13-segment-tree-lazy-range-add",
        generateSegmentTreeLazyAdd
    );
    registry.emplace(
        "A14-segment-tree-lazy-range-assign",
        generateSegmentTreeLazyAssign
    );
}

}  // namespace judge::generators
