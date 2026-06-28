#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef GAUSS_NO_ROW_SWAP
#define GAUSS_NO_ROW_SWAP 0
#endif
#ifndef GAUSS_SKIP_INCONSISTENCY
#define GAUSS_SKIP_INCONSISTENCY 0
#endif

using gauss64 = long long;
constexpr gauss64 GAUSS_MOD = 1'000'000'007;

gauss64 normalizeGauss(gauss64 value) {
    value %= GAUSS_MOD;
    if (value < 0) value += GAUSS_MOD;
    return value;
}

gauss64 multiplyGauss(gauss64 left, gauss64 right) {
    return static_cast<gauss64>(
        static_cast<__int128_t>(left) * right % GAUSS_MOD
    );
}

gauss64 powerGauss(gauss64 base, gauss64 exponent) {
    gauss64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyGauss(result, base);
        base = multiplyGauss(base, base);
        exponent >>= 1;
    }
    return result;
}

struct GaussAnswer {
    enum Kind { kUnique, kNoSolution, kInfinite } kind = kNoSolution;
    int rank = 0;
    std::vector<gauss64> solution;
};

GaussAnswer solveGauss(std::vector<std::vector<gauss64>> matrix) {
    const int equationCount = static_cast<int>(matrix.size());
    const int variableCount = static_cast<int>(matrix.front().size()) - 1;
    for (auto& row : matrix) {
        for (gauss64& value : row) value = normalizeGauss(value);
    }

    std::vector<int> where(variableCount, -1);
    int rank = 0;
    for (int column = 0; column < variableCount; ++column) {
        int selected = -1;
        if (GAUSS_NO_ROW_SWAP) {
            if (rank < equationCount && matrix[rank][column] != 0) selected = rank;
        } else {
            for (int row = rank; row < equationCount; ++row) {
                if (matrix[row][column] != 0) {
                    selected = row;
                    break;
                }
            }
        }
        if (selected == -1) continue;
        std::swap(matrix[rank], matrix[selected]);
        const gauss64 inverse = powerGauss(matrix[rank][column], GAUSS_MOD - 2);
        for (int current = column; current <= variableCount; ++current) {
            matrix[rank][current] = multiplyGauss(matrix[rank][current], inverse);
        }
        for (int row = 0; row < equationCount; ++row) {
            if (row == rank || matrix[row][column] == 0) continue;
            const gauss64 factor = matrix[row][column];
            for (int current = column; current <= variableCount; ++current) {
                matrix[row][current] = normalizeGauss(
                    matrix[row][current] - multiplyGauss(factor, matrix[rank][current])
                );
            }
        }
        where[column] = rank;
        ++rank;
    }

    if (!GAUSS_SKIP_INCONSISTENCY) {
        for (int row = 0; row < equationCount; ++row) {
            bool allZero = true;
            for (int column = 0; column < variableCount; ++column) {
                if (matrix[row][column] != 0) {
                    allZero = false;
                    break;
                }
            }
            if (allZero && matrix[row][variableCount] != 0) {
                return {GaussAnswer::kNoSolution, rank, {}};
            }
        }
    }

    if (std::any_of(where.begin(), where.end(), [](int row) { return row == -1; })) {
        return {GaussAnswer::kInfinite, rank, {}};
    }

    std::vector<gauss64> solution(variableCount, 0);
    for (int column = 0; column < variableCount; ++column) {
        solution[column] = matrix[where[column]][variableCount];
    }
    return {GaussAnswer::kUnique, rank, solution};
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int equationCount;
        int variableCount;
        std::cin >> operation >> equationCount >> variableCount;
        std::vector<std::vector<gauss64>> matrix(
            equationCount,
            std::vector<gauss64>(variableCount + 1, 0)
        );
        for (auto& row : matrix) {
            for (gauss64& value : row) std::cin >> value;
        }
        const GaussAnswer answer = solveGauss(matrix);
        if (answer.kind == GaussAnswer::kNoSolution) {
            std::cout << "NO\n";
        } else if (answer.kind == GaussAnswer::kInfinite) {
            std::cout << "INF " << answer.rank << '\n';
        } else {
            std::cout << "UNIQUE";
            for (gauss64 value : answer.solution) std::cout << ' ' << value;
            std::cout << '\n';
        }
    }
    return 0;
}
