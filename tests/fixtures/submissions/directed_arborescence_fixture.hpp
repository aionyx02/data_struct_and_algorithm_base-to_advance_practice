#include "matrix_tree_support_fixture.hpp"

#ifndef ARBORESCENCE_IN_LAPLACIAN
#define ARBORESCENCE_IN_LAPLACIAN 0
#endif
#ifndef ARBORESCENCE_FORCE_ROOT_ONE
#define ARBORESCENCE_FORCE_ROOT_ONE 0
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
            std::cin >> edge.from >> edge.to >> edge.weight;
            --edge.from;
            --edge.to;
        }
        std::cout << directedArborescenceMt(
            vertexCount,
            root,
            edges,
            ARBORESCENCE_IN_LAPLACIAN != 0,
            ARBORESCENCE_FORCE_ROOT_ONE != 0
        ) << '\n';
    }
    return 0;
}
