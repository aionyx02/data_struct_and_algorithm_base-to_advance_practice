#include "matrix_polynomial_support_fixture.hpp"

#ifndef SCHUR_SOLVE_PLUS_SIGN
#define SCHUR_SOLVE_PLUS_SIGN 0
#endif
#ifndef SCHUR_SOLVE_TRANSPOSED_PRODUCT
#define SCHUR_SOLVE_TRANSPOSED_PRODUCT 0
#endif

MpMatrix readSchurSolveRectMatrix(int rowCount, int columnCount) {
    MpMatrix matrix(rowCount, std::vector<mp64>(columnCount, 0));
    for (auto& row : matrix) {
        for (mp64& value : row) std::cin >> value;
    }
    return matrix;
}

void printSchurSolveVector(const std::vector<mp64>& values) {
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
        int blockSize;
        int queryCount;
        std::cin >> operation >> size >> blockSize >> queryCount;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        for (int query = 0; query < queryCount; ++query) {
            const MpMatrix upperRight = readSchurSolveRectMatrix(size, blockSize);
            const MpMatrix lowerLeft = readSchurSolveRectMatrix(blockSize, size);
            const MpMatrix lowerRight = readSchurSolveRectMatrix(blockSize, blockSize);
            std::vector<mp64> upperRhs(size, 0);
            std::vector<mp64> lowerRhs(blockSize, 0);
            for (mp64& value : upperRhs) std::cin >> value;
            for (mp64& value : lowerRhs) std::cin >> value;
            printSchurSolveVector(solveSchurComplementMp(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight,
                upperRhs,
                lowerRhs,
                SCHUR_SOLVE_PLUS_SIGN != 0,
                SCHUR_SOLVE_TRANSPOSED_PRODUCT != 0
            ));
        }
    }
    return 0;
}
