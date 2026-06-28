#include "matrix_tree_support_fixture.hpp"

#ifndef BEST_OMIT_FACTORIALS
#define BEST_OMIT_FACTORIALS 0
#endif
#ifndef BEST_FULL_FACTORIALS
#define BEST_FULL_FACTORIALS 0
#endif

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int vertexCount;
        int edgeCount;
        int root;
        std::cin >> operation >> vertexCount >> edgeCount >> root;
        --root;
        std::vector<MatrixTreeEdge> edges(edgeCount);
        for (MatrixTreeEdge& edge : edges) {
            std::cin >> edge.from >> edge.to;
            edge.weight = 1;
            --edge.from;
            --edge.to;
        }
        std::cout << bestTheoremMt(
            vertexCount,
            root,
            edges,
            BEST_OMIT_FACTORIALS != 0,
            BEST_FULL_FACTORIALS != 0
        ) << '\n';
    }
    return 0;
}
