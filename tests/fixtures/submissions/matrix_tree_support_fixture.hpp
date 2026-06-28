#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using mt64 = long long;
constexpr mt64 MT_MOD = 1'000'000'007;

struct MatrixTreeEdge {
    int from = 0;
    int to = 0;
    mt64 weight = 1;
};

mt64 normalizeMt(mt64 value) {
    value %= MT_MOD;
    if (value < 0) value += MT_MOD;
    return value;
}

mt64 multiplyMt(mt64 left, mt64 right) {
    return static_cast<mt64>(
        static_cast<__int128_t>(left) * right % MT_MOD
    );
}

mt64 powerMt(mt64 base, mt64 exponent) {
    mt64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyMt(result, base);
        base = multiplyMt(base, base);
        exponent >>= 1;
    }
    return result;
}

mt64 determinantMt(std::vector<std::vector<mt64>> matrix) {
    const int size = static_cast<int>(matrix.size());
    mt64 answer = 1;
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
            answer = normalizeMt(-answer);
        }
        answer = multiplyMt(answer, matrix[column][column]);
        const mt64 inverse = powerMt(matrix[column][column], MT_MOD - 2);
        for (int row = column + 1; row < size; ++row) {
            if (matrix[row][column] == 0) continue;
            const mt64 factor = multiplyMt(matrix[row][column], inverse);
            for (int current = column; current < size; ++current) {
                matrix[row][current] = normalizeMt(
                    matrix[row][current] - multiplyMt(factor, matrix[column][current])
                );
            }
        }
    }
    return answer;
}

std::vector<std::vector<mt64>> cofactorMt(
    const std::vector<std::vector<mt64>>& laplacian,
    int root
) {
    const int size = static_cast<int>(laplacian.size());
    std::vector<std::vector<mt64>> cofactor;
    for (int row = 0; row < size; ++row) {
        if (row == root) continue;
        std::vector<mt64> current;
        for (int column = 0; column < size; ++column) {
            if (column != root) current.push_back(laplacian[row][column]);
        }
        cofactor.push_back(current);
    }
    return cofactor;
}

mt64 undirectedTreeCountMt(
    int vertexCount,
    const std::vector<MatrixTreeEdge>& edges,
    bool oneWay,
    bool ignoreWeights
) {
    if (vertexCount == 1) return 1;
    std::vector<std::vector<mt64>> laplacian(vertexCount, std::vector<mt64>(vertexCount, 0));
    for (const MatrixTreeEdge& edge : edges) {
        const mt64 weight = ignoreWeights ? 1 : normalizeMt(edge.weight);
        laplacian[edge.from][edge.from] = (laplacian[edge.from][edge.from] + weight) % MT_MOD;
        laplacian[edge.from][edge.to] = normalizeMt(laplacian[edge.from][edge.to] - weight);
        if (!oneWay) {
            laplacian[edge.to][edge.to] = (laplacian[edge.to][edge.to] + weight) % MT_MOD;
            laplacian[edge.to][edge.from] = normalizeMt(laplacian[edge.to][edge.from] - weight);
        }
    }
    return determinantMt(cofactorMt(laplacian, vertexCount - 1));
}

mt64 directedArborescenceMt(
    int vertexCount,
    int root,
    const std::vector<MatrixTreeEdge>& edges,
    bool useInLaplacian,
    bool forceRootOne
) {
    if (vertexCount == 1) return 1;
    if (forceRootOne) root = 0;
    std::vector<std::vector<mt64>> laplacian(vertexCount, std::vector<mt64>(vertexCount, 0));
    for (const MatrixTreeEdge& edge : edges) {
        if (useInLaplacian) {
            laplacian[edge.to][edge.to] = (laplacian[edge.to][edge.to] + edge.weight) % MT_MOD;
            laplacian[edge.to][edge.from] = normalizeMt(laplacian[edge.to][edge.from] - edge.weight);
        } else {
            laplacian[edge.from][edge.from] = (laplacian[edge.from][edge.from] + edge.weight) % MT_MOD;
            laplacian[edge.from][edge.to] = normalizeMt(laplacian[edge.from][edge.to] - edge.weight);
        }
    }
    return determinantMt(cofactorMt(laplacian, root));
}

mt64 bestTheoremMt(
    int vertexCount,
    int root,
    const std::vector<MatrixTreeEdge>& edges,
    bool omitFactorials,
    bool fullFactorials
) {
    std::vector<int> inDegree(vertexCount, 0);
    std::vector<int> outDegree(vertexCount, 0);
    for (const MatrixTreeEdge& edge : edges) {
        ++outDegree[edge.from];
        ++inDegree[edge.to];
    }
    if (inDegree != outDegree) return 0;
    mt64 answer = directedArborescenceMt(vertexCount, root, edges, false, false);
    if (answer == 0 || omitFactorials) return answer;
    int maximumDegree = 0;
    for (int degree : outDegree) maximumDegree = std::max(maximumDegree, degree);
    std::vector<mt64> factorial(maximumDegree + 1, 1);
    for (int value = 1; value <= maximumDegree; ++value) {
        factorial[value] = multiplyMt(factorial[value - 1], value);
    }
    for (int degree : outDegree) {
        const int factorIndex = fullFactorials ? degree : std::max(0, degree - 1);
        answer = multiplyMt(answer, factorial[factorIndex]);
    }
    return answer;
}
