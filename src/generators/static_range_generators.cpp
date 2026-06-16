#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

GeneratedCase generateSparseTableRangeQuery(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A21-sparse-table-range-query requires operation_limit of at "
            "least 4"
        );
    }
    const int size = randomInt(random, 3, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // Pin a length-three prefix whose unique minimum sits at the right endpoint.
    // A correct O(1) query combines two overlapping power-of-two blocks; the
    // probe `min 0 2` therefore exposes a query that drops the right endpoint
    // and a query that consults only one of the two blocks.
    values[0] = 5;
    values[1] = 3;
    values[2] = 1;

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
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
    auto rangeMin = [&](int left, int right) {
        long long result = values[static_cast<std::size_t>(left)];
        for (int index = left + 1; index <= right; ++index) {
            result = std::min(result, values[static_cast<std::size_t>(index)]);
        }
        return result;
    };
    auto rangeMax = [&](int left, int right) {
        long long result = values[static_cast<std::size_t>(left)];
        for (int index = left + 1; index <= right; ++index) {
            result = std::max(result, values[static_cast<std::size_t>(index)]);
        }
        return result;
    };

    input << "min 0 2\n";
    output << rangeMin(0, 2) << '\n';
    input << "max 0 2\n";
    output << rangeMax(0, 2) << '\n';
    input << "get 2\n";
    output << values[2] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 3)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "min " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeMin(left, right) << '\n';
                }
                break;
            }
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "max " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeMax(left, right) << '\n';
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateDisjointSparseTableSum(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A22-disjoint-sparse-table-sum requires operation_limit of at "
            "least 4"
        );
    }
    const int size = randomInt(random, 3, 20);
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // Pin a non-zero middle element so that a query over a non-power-of-two
    // range double-counts the overlap if the structure reuses an overlapping
    // Sparse Table, and a single-element query doubles if the structure forgets
    // the `left == right` case.
    values[0] = 3;
    values[1] = 5;
    values[2] = 2;

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
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
    auto rangeSum = [&](int left, int right) {
        long long sum = 0;
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };

    input << "sum 0 2\n";
    output << rangeSum(0, 2) << '\n';
    input << "sum 1 1\n";
    output << rangeSum(1, 1) << '\n';
    input << "get 1\n";
    output << values[1] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 3)) {
            case 0:
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                input << "sum " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeSum(left, right) << '\n';
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

}  // namespace

void registerStaticRangeGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace(
        "A21-sparse-table-range-query",
        generateSparseTableRangeQuery
    );
    registry.emplace(
        "A22-disjoint-sparse-table-sum",
        generateDisjointSparseTableSum
    );
}

}  // namespace judge::generators
