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

Matrix identityMatrix(int size);

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

std::vector<ll> multiplyRectVector(const Matrix& matrix, const std::vector<ll>& vector) {
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    std::vector<ll> result(rowCount, 0);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
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

Matrix multiplySquareByRect(const Matrix& left, const Matrix& right) {
    const int rowCount = static_cast<int>(left.size());
    const int columnCount = static_cast<int>(right.front().size());
    Matrix result(rowCount, std::vector<ll>(columnCount, 0));
    for (int row = 0; row < rowCount; ++row) {
        for (int middle = 0; middle < rowCount; ++middle) {
            if (left[row][middle] == 0) continue;
            for (int column = 0; column < columnCount; ++column) {
                result[row][column] = (result[row][column]
                    + multiplyMod(left[row][middle], right[middle][column])) % kMod;
            }
        }
    }
    return result;
}

Matrix matrixPower(Matrix base, std::uint64_t exponent) {
    const int size = static_cast<int>(base.size());
    Matrix result = identityMatrix(size);
    while (exponent > 0) {
        if (exponent & 1) result = multiplyMatrices(result, base);
        base = multiplyMatrices(base, base);
        exponent >>= 1;
    }
    return result;
}

std::vector<ll> matrixPowerVector(Matrix base, std::uint64_t exponent, std::vector<ll> vector) {
    return multiplyMatrixVector(matrixPower(base, exponent), vector);
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

int matrixRank(Matrix matrix) {
    if (matrix.empty()) return 0;
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    for (auto& row : matrix) {
        for (ll& value : row) value = normalize(value);
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
        const ll inversePivot = inverseMod(matrix[rank][column]);
        for (int current = column; current < columnCount; ++current) {
            matrix[rank][current] = multiplyMod(matrix[rank][current], inversePivot);
        }
        for (int row = 0; row < rowCount; ++row) {
            if (row == rank || matrix[row][column] == 0) continue;
            const ll factor = matrix[row][column];
            for (int current = column; current < columnCount; ++current) {
                matrix[row][current] = normalize(
                    matrix[row][current] - multiplyMod(factor, matrix[rank][current])
                );
            }
        }
        ++rank;
    }
    return rank;
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

struct WeightedEdge {
    int from = 0;
    int to = 0;
    ll weight = 0;
};

Matrix cofactorWithoutRoot(const Matrix& laplacian, int root) {
    const int size = static_cast<int>(laplacian.size());
    Matrix cofactor;
    cofactor.reserve(std::max(0, size - 1));
    for (int row = 0; row < size; ++row) {
        if (row == root) continue;
        std::vector<ll> current;
        current.reserve(std::max(0, size - 1));
        for (int column = 0; column < size; ++column) {
            if (column == root) continue;
            current.push_back(laplacian[row][column]);
        }
        cofactor.push_back(current);
    }
    return cofactor;
}

ll undirectedSpanningTreeCount(int vertexCount, const std::vector<WeightedEdge>& edges) {
    if (vertexCount == 1) return 1;
    Matrix laplacian(vertexCount, std::vector<ll>(vertexCount, 0));
    for (const WeightedEdge& edge : edges) {
        const int from = edge.from;
        const int to = edge.to;
        const ll weight = normalize(edge.weight);
        laplacian[from][from] = (laplacian[from][from] + weight) % kMod;
        laplacian[to][to] = (laplacian[to][to] + weight) % kMod;
        laplacian[from][to] = normalize(laplacian[from][to] - weight);
        laplacian[to][from] = normalize(laplacian[to][from] - weight);
    }
    return determinantMod(cofactorWithoutRoot(laplacian, vertexCount - 1));
}

ll directedArborescenceCount(
    int vertexCount,
    int root,
    const std::vector<WeightedEdge>& edges
) {
    if (vertexCount == 1) return 1;
    Matrix laplacian(vertexCount, std::vector<ll>(vertexCount, 0));
    for (const WeightedEdge& edge : edges) {
        const int from = edge.from;
        const int to = edge.to;
        const ll weight = normalize(edge.weight);
        laplacian[from][from] = (laplacian[from][from] + weight) % kMod;
        laplacian[from][to] = normalize(laplacian[from][to] - weight);
    }
    return determinantMod(cofactorWithoutRoot(laplacian, root));
}

GeneratedCase generateUndirectedMatrixTree(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M34-undirected-matrix-tree operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int vertexCount = randomInt(random, 1, 7);
        std::vector<WeightedEdge> edges;
        if (vertexCount > 1 && query % 4 != 1) {
            for (int vertex = 1; vertex < vertexCount; ++vertex) {
                edges.push_back({vertex - 1, vertex, randomInt(random, 1, 9)});
            }
        }
        const int extraEdges = randomInt(random, 0, vertexCount + 2);
        for (int edge = 0; edge < extraEdges && vertexCount > 1; ++edge) {
            int from = randomInt(random, 0, vertexCount - 1);
            int to = randomInt(random, 0, vertexCount - 1);
            while (to == from) to = randomInt(random, 0, vertexCount - 1);
            edges.push_back({from, to, randomInt(random, 1, 9)});
        }
        in << "tree " << vertexCount << ' ' << edges.size() << '\n';
        for (const WeightedEdge& edge : edges) {
            in << edge.from + 1 << ' ' << edge.to + 1 << ' ' << edge.weight << '\n';
        }
        out << undirectedSpanningTreeCount(vertexCount, edges) << '\n';
    }
    return {in.str(), out.str()};
}

GeneratedCase generateDirectedArborescence(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M35-directed-arborescence operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int vertexCount = randomInt(random, 1, 7);
        const int root = randomInt(random, 0, vertexCount - 1);
        std::vector<WeightedEdge> edges;
        for (int vertex = 0; vertex < vertexCount; ++vertex) {
            if (vertex == root) continue;
            edges.push_back({vertex, root, randomInt(random, 1, 7)});
            if (vertexCount > 2 && randomInt(random, 0, 1) == 1) {
                int via = randomInt(random, 0, vertexCount - 1);
                while (via == vertex) via = randomInt(random, 0, vertexCount - 1);
                edges.push_back({vertex, via, randomInt(random, 1, 7)});
            }
        }
        if (query % 4 == 1 && vertexCount > 1) {
            edges.clear();
            for (int vertex = 0; vertex < vertexCount; ++vertex) {
                if (vertex != root) edges.push_back({root, vertex, 1});
            }
        }
        in << "arborescence " << vertexCount << ' ' << edges.size()
           << ' ' << root + 1 << '\n';
        for (const WeightedEdge& edge : edges) {
            in << edge.from + 1 << ' ' << edge.to + 1 << ' ' << edge.weight << '\n';
        }
        out << directedArborescenceCount(vertexCount, root, edges) << '\n';
    }
    return {in.str(), out.str()};
}

ll bestTheoremCount(int vertexCount, int root, const std::vector<WeightedEdge>& edges) {
    std::vector<int> inDegree(vertexCount, 0);
    std::vector<int> outDegree(vertexCount, 0);
    for (const WeightedEdge& edge : edges) {
        ++outDegree[edge.from];
        ++inDegree[edge.to];
    }
    if (inDegree != outDegree) return 0;
    const ll treeCount = directedArborescenceCount(vertexCount, root, edges);
    if (treeCount == 0) return 0;
    int maximumDegree = 0;
    for (int degree : outDegree) maximumDegree = std::max(maximumDegree, degree);
    std::vector<ll> factorial(maximumDegree + 1, 1);
    for (int value = 1; value <= maximumDegree; ++value) {
        factorial[value] = multiplyMod(factorial[value - 1], value);
    }
    ll answer = treeCount;
    for (int degree : outDegree) {
        answer = multiplyMod(answer, factorial[std::max(0, degree - 1)]);
    }
    return answer;
}

GeneratedCase generateBestTheoremCount(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M36-best-theorem-count operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int vertexCount = randomInt(random, 1, 7);
        const int root = randomInt(random, 0, vertexCount - 1);
        std::vector<WeightedEdge> edges;
        if (vertexCount > 1) {
            for (int vertex = 0; vertex < vertexCount; ++vertex) {
                edges.push_back({vertex, (vertex + 1) % vertexCount, 1});
            }
            const int extraCycles = randomInt(random, 0, 3);
            for (int cycle = 0; cycle < extraCycles; ++cycle) {
                int a = randomInt(random, 0, vertexCount - 1);
                int b = randomInt(random, 0, vertexCount - 1);
                while (b == a) b = randomInt(random, 0, vertexCount - 1);
                edges.push_back({a, b, 1});
                edges.push_back({b, a, 1});
            }
        }
        if (query % 5 == 1 && vertexCount > 1) {
            edges.push_back({root, (root + 1) % vertexCount, 1});
        }
        if (query % 5 == 2 && vertexCount > 3) {
            edges.clear();
            edges.push_back({0, 1, 1});
            edges.push_back({1, 0, 1});
            edges.push_back({2, 3, 1});
            edges.push_back({3, 2, 1});
        }
        in << "best " << vertexCount << ' ' << edges.size() << ' ' << root + 1 << '\n';
        for (const WeightedEdge& edge : edges) {
            in << edge.from + 1 << ' ' << edge.to + 1 << '\n';
        }
        out << bestTheoremCount(vertexCount, root, edges) << '\n';
    }
    return {in.str(), out.str()};
}

std::vector<ll> multiplyPolynomialsByLinear(
    const std::vector<ll>& polynomial,
    ll constant,
    ll linear
) {
    std::vector<ll> result(polynomial.size() + 1, 0);
    for (std::size_t degree = 0; degree < polynomial.size(); ++degree) {
        result[degree] = (result[degree]
            + multiplyMod(polynomial[degree], constant)) % kMod;
        result[degree + 1] = (result[degree + 1]
            + multiplyMod(polynomial[degree], linear)) % kMod;
    }
    return result;
}

std::vector<ll> interpolateAtConsecutivePoints(const std::vector<ll>& values) {
    const int count = static_cast<int>(values.size());
    std::vector<ll> coefficients(count, 0);
    for (int point = 0; point < count; ++point) {
        std::vector<ll> basis{1};
        ll denominator = 1;
        for (int other = 0; other < count; ++other) {
            if (other == point) continue;
            basis = multiplyPolynomialsByLinear(basis, normalize(-other), 1);
            denominator = multiplyMod(denominator, normalize(point - other));
        }
        const ll scale = multiplyMod(values[point], inverseMod(denominator));
        for (int degree = 0; degree < count; ++degree) {
            coefficients[degree] = (coefficients[degree]
                + multiplyMod(scale, basis[degree])) % kMod;
        }
    }
    return coefficients;
}

std::vector<ll> characteristicPolynomial(const Matrix& matrix) {
    const int size = static_cast<int>(matrix.size());
    std::vector<ll> values(size + 1, 0);
    for (int point = 0; point <= size; ++point) {
        Matrix shifted(size, std::vector<ll>(size, 0));
        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                shifted[row][column] = normalize(-matrix[row][column]);
            }
            shifted[row][row] = normalize(shifted[row][row] + point);
        }
        values[point] = determinantMod(shifted);
    }
    return interpolateAtConsecutivePoints(values);
}

ll traceMatrix(const Matrix& matrix) {
    ll answer = 0;
    for (int index = 0; index < static_cast<int>(matrix.size()); ++index) {
        answer = (answer + matrix[index][index]) % kMod;
    }
    return answer;
}

ll dotVectors(const std::vector<ll>& left, const std::vector<ll>& right) {
    ll answer = 0;
    for (int index = 0; index < static_cast<int>(left.size()); ++index) {
        answer = (answer + multiplyMod(left[index], right[index])) % kMod;
    }
    return answer;
}

ll determinantRankOneUpdate(
    const Matrix& matrix,
    const std::vector<ll>& left,
    const std::vector<ll>& right
) {
    Matrix inverse;
    if (!invertMatrix(matrix, inverse)) return determinantMod(matrix);
    const std::vector<ll> transformed = multiplyMatrixVector(inverse, left);
    const ll dot = dotVectors(right, transformed);
    return multiplyMod(determinantMod(matrix), normalize(1 + dot));
}

std::vector<ll> solveShermanMorrison(
    const Matrix& matrix,
    const std::vector<ll>& left,
    const std::vector<ll>& right,
    const std::vector<ll>& rhs
) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const std::vector<ll> base = multiplyMatrixVector(inverse, rhs);
    const std::vector<ll> direction = multiplyMatrixVector(inverse, left);
    const ll denominator = normalize(1 + dotVectors(right, direction));
    const ll scale = multiplyMod(dotVectors(right, base), inverseMod(denominator));
    std::vector<ll> answer = base;
    for (int index = 0; index < static_cast<int>(answer.size()); ++index) {
        answer[index] = normalize(answer[index] - multiplyMod(direction[index], scale));
    }
    return answer;
}

std::vector<ll> multiplyTransposedRectVector(
    const Matrix& matrix,
    const std::vector<ll>& vector
) {
    const int rowCount = static_cast<int>(matrix.size());
    const int columnCount = static_cast<int>(matrix.front().size());
    std::vector<ll> result(columnCount, 0);
    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            result[column] = (result[column]
                + multiplyMod(matrix[row][column], vector[row])) % kMod;
        }
    }
    return result;
}

Matrix woodburyCore(const Matrix& inverse, const Matrix& left, const Matrix& right) {
    const Matrix directions = multiplySquareByRect(inverse, left);
    const int size = static_cast<int>(inverse.size());
    const int rank = static_cast<int>(left.front().size());
    Matrix core(rank, std::vector<ll>(rank, 0));
    for (int row = 0; row < rank; ++row) {
        for (int column = 0; column < rank; ++column) {
            ll value = row == column ? 1 : 0;
            for (int index = 0; index < size; ++index) {
                value = (value
                    + multiplyMod(right[index][row], directions[index][column])) % kMod;
            }
            core[row][column] = value;
        }
    }
    return core;
}

std::vector<ll> solveWoodburyLowRank(
    const Matrix& matrix,
    const Matrix& left,
    const Matrix& right,
    const std::vector<ll>& rhs
) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const Matrix directions = multiplySquareByRect(inverse, left);
    const Matrix core = woodburyCore(inverse, left, right);
    Matrix coreInverse;
    invertMatrix(core, coreInverse);
    const std::vector<ll> base = multiplyMatrixVector(inverse, rhs);
    const std::vector<ll> projected = multiplyTransposedRectVector(right, base);
    const std::vector<ll> weights = multiplyMatrixVector(coreInverse, projected);
    std::vector<ll> answer = base;
    const int size = static_cast<int>(matrix.size());
    const int rank = static_cast<int>(left.front().size());
    for (int row = 0; row < size; ++row) {
        ll correction = 0;
        for (int column = 0; column < rank; ++column) {
            correction = (correction
                + multiplyMod(directions[row][column], weights[column])) % kMod;
        }
        answer[row] = normalize(answer[row] - correction);
    }
    return answer;
}

ll determinantWoodbury(const Matrix& matrix, const Matrix& left, const Matrix& right) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const Matrix transformed = multiplySquareByRect(inverse, left);
    const int size = static_cast<int>(matrix.size());
    const int rank = static_cast<int>(left.front().size());
    Matrix core(rank, std::vector<ll>(rank, 0));
    for (int row = 0; row < rank; ++row) {
        for (int column = 0; column < rank; ++column) {
            ll value = row == column ? 1 : 0;
            for (int index = 0; index < size; ++index) {
                value = (value
                    + multiplyMod(right[index][row], transformed[index][column])) % kMod;
            }
            core[row][column] = value;
        }
    }
    return multiplyMod(determinantMod(matrix), determinantMod(core));
}

ll determinantSchurComplement(
    const Matrix& matrix,
    const Matrix& upperRight,
    const Matrix& lowerLeft,
    const Matrix& lowerRight
) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const Matrix transformed = multiplySquareByRect(inverse, upperRight);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    Matrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            ll correction = 0;
            for (int index = 0; index < size; ++index) {
                correction = (correction
                    + multiplyMod(lowerLeft[row][index], transformed[index][column])) % kMod;
            }
            complement[row][column] = normalize(complement[row][column] - correction);
        }
    }
    return multiplyMod(determinantMod(matrix), determinantMod(complement));
}

int rankSchurComplement(
    const Matrix& matrix,
    const Matrix& upperRight,
    const Matrix& lowerLeft,
    const Matrix& lowerRight
) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const Matrix transformed = multiplySquareByRect(inverse, upperRight);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    Matrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            ll correction = 0;
            for (int index = 0; index < size; ++index) {
                correction = (correction
                    + multiplyMod(lowerLeft[row][index], transformed[index][column])) % kMod;
            }
            complement[row][column] = normalize(complement[row][column] - correction);
        }
    }
    return size + matrixRank(complement);
}

std::vector<ll> solveSchurComplement(
    const Matrix& matrix,
    const Matrix& upperRight,
    const Matrix& lowerLeft,
    const Matrix& lowerRight,
    const std::vector<ll>& upperRhs,
    const std::vector<ll>& lowerRhs
) {
    Matrix inverse;
    invertMatrix(matrix, inverse);
    const Matrix transformed = multiplySquareByRect(inverse, upperRight);
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    Matrix complement = lowerRight;
    for (int row = 0; row < blockSize; ++row) {
        for (int column = 0; column < blockSize; ++column) {
            ll correction = 0;
            for (int index = 0; index < size; ++index) {
                correction = (correction
                    + multiplyMod(lowerLeft[row][index], transformed[index][column])) % kMod;
            }
            complement[row][column] = normalize(complement[row][column] - correction);
        }
    }
    Matrix complementInverse;
    invertMatrix(complement, complementInverse);
    const std::vector<ll> baseUpper = multiplyMatrixVector(inverse, upperRhs);
    const std::vector<ll> projection = multiplyRectVector(lowerLeft, baseUpper);
    std::vector<ll> reducedRhs(blockSize, 0);
    for (int row = 0; row < blockSize; ++row) {
        reducedRhs[row] = normalize(lowerRhs[row] - projection[row]);
    }
    const std::vector<ll> lowerSolution = multiplyMatrixVector(complementInverse, reducedRhs);
    const std::vector<ll> upperCorrection =
        multiplyMatrixVector(inverse, multiplyRectVector(upperRight, lowerSolution));
    std::vector<ll> answer = baseUpper;
    for (int index = 0; index < size; ++index) {
        answer[index] = normalize(answer[index] - upperCorrection[index]);
    }
    answer.insert(answer.end(), lowerSolution.begin(), lowerSolution.end());
    return answer;
}

Matrix inverseSchurComplement(
    const Matrix& matrix,
    const Matrix& upperRight,
    const Matrix& lowerLeft,
    const Matrix& lowerRight
) {
    const int size = static_cast<int>(matrix.size());
    const int blockSize = static_cast<int>(lowerRight.size());
    const int totalSize = size + blockSize;
    Matrix inverse(totalSize, std::vector<ll>(totalSize, 0));
    for (int column = 0; column < totalSize; ++column) {
        std::vector<ll> upperRhs(size, 0);
        std::vector<ll> lowerRhs(blockSize, 0);
        if (column < size) {
            upperRhs[column] = 1;
        } else {
            lowerRhs[column - size] = 1;
        }
        const std::vector<ll> solution = solveSchurComplement(
            matrix,
            upperRight,
            lowerLeft,
            lowerRight,
            upperRhs,
            lowerRhs
        );
        for (int row = 0; row < totalSize; ++row) {
            inverse[row][column] = solution[row];
        }
    }
    return inverse;
}

GeneratedCase generateCharacteristicPolynomial(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M37-characteristic-polynomial operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 6);
        Matrix matrix(size, std::vector<ll>(size, 0));
        for (auto& row : matrix) {
            for (ll& value : row) value = randomInt(random, 0, 9);
        }
        if (query % 5 == 1) {
            matrix.assign(size, std::vector<ll>(size, 0));
            for (int index = 0; index < size; ++index) matrix[index][index] = index + 1;
        } else if (query % 5 == 2 && size > 1) {
            matrix.assign(size, std::vector<ll>(size, 0));
            for (int index = 0; index + 1 < size; ++index) matrix[index][index + 1] = 1;
        }
        in << "char " << size << '\n';
        appendMatrix(in, matrix);
        const std::vector<ll> coefficients = characteristicPolynomial(matrix);
        for (int degree = size; degree >= 0; --degree) {
            if (degree != size) out << ' ';
            out << coefficients[degree];
        }
        out << '\n';
    }
    return {in.str(), out.str()};
}

GeneratedCase generateCayleyHamiltonTrace(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M38-cayley-hamilton-trace operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 5);
        std::uint64_t exponent = static_cast<std::uint64_t>(randomInt(random, 0, 80));
        if (query % 5 == 0) exponent = 0;
        if (query % 7 == 0) exponent = 1'000'000'000'000ULL + static_cast<std::uint64_t>(query);
        Matrix matrix(size, std::vector<ll>(size, 0));
        for (auto& row : matrix) {
            for (ll& value : row) value = randomInt(random, 0, 8);
        }
        if (query % 4 == 1 && size > 1) {
            matrix[0][1] = (matrix[0][1] + 3) % kMod;
            matrix[1][0] = (matrix[1][0] + 5) % kMod;
        }
        in << "trace " << size << ' ' << exponent << '\n';
        appendMatrix(in, matrix);
        out << traceMatrix(matrixPower(matrix, exponent)) << '\n';
    }
    return {in.str(), out.str()};
}

GeneratedCase generateDeterminantLemma(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M39-determinant-lemma operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        const int size = randomInt(random, 1, 5);
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        in << "lemma " << size << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            std::vector<ll> left(size, 0);
            std::vector<ll> right(size, 0);
            for (ll& value : left) value = randomInt(random, 0, 9);
            for (ll& value : right) value = randomInt(random, 0, 9);
            if (query % 4 == 1 && update == 0 && size > 1) {
                left[0] = 1;
                left[1] = 2;
                right[0] = 3;
                right[1] = 4;
            }
            appendVector(in, left);
            appendVector(in, right);
            out << determinantRankOneUpdate(matrix, left, right) << '\n';
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateShermanMorrisonSolve(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M40-sherman-morrison-solve operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 1, 5);
        if (query % 4 <= 1) size = std::max(size, 2);
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            matrix = identityMatrix(size);
            matrix[0][1] = 2;
        } else if (query % 4 == 1) {
            matrix = identityMatrix(size);
        }
        Matrix inverse;
        invertMatrix(matrix, inverse);
        in << "solve " << size << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            std::vector<ll> left(size, 0);
            std::vector<ll> right(size, 0);
            std::vector<ll> rhs(size, 0);
            ll denominator = 0;
            for (int attempt = 0; attempt < 100 && denominator == 0; ++attempt) {
                for (ll& value : left) value = randomInt(random, 0, 9);
                for (ll& value : right) value = randomInt(random, 0, 9);
                for (ll& value : rhs) value = randomInt(random, 0, 12);
                if (query % 4 == 0 && update == 0) {
                    left[0] = 1;
                    left[1] = 3;
                    right[0] = 4;
                    right[1] = 5;
                    rhs[0] = 7;
                    rhs[1] = 11;
                } else if (query % 4 == 1 && update == 0) {
                    left[0] = 1;
                    left[1] = 1;
                    right[0] = 1;
                    right[1] = 0;
                    rhs[0] = 4;
                    rhs[1] = 6;
                }
                denominator = normalize(
                    1 + dotVectors(right, multiplyMatrixVector(inverse, left))
                );
            }
            if (denominator == 0) {
                left.assign(size, 0);
                right.assign(size, 0);
                rhs.assign(size, 1);
            }
            appendVector(in, left);
            appendVector(in, right);
            appendVector(in, rhs);
            appendVector(out, solveShermanMorrison(matrix, left, right, rhs));
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateWoodburyDeterminant(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M41-woodbury-determinant operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 2, 5);
        int rank = randomInt(random, 1, std::min(3, size));
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 2;
            rank = 2;
            matrix = {{2, 1}, {0, 3}};
        } else if (query % 4 == 1) {
            size = 2;
            rank = 1;
            matrix = {{1, 2}, {0, 1}};
        }
        in << "woodbury " << size << ' ' << rank << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix left(size, std::vector<ll>(rank, 0));
            Matrix right(size, std::vector<ll>(rank, 0));
            for (auto& row : left) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            for (auto& row : right) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            if (query % 4 == 0 && update == 0) {
                left = {{1, 0}, {0, 1}};
                right = {{1, 2}, {3, 4}};
            } else if (query % 4 == 1 && update == 0) {
                left = {{1}, {3}};
                right = {{4}, {5}};
            }
            appendMatrix(in, left);
            appendMatrix(in, right);
            out << determinantWoodbury(matrix, left, right) << '\n';
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateSchurComplementDeterminant(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M42-schur-complement-determinant operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 1, 5);
        int blockSize = randomInt(random, 1, std::min(3, size));
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 2;
            blockSize = 2;
            matrix = {{2, 1}, {0, 3}};
        } else if (query % 4 == 1) {
            size = 2;
            blockSize = 1;
            matrix = {{1, 2}, {0, 1}};
        }
        in << "schur " << size << ' ' << blockSize << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix upperRight(size, std::vector<ll>(blockSize, 0));
            Matrix lowerLeft(blockSize, std::vector<ll>(size, 0));
            Matrix lowerRight(blockSize, std::vector<ll>(blockSize, 0));
            for (auto& row : upperRight) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            for (auto& row : lowerLeft) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            for (auto& row : lowerRight) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            if (query % 4 == 0 && update == 0) {
                upperRight = {{1, 0}, {0, 1}};
                lowerLeft = {{1, 2}, {3, 4}};
                lowerRight = {{7, 1}, {2, 6}};
            } else if (query % 4 == 1 && update == 0) {
                upperRight = {{1}, {3}};
                lowerLeft = {{4, 5}};
                lowerRight = {{6}};
            }
            appendMatrix(in, upperRight);
            appendMatrix(in, lowerLeft);
            appendMatrix(in, lowerRight);
            out << determinantSchurComplement(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight
            ) << '\n';
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateWoodburySolve(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M43-woodbury-solve operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 2, 5);
        int rank = randomInt(random, 1, std::min(3, size));
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 2;
            rank = 2;
            matrix = {{2, 1}, {0, 3}};
        } else if (query % 4 == 1) {
            size = 2;
            rank = 1;
            matrix = {{1, 2}, {0, 1}};
        }
        Matrix inverse;
        invertMatrix(matrix, inverse);
        in << "woodsolve " << size << ' ' << rank << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix left(size, std::vector<ll>(rank, 0));
            Matrix right(size, std::vector<ll>(rank, 0));
            std::vector<ll> rhs(size, 0);
            bool invertibleCore = false;
            for (int attempt = 0; attempt < 100 && !invertibleCore; ++attempt) {
                for (auto& row : left) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (auto& row : right) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (ll& value : rhs) value = randomInt(random, 0, 12);
                if (query % 4 == 0 && update == 0) {
                    left = {{1, 0}, {0, 1}};
                    right = {{1, 2}, {3, 4}};
                    rhs = {7, 11};
                } else if (query % 4 == 1 && update == 0) {
                    left = {{1}, {3}};
                    right = {{4}, {5}};
                    rhs = {6, 7};
                }
                Matrix coreInverse;
                invertibleCore = invertMatrix(woodburyCore(inverse, left, right), coreInverse);
            }
            if (!invertibleCore) {
                left.assign(size, std::vector<ll>(rank, 0));
                right.assign(size, std::vector<ll>(rank, 0));
                rhs.assign(size, 1);
            }
            appendMatrix(in, left);
            appendMatrix(in, right);
            appendVector(in, rhs);
            appendVector(out, solveWoodburyLowRank(matrix, left, right, rhs));
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateSchurComplementSolve(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M44-schur-complement-solve operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 1, 5);
        int blockSize = randomInt(random, 1, std::min(3, size));
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 2;
            blockSize = 2;
            matrix = {{2, 1}, {0, 3}};
        } else if (query % 4 == 1) {
            size = 2;
            blockSize = 1;
            matrix = {{1, 2}, {0, 1}};
        }
        Matrix inverse;
        invertMatrix(matrix, inverse);
        in << "blocksolve " << size << ' ' << blockSize << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix upperRight(size, std::vector<ll>(blockSize, 0));
            Matrix lowerLeft(blockSize, std::vector<ll>(size, 0));
            Matrix lowerRight(blockSize, std::vector<ll>(blockSize, 0));
            std::vector<ll> upperRhs(size, 0);
            std::vector<ll> lowerRhs(blockSize, 0);
            bool invertibleComplement = false;
            for (int attempt = 0; attempt < 100 && !invertibleComplement; ++attempt) {
                for (auto& row : upperRight) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (auto& row : lowerLeft) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (auto& row : lowerRight) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (ll& value : upperRhs) value = randomInt(random, 0, 12);
                for (ll& value : lowerRhs) value = randomInt(random, 0, 12);
                if (query % 4 == 0 && update == 0) {
                    upperRight = {{1, 0}, {0, 1}};
                    lowerLeft = {{1, 2}, {3, 4}};
                    lowerRight = {{7, 1}, {2, 6}};
                    upperRhs = {7, 11};
                    lowerRhs = {5, 8};
                } else if (query % 4 == 1 && update == 0) {
                    upperRight = {{1}, {3}};
                    lowerLeft = {{4, 5}};
                    lowerRight = {{6}};
                    upperRhs = {6, 7};
                    lowerRhs = {8};
                }
                Matrix transformed = multiplySquareByRect(inverse, upperRight);
                Matrix complement = lowerRight;
                for (int row = 0; row < blockSize; ++row) {
                    for (int column = 0; column < blockSize; ++column) {
                        ll correction = 0;
                        for (int index = 0; index < size; ++index) {
                            correction = (correction + multiplyMod(
                                lowerLeft[row][index],
                                transformed[index][column]
                            )) % kMod;
                        }
                        complement[row][column] = normalize(complement[row][column] - correction);
                    }
                }
                Matrix complementInverse;
                invertibleComplement = invertMatrix(complement, complementInverse);
            }
            if (!invertibleComplement) {
                upperRight.assign(size, std::vector<ll>(blockSize, 0));
                lowerLeft.assign(blockSize, std::vector<ll>(size, 0));
                lowerRight.assign(blockSize, std::vector<ll>(blockSize, 0));
                for (int index = 0; index < blockSize; ++index) lowerRight[index][index] = 1;
                upperRhs.assign(size, 1);
                lowerRhs.assign(blockSize, 1);
            }
            appendMatrix(in, upperRight);
            appendMatrix(in, lowerLeft);
            appendMatrix(in, lowerRight);
            appendVector(in, upperRhs);
            appendVector(in, lowerRhs);
            appendVector(out, solveSchurComplement(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight,
                upperRhs,
                lowerRhs
            ));
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateSchurComplementInverse(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M45-schur-complement-inverse operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 1, 5);
        int blockSize = randomInt(random, 1, std::min(3, size));
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 2;
            blockSize = 2;
            matrix = {{2, 1}, {0, 3}};
        } else if (query % 4 == 1) {
            size = 2;
            blockSize = 1;
            matrix = {{1, 2}, {0, 1}};
        }
        Matrix inverse;
        invertMatrix(matrix, inverse);
        in << "blockinverse " << size << ' ' << blockSize << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix upperRight(size, std::vector<ll>(blockSize, 0));
            Matrix lowerLeft(blockSize, std::vector<ll>(size, 0));
            Matrix lowerRight(blockSize, std::vector<ll>(blockSize, 0));
            bool invertibleComplement = false;
            for (int attempt = 0; attempt < 100 && !invertibleComplement; ++attempt) {
                for (auto& row : upperRight) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (auto& row : lowerLeft) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                for (auto& row : lowerRight) {
                    for (ll& value : row) value = randomInt(random, 0, 9);
                }
                if (query % 4 == 0 && update == 0) {
                    upperRight = {{1, 0}, {0, 1}};
                    lowerLeft = {{1, 2}, {3, 4}};
                    lowerRight = {{7, 1}, {2, 6}};
                } else if (query % 4 == 1 && update == 0) {
                    upperRight = {{1}, {3}};
                    lowerLeft = {{4, 5}};
                    lowerRight = {{6}};
                }
                Matrix transformed = multiplySquareByRect(inverse, upperRight);
                Matrix complement = lowerRight;
                for (int row = 0; row < blockSize; ++row) {
                    for (int column = 0; column < blockSize; ++column) {
                        ll correction = 0;
                        for (int index = 0; index < size; ++index) {
                            correction = (correction + multiplyMod(
                                lowerLeft[row][index],
                                transformed[index][column]
                            )) % kMod;
                        }
                        complement[row][column] = normalize(complement[row][column] - correction);
                    }
                }
                Matrix complementInverse;
                invertibleComplement = invertMatrix(complement, complementInverse);
            }
            if (!invertibleComplement) {
                upperRight.assign(size, std::vector<ll>(blockSize, 0));
                lowerLeft.assign(blockSize, std::vector<ll>(size, 0));
                lowerRight.assign(blockSize, std::vector<ll>(blockSize, 0));
                for (int index = 0; index < blockSize; ++index) lowerRight[index][index] = 1;
            }
            appendMatrix(in, upperRight);
            appendMatrix(in, lowerLeft);
            appendMatrix(in, lowerRight);
            appendMatrix(out, inverseSchurComplement(matrix, upperRight, lowerLeft, lowerRight));
        }
    }
    return {in.str(), out.str()};
}

GeneratedCase generateSchurComplementRank(std::mt19937_64& random, int operationCount) {
    if (operationCount < 3) {
        throw std::runtime_error("M46-schur-complement-rank operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    for (int query = 0; query < operationCount; ++query) {
        int size = randomInt(random, 1, 5);
        int blockSize = randomInt(random, 1, 4);
        const int updateCount = randomInt(random, 1, 4);
        Matrix matrix = randomInvertibleMatrix(random, size);
        if (query % 4 == 0) {
            size = 1;
            blockSize = 1;
            matrix = {{1}};
        } else if (query % 4 == 1) {
            size = 2;
            blockSize = 1;
            matrix = {{1, 2}, {0, 1}};
        } else if (query % 4 == 2) {
            size = 1;
            blockSize = 2;
            matrix = {{1}};
        }
        in << "blockrank " << size << ' ' << blockSize << ' ' << updateCount << '\n';
        appendMatrix(in, matrix);
        for (int update = 0; update < updateCount; ++update) {
            Matrix upperRight(size, std::vector<ll>(blockSize, 0));
            Matrix lowerLeft(blockSize, std::vector<ll>(size, 0));
            Matrix lowerRight(blockSize, std::vector<ll>(blockSize, 0));
            for (auto& row : upperRight) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            for (auto& row : lowerLeft) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            for (auto& row : lowerRight) {
                for (ll& value : row) value = randomInt(random, 0, 9);
            }
            if (query % 4 == 0 && update == 0) {
                upperRight = {{2}};
                lowerLeft = {{3}};
                lowerRight = {{6}};
            } else if (query % 4 == 1 && update == 0) {
                upperRight = {{1}, {3}};
                lowerLeft = {{4, 5}};
                lowerRight = {{1'000'000'002}};
            } else if (query % 4 == 2 && update == 0) {
                upperRight = {{1, 2}};
                lowerLeft = {{3}, {4}};
                lowerRight = {{3, 6}, {4, 8}};
            }
            appendMatrix(in, upperRight);
            appendMatrix(in, lowerLeft);
            appendMatrix(in, lowerRight);
            out << rankSchurComplement(matrix, upperRight, lowerLeft, lowerRight) << '\n';
        }
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
    registry.emplace("M34-undirected-matrix-tree", generateUndirectedMatrixTree);
    registry.emplace("M35-directed-arborescence", generateDirectedArborescence);
    registry.emplace("M36-best-theorem-count", generateBestTheoremCount);
    registry.emplace("M37-characteristic-polynomial", generateCharacteristicPolynomial);
    registry.emplace("M38-cayley-hamilton-trace", generateCayleyHamiltonTrace);
    registry.emplace("M39-determinant-lemma", generateDeterminantLemma);
    registry.emplace("M40-sherman-morrison-solve", generateShermanMorrisonSolve);
    registry.emplace("M41-woodbury-determinant", generateWoodburyDeterminant);
    registry.emplace("M42-schur-complement-determinant", generateSchurComplementDeterminant);
    registry.emplace("M43-woodbury-solve", generateWoodburySolve);
    registry.emplace("M44-schur-complement-solve", generateSchurComplementSolve);
    registry.emplace("M45-schur-complement-inverse", generateSchurComplementInverse);
    registry.emplace("M46-schur-complement-rank", generateSchurComplementRank);
}

}  // namespace judge::generators
