#include "matrix_polynomial_support_fixture.hpp"

#ifndef SCHUR_INVERSE_PLUS_SIGN
#define SCHUR_INVERSE_PLUS_SIGN 0
#endif
#ifndef SCHUR_INVERSE_TRANSPOSED_PRODUCT
#define SCHUR_INVERSE_TRANSPOSED_PRODUCT 0
#endif

MpMatrix readSchurInverseRectMatrix(int rowCount, int columnCount) {
    MpMatrix matrix(rowCount, std::vector<mp64>(columnCount, 0));
    for (auto& row : matrix) {
        for (mp64& value : row) std::cin >> value;
    }
    return matrix;
}

void printSchurInverseMatrix(const MpMatrix& matrix) {
    for (const auto& row : matrix) {
        for (int column = 0; column < static_cast<int>(row.size()); ++column) {
            if (column != 0) std::cout << ' ';
            std::cout << row[column];
        }
        std::cout << '\n';
    }
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
            const MpMatrix upperRight = readSchurInverseRectMatrix(size, blockSize);
            const MpMatrix lowerLeft = readSchurInverseRectMatrix(blockSize, size);
            const MpMatrix lowerRight = readSchurInverseRectMatrix(blockSize, blockSize);
            printSchurInverseMatrix(inverseSchurComplementMp(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight,
                SCHUR_INVERSE_PLUS_SIGN != 0,
                SCHUR_INVERSE_TRANSPOSED_PRODUCT != 0
            ));
        }
    }
    return 0;
}
