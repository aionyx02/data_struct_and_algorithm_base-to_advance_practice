#include <iostream>
#include <string>
#include <vector>

#ifndef COMPOSITION_SHIFTED_POWER
#define COMPOSITION_SHIFTED_POWER 0
#endif
#ifndef COMPOSITION_DROPS_TARGET_DEGREE
#define COMPOSITION_DROPS_TARGET_DEGREE 0
#endif

using compose64 = long long;
constexpr compose64 COMPOSE_MOD = 1'000'000'007;

std::vector<compose64> composeMultiply(
    const std::vector<compose64>& left,
    const std::vector<compose64>& right,
    int target
) {
    std::vector<compose64> product(target + 1, 0);
    for (int i = 0; i < static_cast<int>(left.size()) && i <= target; ++i) {
        for (int j = 0; j < static_cast<int>(right.size()); ++j) {
            int degree = i + j;
            bool inRange = COMPOSITION_DROPS_TARGET_DEGREE
                ? degree < target
                : degree <= target;
            if (!inRange) break;
            product[degree] = (product[degree] + left[i] * right[j]) % COMPOSE_MOD;
        }
    }
    return product;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int degree, target;
        std::cin >> operation >> degree >> target;
        std::vector<compose64> outer(degree + 1);
        std::vector<compose64> inner(degree + 1);
        for (compose64& value : outer) std::cin >> value;
        for (compose64& value : inner) std::cin >> value;

        std::vector<compose64> power(target + 1, 0);
        power[0] = 1;
        if (COMPOSITION_SHIFTED_POWER) power = composeMultiply(power, inner, target);
        compose64 answer = 0;
        for (int i = 0; i <= degree; ++i) {
            answer = (answer + outer[i] * power[target]) % COMPOSE_MOD;
            power = composeMultiply(power, inner, target);
        }
        std::cout << answer << '\n';
    }
    return 0;
}
