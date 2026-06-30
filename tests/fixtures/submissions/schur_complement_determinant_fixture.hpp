#include "matrix_polynomial_support_fixture.hpp"

#ifndef SCHUR_PLUS_SIGN
#define SCHUR_PLUS_SIGN 0
#endif
#ifndef SCHUR_TRANSPOSED_PRODUCT
#define SCHUR_TRANSPOSED_PRODUCT 0
#endif

MpMatrix readSchurRectMatrix(int rowCount, int columnCount) {
    MpMatrix matrix(rowCount, std::vector<mp64>(columnCount, 0));
    for (auto& row : matrix) {
        for (mp64& value : row) std::cin >> value;
    }
    return matrix;
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
            const MpMatrix upperRight = readSchurRectMatrix(size, blockSize);
            const MpMatrix lowerLeft = readSchurRectMatrix(blockSize, size);
            const MpMatrix lowerRight = readSchurRectMatrix(blockSize, blockSize);
            std::cout << determinantSchurComplementMp(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight,
                SCHUR_PLUS_SIGN != 0,
                SCHUR_TRANSPOSED_PRODUCT != 0
            ) << '\n';
        }
    }
    return 0;
}
