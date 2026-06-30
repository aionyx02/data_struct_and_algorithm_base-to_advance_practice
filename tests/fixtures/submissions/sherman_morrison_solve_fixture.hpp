#include "matrix_polynomial_support_fixture.hpp"

#ifndef SHERMAN_TRANSPOSE_UPDATE
#define SHERMAN_TRANSPOSE_UPDATE 0
#endif
#ifndef SHERMAN_OMIT_DENOMINATOR
#define SHERMAN_OMIT_DENOMINATOR 0
#endif

void printShermanVector(const std::vector<mp64>& values) {
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
        int queryCount;
        std::cin >> operation >> size >> queryCount;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        for (int query = 0; query < queryCount; ++query) {
            std::vector<mp64> left(size, 0);
            std::vector<mp64> right(size, 0);
            std::vector<mp64> rhs(size, 0);
            for (mp64& value : left) std::cin >> value;
            for (mp64& value : right) std::cin >> value;
            for (mp64& value : rhs) std::cin >> value;
            printShermanVector(solveShermanMorrisonMp(
                matrix,
                left,
                right,
                rhs,
                SHERMAN_TRANSPOSE_UPDATE != 0,
                SHERMAN_OMIT_DENOMINATOR != 0
            ));
        }
    }
    return 0;
}
