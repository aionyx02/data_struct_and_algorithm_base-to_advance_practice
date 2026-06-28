#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef CATALAN_INTEGER_DIVISION
#define CATALAN_INTEGER_DIVISION 0
#endif
#ifndef CATALAN_WRONG_REFLECTION
#define CATALAN_WRONG_REFLECTION 0
#endif

using cat64 = long long;
using cat128 = __int128_t;
constexpr cat64 CAT_MOD = 1'000'000'007;

cat64 catPower(cat64 base, cat64 exponent) {
    cat64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<cat64>(static_cast<cat128>(result) * base % CAT_MOD);
        base = static_cast<cat64>(static_cast<cat128>(base) * base % CAT_MOD);
        exponent >>= 1;
    }
    return result;
}

struct CatalanCombinations {
    std::vector<cat64> factorial;
    std::vector<cat64> inverseFactorial;

    explicit CatalanCombinations(int limit)
        : factorial(limit + 1), inverseFactorial(limit + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= limit; ++i) factorial[i] = factorial[i - 1] * i % CAT_MOD;
        inverseFactorial[limit] = catPower(factorial[limit], CAT_MOD - 2);
        for (int i = limit; i > 0; --i) inverseFactorial[i - 1] = inverseFactorial[i] * i % CAT_MOD;
    }

    cat64 choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return factorial[n] * inverseFactorial[k] % CAT_MOD
            * inverseFactorial[n - k] % CAT_MOD;
    }
};

cat64 divideValue(cat64 value, int divisor) {
    if (CATALAN_INTEGER_DIVISION) return value / divisor;
    return value * catPower(divisor, CAT_MOD - 2) % CAT_MOD;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; int a; int b; };
    std::vector<Query> queries;
    queries.reserve(queryCount);
    int limit = 0;
    for (int query = 0; query < queryCount; ++query) {
        Query current;
        std::cin >> current.operation >> current.a;
        current.b = 0;
        if (current.operation != "catalan") std::cin >> current.b;
        if (current.operation == "ballot") limit = std::max(limit, current.a + current.b);
        else limit = std::max(limit, 2 * current.a);
        queries.push_back(current);
    }
    CatalanCombinations combinations(limit);
    for (const Query& query : queries) {
        if (query.operation == "catalan") {
            std::cout << divideValue(combinations.choose(2 * query.a, query.a), query.a + 1)
                      << '\n';
        } else if (query.operation == "narayana") {
            if (query.a == 0) {
                std::cout << (query.b == 0 ? 1 : 0) << '\n';
            } else {
                cat64 value = combinations.choose(query.a, query.b)
                    * combinations.choose(query.a, query.b - 1) % CAT_MOD;
                std::cout << divideValue(value, query.a) << '\n';
            }
        } else {
            if (query.b > query.a) {
                std::cout << 0 << '\n';
                continue;
            }
            int reflected = CATALAN_WRONG_REFLECTION ? query.b + 1 : query.b - 1;
            cat64 answer = (combinations.choose(query.a + query.b, query.b)
                - combinations.choose(query.a + query.b, reflected) + CAT_MOD) % CAT_MOD;
            std::cout << answer << '\n';
        }
    }
    return 0;
}
