#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef DET_NO_ROW_SWAP
#define DET_NO_ROW_SWAP 0
#endif
#ifndef DET_NO_SWAP_SIGN
#define DET_NO_SWAP_SIGN 0
#endif

using det64 = long long;
constexpr det64 DET_MOD = 1'000'000'007;

det64 normalizeDet(det64 value) {
    value %= DET_MOD;
    if (value < 0) value += DET_MOD;
    return value;
}

det64 multiplyDet(det64 left, det64 right) {
    return static_cast<det64>(
        static_cast<__int128_t>(left) * right % DET_MOD
    );
}

det64 powerDet(det64 base, det64 exponent) {
    det64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyDet(result, base);
        base = multiplyDet(base, base);
        exponent >>= 1;
    }
    return result;
}

det64 determinant(std::vector<std::vector<det64>> matrix) {
    const int size = static_cast<int>(matrix.size());
    det64 answer = 1;
    for (int column = 0; column < size; ++column) {
        int selected = -1;
        if (DET_NO_ROW_SWAP) {
            if (matrix[column][column] != 0) selected = column;
        } else {
            for (int row = column; row < size; ++row) {
                if (matrix[row][column] != 0) {
                    selected = row;
                    break;
                }
            }
        }
        if (selected == -1) return 0;
        if (selected != column) {
            std::swap(matrix[column], matrix[selected]);
            if (!DET_NO_SWAP_SIGN) answer = normalizeDet(-answer);
        }
        answer = multiplyDet(answer, matrix[column][column]);
        const det64 inverse = powerDet(matrix[column][column], DET_MOD - 2);
        for (int row = column + 1; row < size; ++row) {
            if (matrix[row][column] == 0) continue;
            const det64 factor = multiplyDet(matrix[row][column], inverse);
            for (int current = column; current < size; ++current) {
                matrix[row][current] = normalizeDet(
                    matrix[row][current] - multiplyDet(factor, matrix[column][current])
                );
            }
        }
    }
    return answer;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int size;
        std::cin >> operation >> size;
        std::vector<std::vector<det64>> matrix(size, std::vector<det64>(size, 0));
        for (auto& row : matrix) {
            for (det64& value : row) std::cin >> value;
        }
        std::cout << determinant(matrix) << '\n';
    }
    return 0;
}
