#include "matrix_polynomial_support_fixture.hpp"

#ifndef LEMMA_TRANSPOSE_DOT
#define LEMMA_TRANSPOSE_DOT 0
#endif
#ifndef LEMMA_CUMULATIVE
#define LEMMA_CUMULATIVE 0
#endif

int main() {
    int batchCount;
    if (!(std::cin >> batchCount)) return 0;
    for (int batch = 0; batch < batchCount; ++batch) {
        std::string operation;
        int size;
        int updateCount;
        std::cin >> operation >> size >> updateCount;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        for (int update = 0; update < updateCount; ++update) {
            std::vector<mp64> left(size, 0);
            std::vector<mp64> right(size, 0);
            for (mp64& value : left) std::cin >> value;
            for (mp64& value : right) std::cin >> value;
            std::cout << determinantLemmaMp(
                matrix,
                left,
                right,
                LEMMA_TRANSPOSE_DOT != 0
            ) << '\n';
            if (LEMMA_CUMULATIVE != 0) addRankOneMp(matrix, left, right);
        }
    }
    return 0;
}
