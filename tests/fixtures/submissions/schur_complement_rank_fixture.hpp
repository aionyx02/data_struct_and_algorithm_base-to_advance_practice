#include "matrix_polynomial_support_fixture.hpp"

#ifndef SCHUR_RANK_PLUS_SIGN
#define SCHUR_RANK_PLUS_SIGN 0
#endif
#ifndef SCHUR_RANK_TRANSPOSED_PRODUCT
#define SCHUR_RANK_TRANSPOSED_PRODUCT 0
#endif

MpMatrix readSchurRankRectMatrix(int rowCount, int columnCount) {
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
            const MpMatrix upperRight = readSchurRankRectMatrix(size, blockSize);
            const MpMatrix lowerLeft = readSchurRankRectMatrix(blockSize, size);
            const MpMatrix lowerRight = readSchurRankRectMatrix(blockSize, blockSize);
            std::cout << rankSchurComplementMp(
                matrix,
                upperRight,
                lowerLeft,
                lowerRight,
                SCHUR_RANK_PLUS_SIGN != 0,
                SCHUR_RANK_TRANSPOSED_PRODUCT != 0
            ) << '\n';
        }
    }
    return 0;
}
