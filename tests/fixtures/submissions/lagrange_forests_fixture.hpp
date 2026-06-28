#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef LAGRANGE_OMIT_COMPONENT_FACTOR
#define LAGRANGE_OMIT_COMPONENT_FACTOR 0
#endif
#ifndef LAGRANGE_WRONG_BINOMIAL_INDEX
#define LAGRANGE_WRONG_BINOMIAL_INDEX 0
#endif

using lag64 = long long;
using lag128 = __int128_t;
constexpr lag64 LAG_MOD = 1'000'000'007;

lag64 lagPower(lag64 base, lag64 exponent) {
    lag64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<lag64>(static_cast<lag128>(result) * base % LAG_MOD);
        base = static_cast<lag64>(static_cast<lag128>(base) * base % LAG_MOD);
        exponent >>= 1;
    }
    return result;
}

struct LagrangeCombinations {
    std::vector<lag64> factorial;
    std::vector<lag64> inverseFactorial;

    explicit LagrangeCombinations(int limit)
        : factorial(limit + 1), inverseFactorial(limit + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= limit; ++i) factorial[i] = factorial[i - 1] * i % LAG_MOD;
        inverseFactorial[limit] = lagPower(factorial[limit], LAG_MOD - 2);
        for (int i = limit; i > 0; --i) inverseFactorial[i - 1] = inverseFactorial[i] * i % LAG_MOD;
    }

    lag64 choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return factorial[n] * inverseFactorial[k] % LAG_MOD
            * inverseFactorial[n - k] % LAG_MOD;
    }
};

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { int arity; int nodes; int components; };
    std::vector<Query> queries(queryCount);
    int limit = 0;
    for (Query& query : queries) {
        std::string operation;
        std::cin >> operation >> query.arity >> query.nodes >> query.components;
        limit = std::max(limit, query.arity * query.nodes);
    }
    LagrangeCombinations combinations(limit);
    for (const Query& query : queries) {
        int selected = LAGRANGE_WRONG_BINOMIAL_INDEX
            ? query.nodes
            : query.nodes - query.components;
        lag64 answer = combinations.choose(query.arity * query.nodes, selected);
        if (!LAGRANGE_OMIT_COMPONENT_FACTOR) answer = answer * query.components % LAG_MOD;
        answer = answer * lagPower(query.nodes, LAG_MOD - 2) % LAG_MOD;
        std::cout << answer << '\n';
    }
    return 0;
}
