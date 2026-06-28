#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using mp64 = long long;
using MpMatrix = std::vector<std::vector<mp64>>;
constexpr mp64 MP_MOD = 1'000'000'007;

mp64 normalizeMp(mp64 value) {
    value %= MP_MOD;
    if (value < 0) value += MP_MOD;
    return value;
}

mp64 multiplyMp(mp64 left, mp64 right) {
    return static_cast<mp64>(
        static_cast<__int128_t>(left) * right % MP_MOD
    );
}

mp64 powerMp(mp64 base, std::uint64_t exponent) {
    mp64 result = 1;
    base = normalizeMp(base);
    while (exponent > 0) {
        if (exponent & 1ULL) result = multiplyMp(result, base);
        base = multiplyMp(base, base);
        exponent >>= 1ULL;
    }
    return result;
}

MpMatrix identityMp(int size) {
    MpMatrix matrix(size, std::vector<mp64>(size, 0));
    for (int index = 0; index < size; ++index) matrix[index][index] = 1;
    return matrix;
}

MpMatrix multiplyMatricesMp(const MpMatrix& left, const MpMatrix& right) {
    const int size = static_cast<int>(left.size());
    MpMatrix result(size, std::vector<mp64>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int middle = 0; middle < size; ++middle) {
            if (left[row][middle] == 0) continue;
            for (int column = 0; column < size; ++column) {
                result[row][column] = (result[row][column]
                    + multiplyMp(left[row][middle], right[middle][column])) % MP_MOD;
            }
        }
    }
    return result;
}

MpMatrix matrixPowerMp(MpMatrix base, std::uint64_t exponent) {
    MpMatrix result = identityMp(static_cast<int>(base.size()));
    while (exponent > 0) {
        if (exponent & 1ULL) result = multiplyMatricesMp(result, base);
        base = multiplyMatricesMp(base, base);
        exponent >>= 1ULL;
    }
    return result;
}

std::vector<mp64> multiplyMatrixVectorMp(
    const MpMatrix& matrix,
    const std::vector<mp64>& vector
) {
    const int size = static_cast<int>(matrix.size());
    std::vector<mp64> result(size, 0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            result[row] = (result[row]
                + multiplyMp(matrix[row][column], vector[column])) % MP_MOD;
        }
    }
    return result;
}

mp64 determinantMp(MpMatrix matrix) {
    const int size = static_cast<int>(matrix.size());
    mp64 answer = 1;
    for (int column = 0; column < size; ++column) {
        int selected = -1;
        for (int row = column; row < size; ++row) {
            if (matrix[row][column] != 0) {
                selected = row;
                break;
            }
        }
        if (selected == -1) return 0;
        if (selected != column) {
            std::swap(matrix[column], matrix[selected]);
            answer = normalizeMp(-answer);
        }
        answer = multiplyMp(answer, matrix[column][column]);
        const mp64 inverse = powerMp(matrix[column][column], MP_MOD - 2);
        for (int row = column + 1; row < size; ++row) {
            if (matrix[row][column] == 0) continue;
            const mp64 factor = multiplyMp(matrix[row][column], inverse);
            for (int current = column; current < size; ++current) {
                matrix[row][current] = normalizeMp(
                    matrix[row][current] - multiplyMp(factor, matrix[column][current])
                );
            }
        }
    }
    return answer;
}

bool invertMatrixMp(MpMatrix matrix, MpMatrix& inverse) {
    const int size = static_cast<int>(matrix.size());
    MpMatrix augmented(size, std::vector<mp64>(2 * size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            augmented[row][column] = normalizeMp(matrix[row][column]);
        }
        augmented[row][size + row] = 1;
    }
    int rank = 0;
    for (int column = 0; column < size; ++column) {
        int selected = -1;
        for (int row = rank; row < size; ++row) {
            if (augmented[row][column] != 0) {
                selected = row;
                break;
            }
        }
        if (selected == -1) return false;
        std::swap(augmented[rank], augmented[selected]);
        const mp64 inversePivot = powerMp(augmented[rank][column], MP_MOD - 2);
        for (int current = column; current < 2 * size; ++current) {
            augmented[rank][current] = multiplyMp(augmented[rank][current], inversePivot);
        }
        for (int row = 0; row < size; ++row) {
            if (row == rank || augmented[row][column] == 0) continue;
            const mp64 factor = augmented[row][column];
            for (int current = column; current < 2 * size; ++current) {
                augmented[row][current] = normalizeMp(
                    augmented[row][current] - multiplyMp(factor, augmented[rank][current])
                );
            }
        }
        ++rank;
    }
    inverse.assign(size, std::vector<mp64>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            inverse[row][column] = augmented[row][size + column];
        }
    }
    return true;
}

std::vector<mp64> multiplyPolynomialByLinearMp(
    const std::vector<mp64>& polynomial,
    mp64 constant,
    mp64 linear
) {
    std::vector<mp64> result(polynomial.size() + 1, 0);
    for (std::size_t degree = 0; degree < polynomial.size(); ++degree) {
        result[degree] = (result[degree]
            + multiplyMp(polynomial[degree], constant)) % MP_MOD;
        result[degree + 1] = (result[degree + 1]
            + multiplyMp(polynomial[degree], linear)) % MP_MOD;
    }
    return result;
}

std::vector<mp64> interpolateConsecutiveMp(const std::vector<mp64>& values) {
    const int count = static_cast<int>(values.size());
    std::vector<mp64> coefficients(count, 0);
    for (int point = 0; point < count; ++point) {
        std::vector<mp64> basis{1};
        mp64 denominator = 1;
        for (int other = 0; other < count; ++other) {
            if (other == point) continue;
            basis = multiplyPolynomialByLinearMp(basis, normalizeMp(-other), 1);
            denominator = multiplyMp(denominator, normalizeMp(point - other));
        }
        const mp64 scale = multiplyMp(values[point], powerMp(denominator, MP_MOD - 2));
        for (int degree = 0; degree < count; ++degree) {
            coefficients[degree] = (coefficients[degree]
                + multiplyMp(scale, basis[degree])) % MP_MOD;
        }
    }
    return coefficients;
}

std::vector<mp64> characteristicPolynomialMp(const MpMatrix& matrix, bool detAMinusXI) {
    const int size = static_cast<int>(matrix.size());
    std::vector<mp64> values(size + 1, 0);
    for (int point = 0; point <= size; ++point) {
        MpMatrix shifted(size, std::vector<mp64>(size, 0));
        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                shifted[row][column] = detAMinusXI
                    ? normalizeMp(matrix[row][column])
                    : normalizeMp(-matrix[row][column]);
            }
            shifted[row][row] = detAMinusXI
                ? normalizeMp(shifted[row][row] - point)
                : normalizeMp(shifted[row][row] + point);
        }
        values[point] = determinantMp(shifted);
    }
    return interpolateConsecutiveMp(values);
}

mp64 tracePowerMp(const MpMatrix& matrix, std::uint64_t exponent, bool diagonalOnly) {
    if (diagonalOnly) {
        mp64 answer = 0;
        for (int index = 0; index < static_cast<int>(matrix.size()); ++index) {
            answer = (answer + powerMp(matrix[index][index], exponent)) % MP_MOD;
        }
        return answer;
    }
    const MpMatrix powered = matrixPowerMp(matrix, exponent);
    mp64 answer = 0;
    for (int index = 0; index < static_cast<int>(powered.size()); ++index) {
        answer = (answer + powered[index][index]) % MP_MOD;
    }
    return answer;
}

mp64 dotMp(const std::vector<mp64>& left, const std::vector<mp64>& right) {
    mp64 answer = 0;
    for (int index = 0; index < static_cast<int>(left.size()); ++index) {
        answer = (answer + multiplyMp(left[index], right[index])) % MP_MOD;
    }
    return answer;
}

void addRankOneMp(MpMatrix& matrix, const std::vector<mp64>& left, const std::vector<mp64>& right) {
    const int size = static_cast<int>(matrix.size());
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            matrix[row][column] = (matrix[row][column]
                + multiplyMp(left[row], right[column])) % MP_MOD;
        }
    }
}

mp64 determinantLemmaMp(
    const MpMatrix& matrix,
    const std::vector<mp64>& left,
    const std::vector<mp64>& right,
    bool transposedDot
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const std::vector<mp64> transformed = transposedDot
        ? multiplyMatrixVectorMp(inverse, right)
        : multiplyMatrixVectorMp(inverse, left);
    const mp64 dot = transposedDot ? dotMp(left, transformed) : dotMp(right, transformed);
    return multiplyMp(determinantMp(matrix), normalizeMp(1 + dot));
}
