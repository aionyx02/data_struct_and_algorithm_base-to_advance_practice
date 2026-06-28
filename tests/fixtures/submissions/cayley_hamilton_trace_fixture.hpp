#include "matrix_polynomial_support_fixture.hpp"

#ifndef TRACE_DIAGONAL_ONLY
#define TRACE_DIAGONAL_ONLY 0
#endif
#ifndef TRACE_NEXT_POWER
#define TRACE_NEXT_POWER 0
#endif

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int size;
        std::uint64_t exponent;
        std::cin >> operation >> size >> exponent;
        MpMatrix matrix(size, std::vector<mp64>(size, 0));
        for (auto& row : matrix) {
            for (mp64& value : row) std::cin >> value;
        }
        if (TRACE_NEXT_POWER != 0) ++exponent;
        std::cout << tracePowerMp(matrix, exponent, TRACE_DIAGONAL_ONLY != 0) << '\n';
    }
    return 0;
}
