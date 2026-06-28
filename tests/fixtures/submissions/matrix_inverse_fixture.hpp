#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef INVERSE_NO_ROW_SWAP
#define INVERSE_NO_ROW_SWAP 0
#endif
#ifndef INVERSE_SKIP_SINGULAR
#define INVERSE_SKIP_SINGULAR 0
#endif

using inv64 = long long;
constexpr inv64 INV_MOD = 1'000'000'007;

inv64 normalizeInverse(inv64 value) {
    value %= INV_MOD;
    if (value < 0) value += INV_MOD;
    return value;
}

inv64 multiplyInverse(inv64 left, inv64 right) {
    return static_cast<inv64>(
        static_cast<__int128_t>(left) * right % INV_MOD
    );
}

inv64 powerInverse(inv64 base, inv64 exponent) {
    inv64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyInverse(result, base);
        base = multiplyInverse(base, base);
        exponent >>= 1;
    }
    return result;
}

bool invertMatrixFixture(
    std::vector<std::vector<inv64>> matrix,
    std::vector<std::vector<inv64>>& inverse
) {
    const int size = static_cast<int>(matrix.size());
    std::vector<std::vector<inv64>> augmented(size, std::vector<inv64>(2 * size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            augmented[row][column] = normalizeInverse(matrix[row][column]);
        }
        augmented[row][size + row] = 1;
    }

    int rank = 0;
    for (int column = 0; column < size; ++column) {
        int selected = -1;
        if (INVERSE_NO_ROW_SWAP) {
            if (rank < size && augmented[rank][column] != 0) selected = rank;
        } else {
            for (int row = rank; row < size; ++row) {
                if (augmented[row][column] != 0) {
                    selected = row;
                    break;
                }
            }
        }
        if (selected == -1) {
            if (INVERSE_SKIP_SINGULAR) continue;
            return false;
        }
        std::swap(augmented[rank], augmented[selected]);
        const inv64 inversePivot = powerInverse(augmented[rank][column], INV_MOD - 2);
        for (int current = column; current < 2 * size; ++current) {
            augmented[rank][current] = multiplyInverse(augmented[rank][current], inversePivot);
        }
        for (int row = 0; row < size; ++row) {
            if (row == rank || augmented[row][column] == 0) continue;
            const inv64 factor = augmented[row][column];
            for (int current = column; current < 2 * size; ++current) {
                augmented[row][current] = normalizeInverse(
                    augmented[row][current] - multiplyInverse(factor, augmented[rank][current])
                );
            }
        }
        ++rank;
    }
    if (rank < size && !INVERSE_SKIP_SINGULAR) return false;

    inverse.assign(size, std::vector<inv64>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            inverse[row][column] = augmented[row][size + column];
        }
    }
    return true;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int size;
        std::cin >> operation >> size;
        std::vector<std::vector<inv64>> matrix(size, std::vector<inv64>(size, 0));
        for (auto& row : matrix) {
            for (inv64& value : row) std::cin >> value;
        }
        std::vector<std::vector<inv64>> inverse;
        if (!invertMatrixFixture(matrix, inverse)) {
            std::cout << "SINGULAR\n";
            continue;
        }
        for (const auto& row : inverse) {
            for (int column = 0; column < size; ++column) {
                if (column != 0) std::cout << ' ';
                std::cout << row[column];
            }
            std::cout << '\n';
        }
    }
    return 0;
}
