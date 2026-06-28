#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef INCLUSION_WRONG_SIGN
#define INCLUSION_WRONG_SIGN 0
#endif
#ifndef INCLUSION_MISSING_EMPTY
#define INCLUSION_MISSING_EMPTY 0
#endif

using inc64 = long long;
using inc128 = __int128_t;
constexpr inc64 INC_MOD = 1'000'000'007;

inc64 incPower(inc64 base, std::uint64_t exponent) {
    if (INCLUSION_MISSING_EMPTY && base == 0 && exponent == 0) return 0;
    inc64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<inc64>(static_cast<inc128>(result) * base % INC_MOD);
        base = static_cast<inc64>(static_cast<inc128>(base) * base % INC_MOD);
        exponent >>= 1;
    }
    return result;
}

struct Combinations {
    std::vector<inc64> factorial;
    std::vector<inc64> inverseFactorial;

    explicit Combinations(int limit) : factorial(limit + 1), inverseFactorial(limit + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= limit; ++i) factorial[i] = factorial[i - 1] * i % INC_MOD;
        inverseFactorial[limit] = incPower(factorial[limit], INC_MOD - 2);
        for (int i = limit; i > 0; --i) inverseFactorial[i - 1] = inverseFactorial[i] * i % INC_MOD;
    }

    inc64 choose(int n, int k) const {
        return factorial[n] * inverseFactorial[k] % INC_MOD
            * inverseFactorial[n - k] % INC_MOD;
    }
};

inc64 onto(std::uint64_t n, int k, const Combinations& combinations) {
    if (INCLUSION_MISSING_EMPTY && n == 0 && k == 0) return 0;
    inc64 answer = 0;
    for (int excluded = 0; excluded <= k; ++excluded) {
        inc64 term = combinations.choose(k, excluded)
            * incPower(k - excluded, n) % INC_MOD;
        bool negative = INCLUSION_WRONG_SIGN
            ? ((k - excluded) & 1)
            : (excluded & 1);
        answer = (answer + (negative ? INC_MOD - term : term)) % INC_MOD;
    }
    return answer;
}

inc64 derangement(int n, const Combinations& combinations) {
    if (INCLUSION_MISSING_EMPTY && n == 0) return 0;
    inc64 series = 0;
    for (int i = 0; i <= n; ++i) {
        inc64 term = combinations.inverseFactorial[i];
        bool negative = INCLUSION_WRONG_SIGN ? ((n - i) & 1) : (i & 1);
        series = (series + (negative ? INC_MOD - term : term)) % INC_MOD;
    }
    return combinations.factorial[n] * series % INC_MOD;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query {
        std::string operation;
        std::uint64_t n;
        int k;
    };
    std::vector<Query> queries;
    queries.reserve(queryCount);
    int limit = 0;
    for (int query = 0; query < queryCount; ++query) {
        Query current;
        std::cin >> current.operation >> current.n;
        if (current.operation == "onto") {
            std::cin >> current.k;
            limit = std::max(limit, current.k);
        } else {
            current.k = 0;
            limit = std::max(limit, static_cast<int>(current.n));
        }
        queries.push_back(current);
    }
    Combinations combinations(limit);
    for (const Query& query : queries) {
        if (query.operation == "onto") {
            std::cout << onto(query.n, query.k, combinations) << '\n';
        } else {
            std::cout << derangement(static_cast<int>(query.n), combinations) << '\n';
        }
    }
    return 0;
}
