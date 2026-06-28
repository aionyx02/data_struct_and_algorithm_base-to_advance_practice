#include "matrix_tree_support_fixture.hpp"

#ifndef MATRIX_TREE_ONE_WAY
#define MATRIX_TREE_ONE_WAY 0
#endif
#ifndef MATRIX_TREE_IGNORE_WEIGHTS
#define MATRIX_TREE_IGNORE_WEIGHTS 0
#endif

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int vertexCount;
        int edgeCount;
        std::cin >> operation >> vertexCount >> edgeCount;
        std::vector<MatrixTreeEdge> edges(edgeCount);
        for (MatrixTreeEdge& edge : edges) {
            std::cin >> edge.from >> edge.to >> edge.weight;
            --edge.from;
            --edge.to;
        }
        std::cout << undirectedTreeCountMt(
            vertexCount,
            edges,
            MATRIX_TREE_ONE_WAY != 0,
            MATRIX_TREE_IGNORE_WEIGHTS != 0
        ) << '\n';
    }
    return 0;
}
