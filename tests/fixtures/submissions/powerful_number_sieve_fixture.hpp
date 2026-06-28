#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef POWERFUL_MISSING_ONE
#define POWERFUL_MISSING_ONE 0
#endif
#ifndef POWERFUL_ALLOW_NONSQUAREFREE
#define POWERFUL_ALLOW_NONSQUAREFREE 0
#endif

using u64 = std::uint64_t;
using u128 = __uint128_t;

constexpr u64 POWERFUL_MOD = 1'000'000'007;
constexpr u64 INV6 = 166'666'668;

u64 floorSqrt(u64 n) {
    u64 root = static_cast<u64>(std::sqrt(static_cast<long double>(n)));
    while (static_cast<u128>(root + 1) * (root + 1) <= n) ++root;
    while (static_cast<u128>(root) * root > n) --root;
    return root;
}

u64 floorCubeRoot(u64 n) {
    u64 root = static_cast<u64>(std::cbrt(static_cast<long double>(n)));
    while (static_cast<u128>(root + 1) * (root + 1) * (root + 1) <= n) ++root;
    while (static_cast<u128>(root) * root * root > n) --root;
    return root;
}

std::pair<u64, u64> summarize(u64 n, const std::vector<bool>& squareFree) {
    u64 count = 0;
    u64 sum = 0;
    u64 limit = floorCubeRoot(n);
    for (u64 b = 1; b <= limit; ++b) {
        if (!POWERFUL_ALLOW_NONSQUAREFREE && !squareFree[b]) continue;
        u64 cube = static_cast<u64>(static_cast<u128>(b) * b * b);
        u64 a = floorSqrt(n / cube);
        count += a;
        u64 squareSum = (a % POWERFUL_MOD) * ((a + 1) % POWERFUL_MOD)
            % POWERFUL_MOD;
        squareSum = squareSum * ((2 * (a % POWERFUL_MOD) + 1) % POWERFUL_MOD)
            % POWERFUL_MOD * INV6 % POWERFUL_MOD;
        sum = (sum + (cube % POWERFUL_MOD) * squareSum) % POWERFUL_MOD;
    }
    if (POWERFUL_MISSING_ONE) {
        --count;
        sum = (sum + POWERFUL_MOD - 1) % POWERFUL_MOD;
    }
    return {count, sum};
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; u64 n; };
    std::vector<Query> queries(queryCount);
    u64 maximum = 1;
    for (Query& query : queries) {
        std::cin >> query.operation >> query.n;
        if (query.n > maximum) maximum = query.n;
    }

    u64 limit = floorCubeRoot(maximum);
    std::vector<bool> squareFree(limit + 1, true);
    if (!squareFree.empty()) squareFree[0] = false;
    for (u64 p = 2; p * p <= limit; ++p) {
        u64 square = p * p;
        for (u64 multiple = square; multiple <= limit; multiple += square) {
            squareFree[multiple] = false;
        }
    }

    for (const Query& query : queries) {
        auto [count, sum] = summarize(query.n, squareFree);
        std::cout << (query.operation == "count" ? count : sum) << '\n';
    }
    return 0;
}
