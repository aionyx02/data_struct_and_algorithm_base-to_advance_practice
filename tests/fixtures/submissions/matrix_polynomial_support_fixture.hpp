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

MpMatrix multiplySquareByRectMp(const MpMatrix& left, const MpMatrix& right) {
    const int rowCount = static_cast<int>(left.size());
    const int columnCount = static_cast<int>(right.front().size());
    MpMatrix result(rowCount, std::vector<mp64>(columnCount, 0));
    for (int row = 0; row < rowCount; ++row) {
        for (int middle = 0; middle < rowCount; ++middle) {
            if (left[row][middle] == 0) continue;
            for (int column = 0; column < columnCount; ++column) {
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

std::vector<mp64> multiplyRectVectorMp(
    const MpMatrix& matrix,
    const std::vector<mp64>& vector
) {
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    std::vector<mp64> result(rowCount, 0);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
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

int rankMatrixMp(MpMatrix matrix) {
    if (matrix.empty()) return 0;
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    for (auto& row : matrix) {
        for (mp64& value : row) value = normalizeMp(value);
    }
    int rank = 0;
    for (int column = 0; column < columnCount && rank < rowCount; ++column) {
        int selected = -1;
        for (int row = rank; row < rowCount; ++row) {
            if (matrix[row][column] != 0) {
                selected = row;
                break;
            }
        }
        if (selected == -1) continue;
        std::swap(matrix[rank], matrix[selected]);
        const mp64 inversePivot = powerMp(matrix[rank][column], MP_MOD - 2);
        for (int current = column; current < columnCount; ++current) {
            matrix[rank][current] = multiplyMp(matrix[rank][current], inversePivot);
        }
        for (int row = 0; row < rowCount; ++row) {
            if (row == rank || matrix[row][column] == 0) continue;
            const mp64 factor = matrix[row][column];
            for (int current = column; current < columnCount; ++current) {
                matrix[row][current] = normalizeMp(
                    matrix[row][current] - multiplyMp(factor, matrix[rank][current])
                );
            }
        }
        ++rank;
    }
    return rank;
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

std::vector<mp64> solveShermanMorrisonMp(
    const MpMatrix& matrix,
    const std::vector<mp64>& left,
    const std::vector<mp64>& right,
    const std::vector<mp64>& rhs,
    bool transposedUpdate,
    bool omitDenominator
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const std::vector<mp64> base = multiplyMatrixVectorMp(inverse, rhs);
    const std::vector<mp64> direction = transposedUpdate
        ? multiplyMatrixVectorMp(inverse, right)
        : multiplyMatrixVectorMp(inverse, left);
    const std::vector<mp64>& probe = transposedUpdate ? left : right;
    const mp64 numerator = dotMp(probe, base);
    const mp64 denominator = normalizeMp(1 + dotMp(probe, direction));
    const mp64 scale = omitDenominator
        ? numerator
        : multiplyMp(numerator, powerMp(denominator, MP_MOD - 2));
    std::vector<mp64> answer = base;
    for (int index = 0; index < static_cast<int>(answer.size()); ++index) {
        answer[index] = normalizeMp(answer[index] - multiplyMp(direction[index], scale));
    }
    return answer;
}

std::vector<mp64> multiplyTransposedRectVectorMp(
    const MpMatrix& matrix,
    const std::vector<mp64>& vector
) {
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    std::vector<mp64> result(columnCount, 0);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            result[column] = (result[column]
                + multiplyMp(matrix[row][column], vector[row])) % MP_MOD;
        }
    }
    return result;
}

std::vector<mp64> solveWoodburyMp(
    const MpMatrix& matrix,
    const MpMatrix& left,
    const MpMatrix& right,
    const std::vector<mp64>& rhs,
    bool transposedCore,
    bool diagonalCore
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const MpMatrix directions = transposedCore
        ? multiplySquareByRectMp(inverse, right)
        : multiplySquareByRectMp(inverse, left);
    const MpMatrix& probe = transposedCore ? left : right;
    const int size = static_cast<int>(matrix.size());
    const int rank = static_cast<int>(left.front().size());
    MpMatrix core(rank, std::vector<mp64>(rank, 0));
    for (int row = 0; row < rank; ++row) {
        for (int column = 0; column < rank; ++column) {
            mp64 value = row == column ? 1 : 0;
            for (int index = 0; index < size; ++index) {
                value = (value
                    + multiplyMp(probe[index][row], directions[index][column])) % MP_MOD;
            }
            core[row][column] = value;
        }
    }
    const std::vector<mp64> base = multiplyMatrixVectorMp(inverse, rhs);
    const std::vector<mp64> projected = multiplyTransposedRectVectorMp(probe, base);
    std::vector<mp64> weights(rank, 0);
    if (diagonalCore) {
        for (int index = 0; index < rank; ++index) {
            weights[index] = multiplyMp(projected[index], powerMp(core[index][index], MP_MOD - 2));
        }
    } else {
        MpMatrix coreInverse;
        invertMatrixMp(core, coreInverse);
        weights = multiplyMatrixVectorMp(coreInverse, projected);
    }
    std::vector<mp64> answer = base;
    for (int row = 0; row < size; ++row) {
        mp64 correction = 0;
        for (int column = 0; column < rank; ++column) {
            correction = (correction
                + multiplyMp(directions[row][column], weights[column])) % MP_MOD;
        }
        answer[row] = normalizeMp(answer[row] - correction);
    }
    return answer;
}

mp64 determinantWoodburyMp(
    const MpMatrix& matrix,
    const MpMatrix& left,
    const MpMatrix& right,
    bool transposedCore,
    bool diagonalOnly
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const MpMatrix transformed = transposedCore
        ? multiplySquareByRectMp(inverse, right)
        : multiplySquareByRectMp(inverse, left);
    const MpMatrix& probe = transposedCore ? left : right;
    const int size = static_cast<int>(matrix.size());
    const int rank = static_cast<int>(left.front().size());
    MpMatrix core(rank, std::vector<mp64>(rank, 0));
    for (int row = 0; row < rank; ++row) {
        for (int column = 0; column < rank; ++column) {
            mp64 value = row == column ? 1 : 0;
            for (int index = 0; index < size; ++index) {
                value = (value
                    + multiplyMp(probe[index][row], transformed[index][column])) % MP_MOD;
            }
            core[row][column] = value;
        }
    }
    if (diagonalOnly) {
        mp64 answer = determinantMp(matrix);
        for (int index = 0; index < rank; ++index) {
            answer = multiplyMp(answer, core[index][index]);
        }
        return answer;
    }
    return multiplyMp(determinantMp(matrix), determinantMp(core));
}

mp64 determinantSchurComplementMp(
    const MpMatrix& matrix,
    const MpMatrix& upperRight,
    const MpMatrix& lowerLeft,
    const MpMatrix& lowerRight,
    bool plusSign,
    bool transposedProduct
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    MpMatrix transformed;
    if (transposedProduct) {
        MpMatrix lowerLeftTranspose(size, std::vector<mp64>(blockSize, 0));
        for (int row = 0; row < blockSize; ++row) {
            for (int column = 0; column < size; ++column) {
                lowerLeftTranspose[column][row] = lowerLeft[row][column];
            }
        }
        transformed = multiplySquareByRectMp(inverse, lowerLeftTranspose);
    } else {
        transformed = multiplySquareByRectMp(inverse, upperRight);
    }
    MpMatrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            mp64 correction = 0;
            for (int index = 0; index < size; ++index) {
                const mp64 probe = transposedProduct
                    ? upperRight[index][row]
                    : lowerLeft[row][index];
                correction = (correction
                    + multiplyMp(probe, transformed[index][column])) % MP_MOD;
            }
            complement[row][column] = plusSign
                ? normalizeMp(complement[row][column] + correction)
                : normalizeMp(complement[row][column] - correction);
        }
    }
    return multiplyMp(determinantMp(matrix), determinantMp(complement));
}

int rankSchurComplementMp(
    const MpMatrix& matrix,
    const MpMatrix& upperRight,
    const MpMatrix& lowerLeft,
    const MpMatrix& lowerRight,
    bool plusSign,
    bool transposedProduct
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    MpMatrix transformed;
    if (transposedProduct) {
        MpMatrix lowerLeftTranspose(size, std::vector<mp64>(blockSize, 0));
        for (int row = 0; row < blockSize; ++row) {
            for (int column = 0; column < size; ++column) {
                lowerLeftTranspose[column][row] = lowerLeft[row][column];
            }
        }
        transformed = multiplySquareByRectMp(inverse, lowerLeftTranspose);
    } else {
        transformed = multiplySquareByRectMp(inverse, upperRight);
    }
    MpMatrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            mp64 correction = 0;
            for (int index = 0; index < size; ++index) {
                const mp64 probe = transposedProduct
                    ? upperRight[index][row]
                    : lowerLeft[row][index];
                correction = (correction
                    + multiplyMp(probe, transformed[index][column])) % MP_MOD;
            }
            complement[row][column] = plusSign
                ? normalizeMp(complement[row][column] + correction)
                : normalizeMp(complement[row][column] - correction);
        }
    }
    return size + rankMatrixMp(complement);
}

std::vector<mp64> solveSchurComplementMp(
    const MpMatrix& matrix,
    const MpMatrix& upperRight,
    const MpMatrix& lowerLeft,
    const MpMatrix& lowerRight,
    const std::vector<mp64>& upperRhs,
    const std::vector<mp64>& lowerRhs,
    bool plusSign,
    bool transposedProduct
) {
    MpMatrix inverse;
    invertMatrixMp(matrix, inverse);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    const std::vector<mp64> baseUpper = multiplyMatrixVectorMp(inverse, upperRhs);
    MpMatrix transformed;
    std::vector<mp64> reducedRhs(blockSize, 0);
    if (transposedProduct) {
        MpMatrix lowerLeftTranspose(size, std::vector<mp64>(blockSize, 0));
        for (int row = 0; row < blockSize; ++row) {
            for (int column = 0; column < size; ++column) {
                lowerLeftTranspose[column][row] = lowerLeft[row][column];
            }
        }
        transformed = multiplySquareByRectMp(inverse, lowerLeftTranspose);
        const std::vector<mp64> transposedProjection =
            multiplyTransposedRectVectorMp(upperRight, baseUpper);
        for (int row = 0; row < blockSize; ++row) {
            reducedRhs[row] = plusSign
                ? normalizeMp(lowerRhs[row] + transposedProjection[row])
                : normalizeMp(lowerRhs[row] - transposedProjection[row]);
        }
    } else {
        transformed = multiplySquareByRectMp(inverse, upperRight);
        const std::vector<mp64> projection = multiplyRectVectorMp(lowerLeft, baseUpper);
        for (int row = 0; row < blockSize; ++row) {
            reducedRhs[row] = plusSign
                ? normalizeMp(lowerRhs[row] + projection[row])
                : normalizeMp(lowerRhs[row] - projection[row]);
        }
    }
    MpMatrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            mp64 correction = 0;
            for (int index = 0; index < size; ++index) {
                const mp64 probe = transposedProduct
                    ? upperRight[index][row]
                    : lowerLeft[row][index];
                correction = (correction
                    + multiplyMp(probe, transformed[index][column])) % MP_MOD;
            }
            complement[row][column] = plusSign
                ? normalizeMp(complement[row][column] + correction)
                : normalizeMp(complement[row][column] - correction);
        }
    }
    MpMatrix complementInverse;
    if (!invertMatrixMp(complement, complementInverse)) {
        return std::vector<mp64>(size + blockSize, 0);
    }
    const std::vector<mp64> lowerSolution =
        multiplyMatrixVectorMp(complementInverse, reducedRhs);
    const std::vector<mp64> upperCorrection =
        multiplyMatrixVectorMp(inverse, multiplyRectVectorMp(upperRight, lowerSolution));
    std::vector<mp64> answer = baseUpper;
    for (int index = 0; index < size; ++index) {
        answer[index] = normalizeMp(answer[index] - upperCorrection[index]);
    }
    answer.insert(answer.end(), lowerSolution.begin(), lowerSolution.end());
    return answer;
}

MpMatrix inverseSchurComplementMp(
    const MpMatrix& matrix,
    const MpMatrix& upperRight,
    const MpMatrix& lowerLeft,
    const MpMatrix& lowerRight,
    bool plusSign,
    bool transposedProduct
) {
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    const int totalSize = size + blockSize;
    MpMatrix inverse(totalSize, std::vector<mp64>(totalSize, 0));
    for (int column = 0; column < totalSize; ++column) {
        std::vector<mp64> upperRhs(size, 0);
        std::vector<mp64> lowerRhs(blockSize, 0);
        if (column < size) {
            upperRhs[column] = 1;
        } else {
            lowerRhs[column - size] = 1;
        }
        const std::vector<mp64> solution = solveSchurComplementMp(
            matrix,
            upperRight,
            lowerLeft,
            lowerRight,
            upperRhs,
            lowerRhs,
            plusSign,
            transposedProduct
        );
        for (int row = 0; row < totalSize; ++row) {
            inverse[row][column] = solution[row];
        }
    }
    return inverse;
}
