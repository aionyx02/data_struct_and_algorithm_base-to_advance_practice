#include "generator_registry.hpp"

#include "generator_support.hpp"

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

using ll = long long;
using Matrix = std::vector<std::vector<ll>>;
constexpr ll kMod = 1'000'000'007;

ll normalize(ll value) {
    value %= kMod;
    if (value < 0) value += kMod;
    return value;
}

ll multiplyMod(ll left, ll right) {
    return static_cast<ll>(
        static_cast<__int128_t>(left) * right % kMod
    );
}

ll powerMod(ll base, ll exponent) {
    ll result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyMod(result, base);
        base = multiplyMod(base, base);
        exponent >>= 1;
    }
    return result;
}

ll inverseMod(ll value) {
    return powerMod(normalize(value), kMod - 2);
}

std::vector<ll> multiplyMatrixVector(const Matrix& matrix, const std::vector<ll>& vector) {
    const int size = static_cast<int>(matrix.size());
    std::vector<ll> result(size, 0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            result[row] = (result[row]
                + multiplyMod(matrix[row][column], vector[column])) % kMod;
        }
    }
    return result;
}

Matrix multiplyMatrices(const Matrix& left, const Matrix& right) {
    const int size = static_cast<int>(left.size());
    Matrix result(size, std::vector<ll>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int middle = 0; middle < size; ++middle) {
            if (left[row][middle] == 0) continue;
            for (int column = 0; column < size; ++column) {
                result[row][column] = (result[row][column]
                    + multiplyMod(left[row][middle], right[middle][column])) % kMod;
            }
        }
    }
    return result;
}

std::vector<ll> matrixPowerVector(Matrix base, std::uint64_t exponent, std::vector<ll> vector) {
    const int size = static_cast<int>(base.size());
    Matrix result(size, std::vector<ll>(size, 0));
    for (int index = 0; index < size; ++index) result[index][index] = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyMatrices(result, base);
        base = multiplyMatrices(base, base);
        exponent >>= 1;
    }
    return multiplyMatrixVector(result, vector);
}

void appendVector(std::ostringstream& out, const std::vector<ll>& values) {
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) out << ' ';
        out << normalize(values[index]);
    }
    out << '\n';
}

struct LinearSystemAnswer {
    enum class Kind { kUnique, kNoSolution, kInfinite };
    Kind kind = Kind::kNoSolution;
    int rank = 0;
    std::vector<ll> solution;
};

LinearSystemAnswer solveLinearSystem(const Matrix& coefficients, const std::vector<ll>& rhs) {
    const int equationCount = static_cast<int>(coefficients.size());
    const int variableCount = static_cast<int>(coefficients.front().size());
    Matrix augmented = coefficients;
    for (int row = 0; row < equationCount; ++row) {
        augmented[row].push_back(normalize(rhs[row]));
        for (ll& value : augmented[row]) value = normalize(value);
    }

    std::vector<int> where(variableCount, -1);
    int rank = 0;
    for (int column = 0; column < variableCount; ++column) {
        int selected = -1;
        for (int row = rank; row < equationCount; ++row) {
            if (augmented[row][column] != 0) {
                selected = row;
                break;
            }
        }
        if (selected == -1) continue;
        std::swap(augmented[rank], augmented[selected]);
        const ll inverse = inverseMod(augmented[rank][column]);
        for (int current = column; current <= variableCount; ++current) {
            augmented[rank][current] = multiplyMod(augmented[rank][current], inverse);
        }
        for (int row = 0; row < equationCount; ++row) {
            if (row == rank || augmented[row][column] == 0) continue;
            const ll factor = augmented[row][column];
            for (int current = column; current <= variableCount; ++current) {
                augmented[row][current] = normalize(
                    augmented[row][current] - multiplyMod(factor, augmented[rank][current])
                );
            }
        }
        where[column] = rank;
        ++rank;
    }

    for (int row = 0; row < equationCount; ++row) {
        bool allZero = true;
        for (int column = 0; column < variableCount; ++column) {
            if (augmented[row][column] != 0) {
                allZero = false;
                break;
            }
        }
        if (allZero && augmented[row][variableCount] != 0) {
            return {LinearSystemAnswer::Kind::kNoSolution, rank, {}};
        }
    }

    if (std::any_of(where.begin(), where.end(), [](int row) { return row == -1; })) {
        return {LinearSystemAnswer::Kind::kInfinite, rank, {}};
    }

    std::vector<ll> solution(variableCount, 0);
    for (int column = 0; column < variableCount; ++column) {
        solution[column] = augmented[where[column]][variableCount];
    }
    return {LinearSystemAnswer::Kind::kUnique, rank, solution};
}

void appendLinearSystemAnswer(
    std::ostringstream& out,
    const Matrix& coefficients,
    const std::vector<ll>& rhs
) {
    const LinearSystemAnswer answer = solveLinearSystem(coefficients, rhs);
    if (answer.kind == LinearSystemAnswer::Kind::kNoSolution) {
        out << "NO\n";
        return;
    }
    if (answer.kind == LinearSystemAnswer::Kind::kInfinite) {
        out << "INF " << answer.rank << '\n';
        return;
    }
    out << "UNIQUE";
    for (ll value : answer.solution) out << ' ' << value;
    out << '\n';
}

Matrix identityMatrix(int size) {
    Matrix matrix(size, std::vector<ll>(size, 0));
    for (int index = 0; index < size; ++index) matrix[index][index] = 1;
    return matrix;
}

Matrix randomInvertibleMatrix(std::mt19937_64& random, int size) {
    Matrix matrix = identityMatrix(size);
    const int operations = size * 8 + 8;
    for (int step = 0; step < operations; ++step) {
        const int kind = randomInt(random, 0, 2);
        const int first = randomInt(random, 0, size - 1);
        int second = randomInt(random, 0, size - 1);
        if (size > 1) {
            while (second == first) second = randomInt(random, 0, size - 1);
        }
        if (kind == 0) {
            std::swap(matrix[first], matrix[second]);
        } else if (kind == 1) {
            const ll scale = randomInt(random, 1, 12);
            for (ll& value : matrix[first]) value = multiplyMod(value, scale);
        } else {
            const ll factor = randomInt(random, 1, 12);
            for (int column = 0; column < size; ++column) {
                matrix[first][column] = (matrix[first][column]
                    + multiplyMod(factor, matrix[second][column])) % kMod;
            }
        }
    }
    return matrix;
}

ll determinantMod(Matrix matrix) {
    const int size = static_cast<int>(matrix.size());
    ll answer = 1;
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
            answer = normalize(-answer);
        }
        answer = multiplyMod(answer, matrix[column][column]);
        const ll inverse = inverseMod(matrix[column][column]);
        for (int row = column + 1; row < size; ++row) {
            if (matrix[row][column] == 0) continue;
            const ll factor = multiplyMod(matrix[row][column], inverse);
            for (int current = column; current < size; ++current) {
                matrix[row][current] = normalize(
                    matrix[row][current] - multiplyMod(factor, matrix[column][current])
                );
            }
        }
    }
    return answer;
}

bool invertMatrix(Matrix matrix, Matrix& inverse) {
    const int size = static_cast<int>(matrix.size());
    Matrix augmented(size, std::vector<ll>(2 * size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            augmented[row][column] = normalize(matrix[row][column]);
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
        const ll inversePivot = inverseMod(augmented[rank][column]);
        for (int current = column; current < 2 * size; ++current) {
            augmented[rank][current] = multiplyMod(augmented[rank][current], inversePivot);
        }
        for (int row = 0; row < size; ++row) {
            if (row == rank || augmented[row][column] == 0) continue;
            const ll factor = augmented[row][column];
            for (int current = column; current < 2 * size; ++current) {
                augmented[row][current] = normalize(
                    augmented[row][current] - multiplyMod(factor, augmented[rank][current])
                );
            }
        }
        ++rank;
    }

    inverse.assign(size, std::vector<ll>(size, 0));
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column) {
            inverse[row][column] = augmented[row][size + column];
        }
    }
    return true;
}

void appendMatrix(std::ostringstream& out, const Matrix& matrix) {
    for (const auto& row : matrix) appendVector(out, row);
}

GeneratedCase generateGaussianElimination(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M28-gaussian-elimination-mod operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int kind = query % 3;
        Matrix matrix;
        std::vector<ll> rhs;
        if (kind == 0) {
            const int size = randomInt(random, 1, 5);
            matrix = randomInvertibleMatrix(random, size);
            std::vector<ll> solution(size);
            for (ll& value : solution) value = randomInt(random, 0, 40);
            rhs = multiplyMatrixVector(matrix, solution);
        } else if (kind == 1) {
            const int variables = randomInt(random, 2, 5);
            matrix.assign(variables + 1, std::vector<ll>(variables, 0));
            rhs.assign(variables + 1, 0);
            for (int index = 0; index < variables; ++index) {
                matrix[index][index] = 1;
                rhs[index] = randomInt(random, 0, 40);
            }
            matrix[variables][0] = 1;
            rhs[variables] = (rhs[0] + randomInt(random, 1, 40)) % kMod;
        } else {
            const int variables = randomInt(random, 2, 6);
            const int rank = randomInt(random, 1, variables - 1);
            const int equations = rank + randomInt(random, 0, 2);
            matrix.assign(equations, std::vector<ll>(variables, 0));
            rhs.assign(equations, 0);
            for (int row = 0; row < equations; ++row) {
                const int pivot = row % rank;
                matrix[row][pivot] = 1;
                rhs[row] = randomInt(random, 0, 40);
            }
        }
        in << "system " << matrix.size() << ' ' << matrix.front().size() << '\n';
        for (std::size_t row = 0; row < matrix.size(); ++row) {
            for (ll value : matrix[row]) in << value << ' ';
            in << rhs[row] << '\n';
        }
        appendLinearSystemAnswer(out, matrix, rhs);
    }
    return {in.str(), out.str()};
}

GeneratedCase generateMatrixPowerVector(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M29-matrix-power-vector operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 5);
        std::uint64_t exponent = static_cast<std::uint64_t>(randomInt(random, 0, 40));
        if (query % 5 == 0) exponent = 0;
        if (query % 7 == 0) exponent = 1'000'000'000'000ULL + static_cast<std::uint64_t>(query);
        Matrix matrix(size, std::vector<ll>(size, 0));
        std::vector<ll> vector(size, 0);
        for (auto& row : matrix) {
            for (ll& value : row) value = randomInt(random, 0, 8);
        }
        for (ll& value : vector) value = randomInt(random, 0, 8);
        in << "apply " << size << ' ' << exponent << '\n';
        for (const auto& row : matrix) appendVector(in, row);
        appendVector(in, vector);
        appendVector(out, matrixPowerVector(matrix, exponent, vector));
    }
    return {in.str(), out.str()};
}

GeneratedCase generateModularDeterminant(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M31-modular-determinant operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 6);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 3 == 1 && size > 1) {
            matrix[size - 1] = matrix[0];
        } else if (query % 3 == 2 && size > 1) {
            matrix[0][0] = 0;
            matrix[1][0] = randomInt(random, 1, 10);
        }
        in << "det " << size << '\n';
        appendMatrix(in, matrix);
        out << determinantMod(matrix) << '\n';
    }
    return {in.str(), out.str()};
}

GeneratedCase generateMatrixInverse(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M32-matrix-inverse-mod operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 5);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 3 == 1 && size > 1) {
            matrix[size - 1] = matrix[0];
        } else if (query % 3 == 2 && size > 1) {
            matrix[0][0] = 0;
            matrix[1][0] = randomInt(random, 1, 10);
        }
        in << "inverse " << size << '\n';
        appendMatrix(in, matrix);
        Matrix inverse;
        if (!invertMatrix(matrix, inverse)) {
            out << "SINGULAR\n";
        } else {
            appendMatrix(out, inverse);
        }
    }
    return {in.str(), out.str()};
}

struct XorBasisAnswer {
    int rank = 0;
    ll maximum = 0;
    bool representable = false;
};

XorBasisAnswer solveXorBasis(const std::vector<ll>& values, ll target) {
    std::vector<ll> basis(61, 0);
    for (ll value : values) {
        ll current = value;
        for (int bit = 60; bit >= 0; --bit) {
            if (((current >> bit) & 1LL) == 0) continue;
            if (basis[bit] == 0) {
                basis[bit] = current;
                break;
            }
            current ^= basis[bit];
        }
    }
    int rank = 0;
    for (ll value : basis) {
        if (value != 0) ++rank;
    }
    ll maximum = 0;
    for (int bit = 60; bit >= 0; --bit) {
        maximum = std::max(maximum, maximum ^ basis[bit]);
    }
    ll reducedTarget = target;
    for (int bit = 60; bit >= 0; --bit) {
        if (((reducedTarget >> bit) & 1LL) != 0) reducedTarget ^= basis[bit];
    }
    return {rank, maximum, reducedTarget == 0};
}

void appendRawXorVector(std::ostringstream& out, const std::vector<ll>& values) {
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) out << ' ';
        out << values[index];
    }
    out << '\n';
}

GeneratedCase generateXorLinearBasis(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M33-xor-linear-basis operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int count = randomInt(random, 1, 16);
        std::vector<ll> values;
        values.reserve(count);
        for (int index = 0; index < count; ++index) {
            const ll high = static_cast<ll>(randomInt(random, 0, 1 << 20));
            const ll low = static_cast<ll>(randomInt(random, 0, 1 << 20));
            values.push_back((high << 30) ^ low);
        }
        if (query % 3 == 0 && count >= 3) {
            values[2] = values[0] ^ values[1];
        }
        ll target = 0;
        if (query % 4 == 0) {
            target = 0;
        } else if (query % 2 == 0) {
            for (ll value : values) {
                if (randomInt(random, 0, 1) == 1) target ^= value;
            }
        } else {
            target = (static_cast<ll>(randomInt(random, 0, 1 << 20)) << 30)
                ^ static_cast<ll>(randomInt(random, 0, 1 << 20));
        }
        in << "basis " << values.size() << ' ' << target << '\n';
        appendRawXorVector(in, values);
        const XorBasisAnswer answer = solveXorBasis(values, target);
        out << answer.rank << ' ' << answer.maximum << ' '
            << (answer.representable ? "YES" : "NO") << '\n';
    }
    return {in.str(), out.str()};
}

std::vector<ll> combineRecurrencePolynomials(
    const std::vector<ll>& left,
    const std::vector<ll>& right,
    const std::vector<ll>& coefficients
) {
    const int order = static_cast<int>(coefficients.size());
    std::vector<ll> product(2 * order - 1, 0);
    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            product[i + j] = (product[i + j]
                + multiplyMod(left[i], right[j])) % kMod;
        }
    }
    for (int degree = 2 * order - 2; degree >= order; --degree) {
        for (int offset = 1; offset <= order; ++offset) {
            product[degree - offset] = (product[degree - offset]
                + multiplyMod(product[degree], coefficients[offset - 1])) % kMod;
        }
    }
    product.resize(order);
    return product;
}

ll linearRecurrenceTerm(
    const std::vector<ll>& initial,
    const std::vector<ll>& coefficients,
    std::uint64_t index
) {
    const int order = static_cast<int>(coefficients.size());
    if (index < initial.size()) return initial[static_cast<std::size_t>(index)];
    if (order == 0) return 0;
    std::vector<ll> result(order, 0);
    std::vector<ll> base(order, 0);
    result[0] = 1;
    if (order == 1) base[0] = coefficients[0];
    else base[1] = 1;
    while (index > 0) {
        if (index & 1) {
            result = combineRecurrencePolynomials(result, base, coefficients);
        }
        base = combineRecurrencePolynomials(base, base, coefficients);
        index >>= 1;
    }
    ll answer = 0;
    for (int indexInBasis = 0; indexInBasis < order; ++indexInBasis) {
        answer = (answer
            + multiplyMod(result[indexInBasis], initial[indexInBasis])) % kMod;
    }
    return answer;
}

GeneratedCase generateBerlekampMassey(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M30-berlekamp-massey operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int order = randomInt(random, 1, 8);
        const int given = 2 * order + randomInt(random, 0, 4);
        std::uint64_t target = static_cast<std::uint64_t>(randomInt(random, 0, 400));
        if (query % 4 == 0) target = static_cast<std::uint64_t>(randomInt(random, 0, given - 1));
        if (query % 5 == 0) target = 1'000'000'000'000ULL + static_cast<std::uint64_t>(query);
        std::vector<ll> coefficients(order);
        std::vector<ll> sequence(given, 0);
        bool anyCoefficient = false;
        for (ll& value : coefficients) {
            value = randomInt(random, 0, 12);
            anyCoefficient = anyCoefficient || value != 0;
        }
        if (!anyCoefficient) coefficients.back() = 1;
        for (int index = 0; index < order; ++index) {
            sequence[index] = randomInt(random, 0, 20);
        }
        for (int index = order; index < given; ++index) {
            ll value = 0;
            for (int offset = 1; offset <= order; ++offset) {
                value = (value
                    + multiplyMod(coefficients[offset - 1], sequence[index - offset])) % kMod;
            }
            sequence[index] = value;
        }
        in << "predict " << given << ' ' << target;
        for (ll value : sequence) in << ' ' << value;
        in << '\n';
        out << linearRecurrenceTerm(
            std::vector<ll>(sequence.begin(), sequence.begin() + order),
            coefficients,
            target
        ) << '\n';
    }
    return {in.str(), out.str()};
}

}  // namespace

void registerLinearAlgebraGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("M28-gaussian-elimination-mod", generateGaussianElimination);
    registry.emplace("M29-matrix-power-vector", generateMatrixPowerVector);
    registry.emplace("M30-berlekamp-massey", generateBerlekampMassey);
    registry.emplace("M31-modular-determinant", generateModularDeterminant);
    registry.emplace("M32-matrix-inverse-mod", generateMatrixInverse);
    registry.emplace("M33-xor-linear-basis", generateXorLinearBasis);
}

}  // namespace judge::generators
