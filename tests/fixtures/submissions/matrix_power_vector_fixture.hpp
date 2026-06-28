#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef MATRIX_POWER_OFF_BY_ONE
#define MATRIX_POWER_OFF_BY_ONE 0
#endif
#ifndef MATRIX_POWER_TRANSPOSED_VECTOR
#define MATRIX_POWER_TRANSPOSED_VECTOR 0
#endif

using matrix64 = long long;
using MatrixPower = std::vector<std::vector<matrix64>>;
constexpr matrix64 MATRIX_MOD = 1'000'000'007;

matrix64 multiplyMatrixValue(matrix64 left, matrix64 right) {
    return static_cast<matrix64>(
        static_cast<__int128_t>(left) * right % MATRIX_MOD
    );
}

MatrixPower multiplyMatrixPower(const MatrixPower& left, const MatrixPower& right) {
    const int size = static_cast<int>(left.size());
    MatrixPower result(size, std::vector<matrix64>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int middle = 0; middle < size; ++middle) {
            if (left[row][middle] == 0) continue;
            for (int column = 0; column < size; ++column) {
                result[row][column] = (result[row][column]
                    + multiplyMatrixValue(left[row][middle], right[middle][column]))
                    % MATRIX_MOD;
            }
        }
    }
    return result;
}

std::vector<matrix64> applyMatrixPower(
    MatrixPower base,
    std::uint64_t exponent,
    const std::vector<matrix64>& vector
) {
    const int size = static_cast<int>(base.size());
    MatrixPower result(size, std::vector<matrix64>(size, 0));
    for (int index = 0; index < size; ++index) result[index][index] = 1;
    if (MATRIX_POWER_OFF_BY_ONE) result = base;

    while (exponent > 0) {
        if (exponent & 1) result = multiplyMatrixPower(result, base);
        base = multiplyMatrixPower(base, base);
        exponent >>= 1;
    }

    std::vector<matrix64> answer(size, 0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            if (MATRIX_POWER_TRANSPOSED_VECTOR) {
                answer[row] = (answer[row]
                    + multiplyMatrixValue(result[column][row], vector[column]))
                    % MATRIX_MOD;
            } else {
                answer[row] = (answer[row]
                    + multiplyMatrixValue(result[row][column], vector[column]))
                    % MATRIX_MOD;
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
        std::uint64_t exponent;
        std::cin >> operation >> size >> exponent;
        MatrixPower matrix(size, std::vector<matrix64>(size, 0));
        std::vector<matrix64> vector(size, 0);
        for (auto& row : matrix) {
            for (matrix64& value : row) std::cin >> value;
        }
        for (matrix64& value : vector) std::cin >> value;

        const std::vector<matrix64> answer = applyMatrixPower(matrix, exponent, vector);
        for (int index = 0; index < size; ++index) {
            if (index != 0) std::cout << ' ';
            std::cout << answer[index];
        }
        std::cout << '\n';
    }
    return 0;
}
