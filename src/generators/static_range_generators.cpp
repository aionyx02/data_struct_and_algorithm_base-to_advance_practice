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

GeneratedCase generateSqrtDecompositionRangeSum(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "A23-sqrt-decomposition-range-sum requires operation_limit of at "
            "least 5"
        );
    }
    // At least three blocks so that block 1 is a strict interior block whose
    // cached sum is consulted by a full-array query.
    const int size = randomInt(random, 9, 30);
    int blockSize = 1;
    while ((blockSize + 1) * (blockSize + 1) <= size) {
        ++blockSize;
    }
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // A non-zero first element makes a "sum of whole blocks" query that ignores
    // the partial left block over-count by a[0].
    values[0] = 7;

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

    // sum 1 (n-1) over-counts a[0] for a whole-block solution that ignores the
    // partial left block; the later interior update over a full-array query
    // catches a solution that forgets to refresh the cached block sum.
    input << "sum 1 " << (size - 1) << '\n';
    output << rangeSum(1, size - 1) << '\n';
    input << "add " << blockSize << " 100\n";
    values[static_cast<std::size_t>(blockSize)] += 100;
    input << "sum 0 " << (size - 1) << '\n';
    output << rangeSum(0, size - 1) << '\n';
    input << "get " << blockSize << '\n';
    output << values[static_cast<std::size_t>(blockSize)] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 5; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 4)) {
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBlockDecompositionLazy(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "A24-block-decomposition-lazy requires operation_limit of at "
            "least 4"
        );
    }
    // At least three blocks so that block 1 becomes a strict interior block that
    // receives a lazy tag from a full-array range add.
    const int size = randomInt(random, 9, 30);
    int blockSize = 1;
    while ((blockSize + 1) * (blockSize + 1) <= size) {
        ++blockSize;
    }
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }

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
    auto rangeAdd = [&](int left, int right, long long delta) {
        for (int index = left; index <= right; ++index) {
            values[static_cast<std::size_t>(index)] += delta;
        }
    };
    auto rangeSum = [&](int left, int right) {
        long long sum = 0;
        for (int index = left; index <= right; ++index) {
            sum += values[static_cast<std::size_t>(index)];
        }
        return sum;
    };

    // A full-array range add gives block 1 a lazy tag. The following full-array
    // sum catches a solution that forgets to fold the lazy tag into the cached
    // block sum, and the get over block 1 catches a solution that reads partial
    // cells without adding the block's lazy tag.
    input << "add 0 " << (size - 1) << " 10\n";
    rangeAdd(0, size - 1, 10);
    input << "sum 0 " << (size - 1) << '\n';
    output << rangeSum(0, size - 1) << '\n';
    input << "get " << blockSize << '\n';
    output << values[static_cast<std::size_t>(blockSize)] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 3)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int delta = randomValue(random);
                input << "add " << left << ' ' << right << ' ' << delta << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    rangeAdd(left, right, delta);
                }
                break;
            }
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

GeneratedCase generateMergeSortTreeRangeCount(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "A25-merge-sort-tree-range-count requires operation_limit of at "
            "least 6"
        );
    }
    const int size = randomInt(random, 6, 24);
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // Duplicates at the right endpoint expose count_le implementations that use
    // lower_bound, and range traversals that accidentally drop the right edge.
    values[0] = 4;
    values[1] = 8;
    values[2] = 1;
    values[3] = 8;
    values[4] = 2;
    values[5] = 8;

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
    auto countLess = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] < value) {
                ++count;
            }
        }
        return count;
    };
    auto countLessEqual = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] <= value) {
                ++count;
            }
        }
        return count;
    };
    auto kthSmallest = [&](int left, int right, int kth) {
        std::vector<long long> sorted;
        sorted.reserve(static_cast<std::size_t>(right - left + 1));
        for (int index = left; index <= right; ++index) {
            sorted.push_back(values[static_cast<std::size_t>(index)]);
        }
        std::sort(sorted.begin(), sorted.end());
        return sorted[static_cast<std::size_t>(kth - 1)];
    };

    input << "count_le 0 5 8\n";
    output << countLessEqual(0, 5, 8) << '\n';
    input << "count_lt 0 5 8\n";
    output << countLess(0, 5, 8) << '\n';
    input << "count_le 3 5 8\n";
    output << countLessEqual(3, 5, 8) << '\n';
    input << "kth 0 5 5\n";
    output << kthSmallest(0, 5, 5) << '\n';
    input << "get 2\n";
    output << values[2] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 6; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 4)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "count_lt " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << countLess(left, right, value) << '\n';
                }
                break;
            }
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "count_le " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << countLessEqual(left, right, value) << '\n';
                }
                break;
            }
            case 2: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int kth = randomInt(random, -1, size + 2);
                input << "kth " << left << ' ' << right << ' ' << kth << '\n';
                if (!valid(left) || !valid(right) || left > right || kth < 1 ||
                    kth > right - left + 1) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << kthSmallest(left, right, kth) << '\n';
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateWaveletTreeRangeKth(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 7) {
        throw std::runtime_error(
            "A26-wavelet-tree-range-kth requires operation_limit of at least 7"
        );
    }
    const int size = randomInt(random, 6, 24);
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // This prefix forces kth queries into the right child after a non-empty
    // left partition, and count queries over [3, 5] require correct right-child
    // range translation.
    values[0] = 4;
    values[1] = 8;
    values[2] = 1;
    values[3] = 9;
    values[4] = 2;
    values[5] = 10;

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
    auto countLessEqual = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] <= value) {
                ++count;
            }
        }
        return count;
    };
    auto frequency = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] == value) {
                ++count;
            }
        }
        return count;
    };
    auto kthSmallest = [&](int left, int right, int kth) {
        std::vector<long long> sorted;
        sorted.reserve(static_cast<std::size_t>(right - left + 1));
        for (int index = left; index <= right; ++index) {
            sorted.push_back(values[static_cast<std::size_t>(index)]);
        }
        std::sort(sorted.begin(), sorted.end());
        return sorted[static_cast<std::size_t>(kth - 1)];
    };

    input << "count_le 0 5 4\n";
    output << countLessEqual(0, 5, 4) << '\n';
    input << "count_le 0 5 8\n";
    output << countLessEqual(0, 5, 8) << '\n';
    input << "kth 0 5 5\n";
    output << kthSmallest(0, 5, 5) << '\n';
    input << "count_le 3 5 9\n";
    output << countLessEqual(3, 5, 9) << '\n';
    input << "freq 0 5 8\n";
    output << frequency(0, 5, 8) << '\n';
    input << "get 3\n";
    output << values[3] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 7; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 4)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int kth = randomInt(random, -1, size + 2);
                input << "kth " << left << ' ' << right << ' ' << kth << '\n';
                if (!valid(left) || !valid(right) || left > right || kth < 1 ||
                    kth > right - left + 1) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << kthSmallest(left, right, kth) << '\n';
                }
                break;
            }
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "count_le " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << countLessEqual(left, right, value) << '\n';
                }
                break;
            }
            case 2: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "freq " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << frequency(left, right, value) << '\n';
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateWaveletMatrixRangeKth(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "A27-wavelet-matrix-range-kth requires operation_limit of at least 8"
        );
    }
    const int size = randomInt(random, 6, 24);
    std::vector<long long> values(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        values[static_cast<std::size_t>(index)] = randomValue(random);
    }
    // The prefix forces kth into the one-bit side at the top level and makes a
    // non-prefix count query over [3, 5] depend on the level's zero-count offset.
    values[0] = 4;
    values[1] = 8;
    values[2] = 1;
    values[3] = 9;
    values[4] = 2;
    values[5] = 10;

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
    auto countLess = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] < value) {
                ++count;
            }
        }
        return count;
    };
    auto countLessEqual = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] <= value) {
                ++count;
            }
        }
        return count;
    };
    auto frequency = [&](int left, int right, long long value) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values[static_cast<std::size_t>(index)] == value) {
                ++count;
            }
        }
        return count;
    };
    auto kthSmallest = [&](int left, int right, int kth) {
        std::vector<long long> sorted;
        sorted.reserve(static_cast<std::size_t>(right - left + 1));
        for (int index = left; index <= right; ++index) {
            sorted.push_back(values[static_cast<std::size_t>(index)]);
        }
        std::sort(sorted.begin(), sorted.end());
        return sorted[static_cast<std::size_t>(kth - 1)];
    };

    input << "count_lt 0 5 4\n";
    output << countLess(0, 5, 4) << '\n';
    input << "count_le 0 5 4\n";
    output << countLessEqual(0, 5, 4) << '\n';
    input << "count_le 0 5 8\n";
    output << countLessEqual(0, 5, 8) << '\n';
    input << "kth 0 5 5\n";
    output << kthSmallest(0, 5, 5) << '\n';
    input << "count_le 3 5 9\n";
    output << countLessEqual(3, 5, 9) << '\n';
    input << "freq 0 5 8\n";
    output << frequency(0, 5, 8) << '\n';
    input << "get 3\n";
    output << values[3] << '\n';
    input << "size\n";
    output << size << '\n';

    for (int operation = 8; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int kth = randomInt(random, -1, size + 2);
                input << "kth " << left << ' ' << right << ' ' << kth << '\n';
                if (!valid(left) || !valid(right) || left > right || kth < 1 ||
                    kth > right - left + 1) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << kthSmallest(left, right, kth) << '\n';
                }
                break;
            }
            case 1: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "count_lt " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << countLess(left, right, value) << '\n';
                }
                break;
            }
            case 2: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "count_le " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << countLessEqual(left, right, value) << '\n';
                }
                break;
            }
            case 3: {
                const int left = randomInt(random, -2, size + 1);
                const int right = randomInt(random, -2, size + 1);
                const int value = randomValue(random);
                input << "freq " << left << ' ' << right << ' ' << value
                      << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << frequency(left, right, value) << '\n';
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
            default:
                input << "size\n";
                output << size << '\n';
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBitsetSetRepresentation(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 10) {
        throw std::runtime_error(
            "A28-bitset-set-representation requires operation_limit of at "
            "least 10"
        );
    }
    const int size = randomInt(random, 70, 150);
    std::vector<int> present(static_cast<std::size_t>(size), 0);
    present[0] = 1;
    present[63] = 1;
    present[64] = 1;
    present[static_cast<std::size_t>(size - 1)] = 1;
    for (int index = 70; index + 1 < size; ++index) {
        present[static_cast<std::size_t>(index)] =
            randomInt(random, 0, 4) == 0 ? 1 : 0;
    }

    std::ostringstream input;
    std::ostringstream output;
    input << size << ' ' << operationCount << '\n';
    for (int index = 0; index < size; ++index) {
        if (index != 0) {
            input << ' ';
        }
        input << present[static_cast<std::size_t>(index)];
    }
    input << '\n';

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto totalCount = [&]() {
        int count = 0;
        for (int value : present) {
            count += value;
        }
        return count;
    };
    auto rangeCount = [&](int left, int right) {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            count += present[static_cast<std::size_t>(index)];
        }
        return count;
    };
    auto firstGreaterEqual = [&](int key) {
        for (int index = key; index < size; ++index) {
            if (present[static_cast<std::size_t>(index)] != 0) {
                return index;
            }
        }
        return -1;
    };

    input << "count 1 62\n";
    output << rangeCount(1, 62) << '\n';
    input << "count 0 63\n";
    output << rangeCount(0, 63) << '\n';
    input << "count 63 64\n";
    output << rangeCount(63, 64) << '\n';
    input << "first_ge 1\n";
    output << firstGreaterEqual(1) << '\n';
    input << "clear 63\n";
    present[63] = 0;
    input << "total\n";
    output << totalCount() << '\n';
    input << "flip 64\n";
    present[64] = 1 - present[64];
    input << "set 68\n";
    present[68] = 1;
    input << "count 64 69\n";
    output << rangeCount(64, 69) << '\n';
    input << "first_ge 65\n";
    output << firstGreaterEqual(65) << '\n';

    for (int operation = 10; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0: {
                const int key = randomInt(random, -2, size + 2);
                input << "set " << key << '\n';
                if (!valid(key)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    present[static_cast<std::size_t>(key)] = 1;
                }
                break;
            }
            case 1: {
                const int key = randomInt(random, -2, size + 2);
                input << "clear " << key << '\n';
                if (!valid(key)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    present[static_cast<std::size_t>(key)] = 0;
                }
                break;
            }
            case 2: {
                const int key = randomInt(random, -2, size + 2);
                input << "flip " << key << '\n';
                if (!valid(key)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    present[static_cast<std::size_t>(key)] =
                        1 - present[static_cast<std::size_t>(key)];
                }
                break;
            }
            case 3: {
                const int key = randomInt(random, -2, size + 2);
                input << "contains " << key << '\n';
                if (!valid(key)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << present[static_cast<std::size_t>(key)] << '\n';
                }
                break;
            }
            case 4: {
                const int left = randomInt(random, -2, size + 2);
                const int right = randomInt(random, -2, size + 2);
                input << "count " << left << ' ' << right << '\n';
                if (!valid(left) || !valid(right) || left > right) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    output << rangeCount(left, right) << '\n';
                }
                break;
            }
            case 5: {
                const int key = randomInt(random, -2, size + 2);
                input << "first_ge " << key << '\n';
                if (!valid(key)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    const int found = firstGreaterEqual(key);
                    if (found < 0) {
                        appendLine(output, "NONE");
                    } else {
                        output << found << '\n';
                    }
                }
                break;
            }
            case 6:
                input << "total\n";
                output << totalCount() << '\n';
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

void registerStaticRangeGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace(
        "A21-sparse-table-range-query",
        generateSparseTableRangeQuery
    );
    registry.emplace(
        "A22-disjoint-sparse-table-sum",
        generateDisjointSparseTableSum
    );
    registry.emplace(
        "A23-sqrt-decomposition-range-sum",
        generateSqrtDecompositionRangeSum
    );
    registry.emplace(
        "A24-block-decomposition-lazy",
        generateBlockDecompositionLazy
    );
    registry.emplace(
        "A25-merge-sort-tree-range-count",
        generateMergeSortTreeRangeCount
    );
    registry.emplace(
        "A26-wavelet-tree-range-kth",
        generateWaveletTreeRangeKth
    );
    registry.emplace(
        "A27-wavelet-matrix-range-kth",
        generateWaveletMatrixRangeKth
    );
    registry.emplace(
        "A28-bitset-set-representation",
        generateBitsetSetRepresentation
    );
}

}  // namespace judge::generators
