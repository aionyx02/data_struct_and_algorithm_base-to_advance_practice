#include "matrix_polynomial_support_fixture.hpp"

#ifndef WOODBURY_TRANSPOSED_CORE
#define WOODBURY_TRANSPOSED_CORE 0
#endif
#ifndef WOODBURY_DIAGONAL_ONLY
#define WOODBURY_DIAGONAL_ONLY 0
#endif

MpMatrix readRectMatrix(int rowCount, int columnCount) {
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
        int rank;
        int queryCount;
        std::cin >> operation >> size >> rank >> queryCount;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        for (int query = 0; query < queryCount; ++query) {
            const MpMatrix left = readRectMatrix(size, rank);
            const MpMatrix right = readRectMatrix(size, rank);
            std::cout << determinantWoodburyMp(
                matrix,
                left,
                right,
                WOODBURY_TRANSPOSED_CORE != 0,
                WOODBURY_DIAGONAL_ONLY != 0
            ) << '\n';
        }
    }
    return 0;
}
