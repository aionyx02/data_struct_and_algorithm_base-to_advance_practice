#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef BM_WRONG_SIGN
#define BM_WRONG_SIGN 0
#endif
#ifndef BM_REVERSED_REDUCTION
#define BM_REVERSED_REDUCTION 0
#endif

using bm64 = long long;
constexpr bm64 BM_MOD = 1'000'000'007;

bm64 normalizeBm(bm64 value) {
    value %= BM_MOD;
    if (value < 0) value += BM_MOD;
    return value;
}

bm64 multiplyBm(bm64 left, bm64 right) {
    return static_cast<bm64>(
        static_cast<__int128_t>(left) * right % BM_MOD
    );
}

bm64 powerBm(bm64 base, bm64 exponent) {
    bm64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = multiplyBm(result, base);
        base = multiplyBm(base, base);
        exponent >>= 1;
    }
    return result;
}

std::vector<bm64> berlekampMassey(const std::vector<bm64>& sequence) {
    std::vector<bm64> current{1};
    std::vector<bm64> previous{1};
    int length = 0;
    int shift = 1;
    bm64 previousDiscrepancy = 1;

    for (int index = 0; index < static_cast<int>(sequence.size()); ++index) {
        bm64 discrepancy = 0;
        for (int offset = 0; offset <= length; ++offset) {
            discrepancy = (discrepancy
                + multiplyBm(current[offset], sequence[index - offset])) % BM_MOD;
        }
        if (discrepancy == 0) {
            ++shift;
            continue;
        }

        std::vector<bm64> saved = current;
        const bm64 factor = multiplyBm(discrepancy, powerBm(previousDiscrepancy, BM_MOD - 2));
        if (current.size() < previous.size() + static_cast<std::size_t>(shift)) {
            current.resize(previous.size() + static_cast<std::size_t>(shift), 0);
        }
        for (std::size_t j = 0; j < previous.size(); ++j) {
            current[j + static_cast<std::size_t>(shift)] = normalizeBm(
                current[j + static_cast<std::size_t>(shift)] - multiplyBm(factor, previous[j])
            );
        }
        if (2 * length <= index) {
            length = index + 1 - length;
            previous = saved;
            previousDiscrepancy = discrepancy;
            shift = 1;
        } else {
            ++shift;
        }
    }

    std::vector<bm64> coefficients(length, 0);
    for (int index = 1; index <= length; ++index) {
        coefficients[index - 1] = BM_WRONG_SIGN
            ? normalizeBm(current[index])
            : normalizeBm(-current[index]);
    }
    return coefficients;
}

std::vector<bm64> combineBm(
    const std::vector<bm64>& left,
    const std::vector<bm64>& right,
    const std::vector<bm64>& coefficients
) {
    const int order = static_cast<int>(coefficients.size());
    std::vector<bm64> product(2 * order - 1, 0);
    for (int i = 0; i < order; ++i) {
        for (int j = 0; j < order; ++j) {
            product[i + j] = (product[i + j] + multiplyBm(left[i], right[j])) % BM_MOD;
        }
    }
    for (int degree = 2 * order - 2; degree >= order; --degree) {
        for (int offset = 1; offset <= order; ++offset) {
            const int coefficientIndex = BM_REVERSED_REDUCTION
                ? order - offset
                : offset - 1;
            product[degree - offset] = (product[degree - offset]
                + multiplyBm(product[degree], coefficients[coefficientIndex])) % BM_MOD;
        }
    }
    product.resize(order);
    return product;
}

bm64 predictBm(const std::vector<bm64>& sequence, std::uint64_t target) {
    if (target < sequence.size()) return sequence[static_cast<std::size_t>(target)];
    const std::vector<bm64> coefficients = berlekampMassey(sequence);
    const int order = static_cast<int>(coefficients.size());
    if (order == 0) return 0;
    std::vector<bm64> result(order, 0);
    std::vector<bm64> base(order, 0);
    result[0] = 1;
    if (order == 1) base[0] = coefficients[0];
    else base[1] = 1;

    while (target > 0) {
        if (target & 1) result = combineBm(result, base, coefficients);
        base = combineBm(base, base, coefficients);
        target >>= 1;
    }

    bm64 answer = 0;
    for (int index = 0; index < order; ++index) {
        answer = (answer + multiplyBm(result[index], sequence[index])) % BM_MOD;
    }
    return answer;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int given;
        std::uint64_t target;
        std::cin >> operation >> given >> target;
        std::vector<bm64> sequence(given, 0);
        for (bm64& value : sequence) std::cin >> value;
        std::cout << predictBm(sequence, target) << '\n';
    }
    return 0;
}
