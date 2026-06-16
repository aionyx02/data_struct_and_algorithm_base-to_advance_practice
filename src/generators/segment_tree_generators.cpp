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
#include <utility>
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

GeneratedCase generateSegmentTreeMixedLazy(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 7) {
        throw std::runtime_error(
            "A15-segment-tree-lazy-mixed-tags requires operation_limit of at "
            "least 7"
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
    auto assignRange = [&](int left, int right, long long value) {
        for (int index = left; index <= right; ++index) {
            values[static_cast<std::size_t>(index)] = value;
        }
    };

    const int fullAssign = randomValue(random);
    const int fullDelta = randomInt(random, 1, 20);
    const int middle = size / 2;
    input << "range_assign 0 " << (size - 1) << ' ' << fullAssign << '\n';
    assignRange(0, size - 1, fullAssign);
    input << "range_add 0 " << (size - 1) << ' ' << fullDelta << '\n';
    addRange(0, size - 1, fullDelta);
    input << "get " << middle << '\n';
    output << values[static_cast<std::size_t>(middle)] << '\n';

    const int suffixAssign = randomValue(random);
    const int prefixDelta = -randomInt(random, 1, 20);
    input << "range_assign " << middle << ' ' << (size - 1) << ' '
          << suffixAssign << '\n';
    assignRange(middle, size - 1, suffixAssign);
    input << "range_add 0 " << middle << ' ' << prefixDelta << '\n';
    addRange(0, middle, prefixDelta);
    input << "range " << middle << ' ' << (size - 1) << '\n';
    output << rangeSum(middle, size - 1) << '\n';
    input << "all\n";
    output << rangeSum(0, size - 1) << '\n';

    for (int operation = 7; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
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
            case 2: {
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
            case 3: {
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
            case 4: {
                const int index = randomInt(random, -2, size + 1);
                input << "get " << index << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << values[static_cast<std::size_t>(index)] << '\n';
                }
                break;
            }
            case 5: {
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
            case 6:
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

GeneratedCase generateSegmentTreePositionalSearch(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 7) {
        throw std::runtime_error(
            "A16-segment-tree-positional-search requires operation_limit of at "
            "least 7"
        );
    }
    const int size = randomInt(random, 5, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    values[0] = 900 + randomInt(random, 0, 50);
    values[2] = 700 + randomInt(random, 0, 50);
    values[static_cast<std::size_t>(size - 2)] =
        650 + randomInt(random, 0, 50);
    values[static_cast<std::size_t>(size - 1)] =
        1000 + randomInt(random, 0, 50);
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
    auto rangeMax = [&](int left, int right) {
        long long result = values[static_cast<std::size_t>(left)];
        for (int index = left + 1; index <= right; ++index) {
            result = std::max(result, values[static_cast<std::size_t>(index)]);
        }
        return result;
    };
    auto firstAtLeast = [&](int left, long long threshold) {
        for (int index = left; index < size; ++index) {
            if (values[static_cast<std::size_t>(index)] >= threshold) {
                return index;
            }
        }
        return -1;
    };
    auto lastAtLeast = [&](int right, long long threshold) {
        for (int index = right; index >= 0; --index) {
            if (values[static_cast<std::size_t>(index)] >= threshold) {
                return index;
            }
        }
        return -1;
    };
    auto appendPosition = [&](int position) {
        if (position == -1) {
            appendLine(output, "NONE");
        } else {
            output << position << '\n';
        }
    };

    input << "first_at_least 2 650\n";
    appendPosition(firstAtLeast(2, 650));
    input << "last_at_least " << (size - 2) << " 650\n";
    appendPosition(lastAtLeast(size - 2, 650));
    input << "range_max 1 " << (size - 2) << '\n';
    output << rangeMax(1, size - 2) << '\n';
    input << "set 0 -1000\n";
    values[0] = -1000;
    input << "add " << (size - 1) << " -900\n";
    values[static_cast<std::size_t>(size - 1)] -= 900;
    input << "first_at_least 0 650\n";
    appendPosition(firstAtLeast(0, 650));
    input << "last_at_least " << (size - 1) << " 650\n";
    appendPosition(lastAtLeast(size - 1, 650));

    for (int operation = 7; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
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
                input << "range_max " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeMax(left, right) << '\n';
                }
                break;
            }
            case 4: {
                const int left = randomInt(random, -2, size + 1);
                const int threshold = randomValue(random);
                input << "first_at_least " << left << ' ' << threshold << '\n';
                if (!valid(left)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendPosition(firstAtLeast(left, threshold));
                }
                break;
            }
            case 5: {
                const int right = randomInt(random, -2, size + 1);
                const int threshold = randomValue(random);
                input << "last_at_least " << right << ' ' << threshold << '\n';
                if (!valid(right)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendPosition(lastAtLeast(right, threshold));
                }
                break;
            }
            case 6:
                input << "all_max\n";
                output << rangeMax(0, size - 1) << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreeDynamicSparse(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "A17-segment-tree-dynamic-sparse requires operation_limit of at "
            "least 8"
        );
    }
    const long long universe =
        1000000000000LL - static_cast<long long>(randomInt(random, 0, 999999));
    std::vector<std::pair<long long, long long>> values;
    std::vector<std::pair<long long, long long>> allocatedSegments;
    allocatedSegments.push_back({0, universe - 1});

    auto hasSegment = [&](long long left, long long right) {
        for (const auto& segment : allocatedSegments) {
            if (segment.first == left && segment.second == right) {
                return true;
            }
        }
        return false;
    };
    auto rememberSegment = [&](long long left, long long right) {
        if (!hasSegment(left, right)) {
            allocatedSegments.push_back({left, right});
        }
    };
    auto allocatePath = [&](long long coordinate) {
        long long left = 0;
        long long right = universe - 1;
        while (left < right) {
            const long long middle = left + (right - left) / 2;
            if (coordinate <= middle) {
                right = middle;
            } else {
                left = middle + 1;
            }
            rememberSegment(left, right);
        }
    };
    auto getPoint = [&](long long coordinate) {
        for (const auto& point : values) {
            if (point.first == coordinate) {
                return point.second;
            }
        }
        return 0LL;
    };
    auto writePoint = [&](long long coordinate, long long value) {
        for (auto iterator = values.begin(); iterator != values.end();
             ++iterator) {
            if (iterator->first == coordinate) {
                if (value == 0) {
                    values.erase(iterator);
                } else {
                    iterator->second = value;
                }
                return;
            }
        }
        if (value != 0) {
            values.push_back({coordinate, value});
        }
    };
    auto addPoint = [&](long long coordinate, long long delta) {
        if (delta == 0) {
            return;
        }
        allocatePath(coordinate);
        writePoint(coordinate, getPoint(coordinate) + delta);
    };
    auto setPoint = [&](long long coordinate, long long value) {
        const long long current = getPoint(coordinate);
        if (current == value) {
            return;
        }
        allocatePath(coordinate);
        writePoint(coordinate, value);
    };
    auto rangeSum = [&](long long left, long long right) {
        long long sum = 0;
        for (const auto& point : values) {
            if (left <= point.first && point.first <= right) {
                sum += point.second;
            }
        }
        return sum;
    };
    auto valid = [&](long long coordinate) {
        return 0 <= coordinate && coordinate < universe;
    };
    auto randomCoordinate = [&]() {
        return static_cast<long long>(
            random() % static_cast<unsigned long long>(universe)
        );
    };
    auto randomMaybeInvalidCoordinate = [&]() -> long long {
        switch (randomInt(random, 0, 9)) {
            case 0:
                return -randomInt(random, 1, 3);
            case 1:
                return universe + randomInt(random, 0, 2);
            default:
                return randomCoordinate();
        }
    };

    const long long firstCoordinate = randomCoordinate();
    long long secondCoordinate = randomCoordinate();
    if (secondCoordinate == firstCoordinate) {
        secondCoordinate = (secondCoordinate + 1) % universe;
    }

    std::ostringstream input;
    std::ostringstream output;
    input << universe << ' ' << operationCount << '\n';

    input << "nodes\n";
    output << allocatedSegments.size() << '\n';
    input << "set 0 0\n";
    setPoint(0, 0);
    input << "nodes\n";
    output << allocatedSegments.size() << '\n';
    input << "add " << firstCoordinate << " 5\n";
    addPoint(firstCoordinate, 5);
    input << "nodes\n";
    output << allocatedSegments.size() << '\n';
    input << "add " << secondCoordinate << " -7\n";
    addPoint(secondCoordinate, -7);
    input << "range 0 " << (universe - 1) << '\n';
    output << rangeSum(0, universe - 1) << '\n';
    input << "nodes\n";
    output << allocatedSegments.size() << '\n';

    for (int operation = 8; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0: {
                const long long coordinate = randomMaybeInvalidCoordinate();
                const int delta = randomValue(random);
                input << "add " << coordinate << ' ' << delta << '\n';
                if (!valid(coordinate)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    addPoint(coordinate, delta);
                }
                break;
            }
            case 1: {
                const long long coordinate = randomMaybeInvalidCoordinate();
                const int value = randomValue(random);
                input << "set " << coordinate << ' ' << value << '\n';
                if (!valid(coordinate)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    setPoint(coordinate, value);
                }
                break;
            }
            case 2: {
                const long long coordinate = randomMaybeInvalidCoordinate();
                input << "get " << coordinate << '\n';
                if (!valid(coordinate)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << getPoint(coordinate) << '\n';
                }
                break;
            }
            case 3: {
                const long long coordinate = randomMaybeInvalidCoordinate();
                input << "prefix " << coordinate << '\n';
                if (!valid(coordinate)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(0, coordinate) << '\n';
                }
                break;
            }
            case 4: {
                const long long left = randomMaybeInvalidCoordinate();
                const long long right = randomMaybeInvalidCoordinate();
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
                output << rangeSum(0, universe - 1) << '\n';
                break;
            case 6:
                input << "nodes\n";
                output << allocatedSegments.size() << '\n';
                break;
            default:
                input << "universe\n";
                output << universe << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreePersistentVersions(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 9) {
        throw std::runtime_error(
            "A18-segment-tree-persistent-versions requires operation_limit of "
            "at least 9"
        );
    }
    const int size = randomInt(random, 2, 20);
    std::vector<long long> initial(static_cast<std::size_t>(size));
    std::vector<std::vector<long long>> versions;

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        initial[static_cast<std::size_t>(index)] = randomValue(random);
        if (index != 0) {
            input << ' ';
        }
        input << initial[static_cast<std::size_t>(index)];
    }
    input << '\n';
    versions.push_back(initial);

    auto validVersion = [&](int version) {
        return 0 <= version &&
               version < static_cast<int>(versions.size());
    };
    auto validIndex = [&](int index) {
        return 0 <= index && index < size;
    };
    auto rangeSum = [&](int version, int left, int right) {
        long long sum = 0;
        const auto& values = versions[static_cast<std::size_t>(version)];
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };
    auto createSetVersion = [&](int baseVersion, int index, long long value) {
        auto values = versions[static_cast<std::size_t>(baseVersion)];
        values[static_cast<std::size_t>(index)] = value;
        versions.push_back(values);
        return static_cast<int>(versions.size()) - 1;
    };
    auto createAddVersion = [&](int baseVersion, int index, long long delta) {
        auto values = versions[static_cast<std::size_t>(baseVersion)];
        values[static_cast<std::size_t>(index)] += delta;
        versions.push_back(values);
        return static_cast<int>(versions.size()) - 1;
    };

    const int middle = size / 2;
    const long long newMiddleValue = randomValue(random) + 2000;
    const long long firstDelta = -randomInt(random, 1, 20);

    input << "range 0 0 " << (size - 1) << '\n';
    output << rangeSum(0, 0, size - 1) << '\n';
    input << "set 0 " << middle << ' ' << newMiddleValue << '\n';
    output << createSetVersion(0, middle, newMiddleValue) << '\n';
    input << "range 0 0 " << (size - 1) << '\n';
    output << rangeSum(0, 0, size - 1) << '\n';
    input << "range 1 0 " << (size - 1) << '\n';
    output << rangeSum(1, 0, size - 1) << '\n';
    input << "add 1 0 " << firstDelta << '\n';
    output << createAddVersion(1, 0, firstDelta) << '\n';
    input << "get 0 " << middle << '\n';
    output << versions[0][static_cast<std::size_t>(middle)] << '\n';
    input << "get 1 " << middle << '\n';
    output << versions[1][static_cast<std::size_t>(middle)] << '\n';
    input << "all 2\n";
    output << rangeSum(2, 0, size - 1) << '\n';
    input << "versions\n";
    output << versions.size() << '\n';

    for (int operation = 9; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int baseVersion = randomInt(
                    random,
                    -2,
                    static_cast<int>(versions.size()) + 1
                );
                const int index = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "set " << baseVersion << ' ' << index << ' ' << value
                      << '\n';
                if (!validVersion(baseVersion) || !validIndex(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << createSetVersion(baseVersion, index, value)
                           << '\n';
                }
                break;
            }
            case 1: {
                const int baseVersion = randomInt(
                    random,
                    -2,
                    static_cast<int>(versions.size()) + 1
                );
                const int index = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "add " << baseVersion << ' ' << index << ' ' << delta
                      << '\n';
                if (!validVersion(baseVersion) || !validIndex(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << createAddVersion(baseVersion, index, delta)
                           << '\n';
                }
                break;
            }
            case 2: {
                const int version = randomInt(
                    random,
                    -2,
                    static_cast<int>(versions.size()) + 1
                );
                const int index = randomInt(random, -2, size + 1);
                input << "get " << version << ' ' << index << '\n';
                if (!validVersion(version) || !validIndex(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << versions[static_cast<std::size_t>(version)]
                                      [static_cast<std::size_t>(index)]
                           << '\n';
                }
                break;
            }
            case 3: {
                const int version = randomInt(
                    random,
                    -2,
                    static_cast<int>(versions.size()) + 1
                );
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "range " << version << ' ' << left << ' ' << right
                      << '\n';
                if (!validVersion(version) || !validIndex(left) ||
                    !validIndex(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(version, left, right) << '\n';
                }
                break;
            }
            case 4: {
                const int version = randomInt(
                    random,
                    -2,
                    static_cast<int>(versions.size()) + 1
                );
                input << "all " << version << '\n';
                if (!validVersion(version)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(version, 0, size - 1) << '\n';
                }
                break;
            }
            case 5:
                input << "versions\n";
                output << versions.size() << '\n';
                break;
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateSegmentTreeOrderStatistic(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "A19-segment-tree-order-statistic requires operation_limit of at "
            "least 6"
        );
    }
    const int universe = randomInt(random, 4, 20);
    std::vector<long long> freq(static_cast<std::size_t>(universe));
    long long total = 0;

    std::ostringstream input;
    std::ostringstream output;
    input << universe << ' ' << operationCount << '\n';

    // Number of stored elements strictly less than bound, bound in [0, universe].
    auto less = [&](int bound) {
        long long sum = 0;
        for (int key = 0; key < bound; ++key) {
            sum += freq[static_cast<std::size_t>(key)];
        }
        return sum;
    };
    // One-based k-th smallest key; -1 when order is below 1 or above total.
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

    // Fixed invariant prefix: keys 0, 2, 3 leave the second-smallest element in
    // the tree's right half, so a k-th walk that forgets to subtract the left
    // count descends the wrong way and answers 3 instead of 2. The `less 2`
    // probe lands on a stored key, so an inclusive (<=) rank answers 2 not 1.
    input << "insert 0\n";
    freq[0] += 1;
    total += 1;
    input << "insert 2\n";
    freq[2] += 1;
    total += 1;
    input << "insert 3\n";
    freq[3] += 1;
    total += 1;
    input << "kth 2\n";
    output << kth(2) << '\n';
    input << "less 2\n";
    output << less(2) << '\n';
    input << "size\n";
    output << total << '\n';

    for (int operation = 6; operation < operationCount; ++operation) {
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
    registry.emplace(
        "A15-segment-tree-lazy-mixed-tags",
        generateSegmentTreeMixedLazy
    );
    registry.emplace(
        "A16-segment-tree-positional-search",
        generateSegmentTreePositionalSearch
    );
    registry.emplace(
        "A17-segment-tree-dynamic-sparse",
        generateSegmentTreeDynamicSparse
    );
    registry.emplace(
        "A18-segment-tree-persistent-versions",
        generateSegmentTreePersistentVersions
    );
    registry.emplace(
        "A19-segment-tree-order-statistic",
        generateSegmentTreeOrderStatistic
    );
}

}  // namespace judge::generators
