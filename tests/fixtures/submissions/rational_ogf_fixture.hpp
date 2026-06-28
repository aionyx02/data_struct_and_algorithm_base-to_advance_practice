#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef OGF_REVERSED_COEFFICIENTS
#define OGF_REVERSED_COEFFICIENTS 0
#endif
#ifndef OGF_SHIFTED_INITIAL
#define OGF_SHIFTED_INITIAL 0
#endif

using ogf64 = long long;
using ogf128 = __int128_t;
constexpr ogf64 OGF_MOD = 1'000'000'007;

std::vector<ogf64> combineOgf(
    const std::vector<ogf64>& left,
    const std::vector<ogf64>& right,
    const std::vector<ogf64>& coefficients
) {
    int order = static_cast<int>(coefficients.size());
    std::vector<ogf64> product(2 * order - 1, 0);
    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            product[i + j] = (product[i + j]
                + static_cast<ogf64>(static_cast<ogf128>(left[i]) * right[j] % OGF_MOD))
                % OGF_MOD;
        }
    }
    for (int degree = 2 * order - 2; degree >= order; --degree) {
        for (int offset = 1; offset <= order; ++offset) {
            int coefficientIndex = OGF_REVERSED_COEFFICIENTS
                ? order - offset
                : offset - 1;
            product[degree - offset] = (product[degree - offset]
                + static_cast<ogf64>(static_cast<ogf128>(product[degree])
                                     * coefficients[coefficientIndex] % OGF_MOD))
                % OGF_MOD;
        }
    }
    product.resize(order);
    return product;
}

ogf64 recurrenceCoefficient(
    const std::vector<ogf64>& initial,
    const std::vector<ogf64>& coefficients,
    std::uint64_t index
) {
    int order = static_cast<int>(initial.size());
    std::vector<ogf64> result(order, 0);
    std::vector<ogf64> base(order, 0);
    result[0] = 1;
    if (order == 1) base[0] = coefficients[0];
    else base[1] = 1;
    while (index > 0) {
        if (index & 1) result = combineOgf(result, base, coefficients);
        base = combineOgf(base, base, coefficients);
        index >>= 1;
    }
    ogf64 answer = 0;
    for (int i = 0; i < order; ++i) {
        int initialIndex = OGF_SHIFTED_INITIAL ? (i + 1) % order : i;
        answer = (answer + static_cast<ogf64>(static_cast<ogf128>(result[i])
                                             * initial[initialIndex] % OGF_MOD))
            % OGF_MOD;
    }
    return answer;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int order;
        std::uint64_t index;
        std::cin >> operation >> order >> index;
        std::vector<ogf64> initial(order);
        std::vector<ogf64> coefficients(order);
        for (ogf64& value : initial) std::cin >> value;
        for (ogf64& coefficient : coefficients) std::cin >> coefficient;
        std::cout << recurrenceCoefficient(initial, coefficients, index) << '\n';
    }
    return 0;
}
