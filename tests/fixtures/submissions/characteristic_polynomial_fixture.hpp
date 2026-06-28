#include "matrix_polynomial_support_fixture.hpp"

#ifndef CHAR_DET_A_MINUS_XI
#define CHAR_DET_A_MINUS_XI 0
#endif
#ifndef CHAR_LOW_TO_HIGH
#define CHAR_LOW_TO_HIGH 0
#endif

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int size;
        std::cin >> operation >> size;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        const std::vector<mp64> coefficients = characteristicPolynomialMp(
            matrix,
            CHAR_DET_A_MINUS_XI != 0
        );
        if (CHAR_LOW_TO_HIGH != 0) {
            for (int degree = 0; degree <= size; ++degree) {
                if (degree != 0) std::cout << ' ';
                std::cout << coefficients[degree];
            }
        } else {
            for (int degree = size; degree >= 0; --degree) {
                if (degree != size) std::cout << ' ';
                std::cout << coefficients[degree];
            }
        }
        std::cout << '\n';
    }
    return 0;
}
