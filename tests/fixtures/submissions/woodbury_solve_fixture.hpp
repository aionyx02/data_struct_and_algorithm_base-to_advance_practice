#include "matrix_polynomial_support_fixture.hpp"

#ifndef WOODBURY_SOLVE_TRANSPOSED_CORE
#define WOODBURY_SOLVE_TRANSPOSED_CORE 0
#endif
#ifndef WOODBURY_SOLVE_DIAGONAL_CORE
#define WOODBURY_SOLVE_DIAGONAL_CORE 0
#endif

MpMatrix readWoodburySolveRectMatrix(int rowCount, int columnCount) {
    MpMatrix matrix(rowCount, std::vector<mp64>(columnCount, 0));
    for (auto& row : matrix) {
        for (mp64& value : row) std::cin >> value;
    }
    return matrix;
}

void printWoodburySolveVector(const std::vector<mp64>& values) {
    for (int index = 0; index < static_cast<int>(values.size()); ++index) {
        if (index != 0) std::cout << ' ';
        std::cout << values[index];
    }
    std::cout << '\n';
}

int main() {
    int batchCount;
    if (!(std::cin >> batchCount)) return 0;
    for (int batch = 0; batch < batchCount; ++batch) {
        std::string operation;
        int size;
        int rank;
        int queryCount;
        std::cin >> operation >> size >> rank >> queryCount;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        for (int query = 0; query < queryCount; ++query) {
            const MpMatrix left = readWoodburySolveRectMatrix(size, rank);
            const MpMatrix right = readWoodburySolveRectMatrix(size, rank);
            std::vector<mp64> rhs(size, 0);
            for (mp64& value : rhs) std::cin >> value;
            printWoodburySolveVector(solveWoodburyMp(
                matrix,
                left,
                right,
                rhs,
                WOODBURY_SOLVE_TRANSPOSED_CORE != 0,
                WOODBURY_SOLVE_DIAGONAL_CORE != 0
            ));
        }
    }
    return 0;
}
