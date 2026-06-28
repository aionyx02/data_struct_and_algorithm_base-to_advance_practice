#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef EGF_MISSING_CYCLE_ORDER
#define EGF_MISSING_CYCLE_ORDER 0
#endif
#ifndef EGF_WRONG_DISTINGUISHED_CHOICE
#define EGF_WRONG_DISTINGUISHED_CHOICE 0
#endif

using egf64 = long long;
constexpr egf64 EGF_MOD = 1'000'000'007;

egf64 egfPower(egf64 base, egf64 exponent) {
    egf64 result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = result * base % EGF_MOD;
        base = base * base % EGF_MOD;
        exponent >>= 1;
    }
    return result;
}

struct EgfCombinations {
    std::vector<egf64> factorial;
    std::vector<egf64> inverseFactorial;

    explicit EgfCombinations(int limit)
        : factorial(limit + 1), inverseFactorial(limit + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= limit; ++i) factorial[i] = factorial[i - 1] * i % EGF_MOD;
        inverseFactorial[limit] = egfPower(factorial[limit], EGF_MOD - 2);
        for (int i = limit; i > 0; --i) inverseFactorial[i - 1] = inverseFactorial[i] * i % EGF_MOD;
    }

    egf64 choose(int n, int k) const {
        if (k < 0 || k > n) return 0;
        return factorial[n] * inverseFactorial[k] % EGF_MOD
            * inverseFactorial[n - k] % EGF_MOD;
    }
};

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { int n; std::vector<int> allowed; };
    std::vector<Query> queries(queryCount);
    int maximum = 0;
    for (Query& query : queries) {
        std::string operation;
        int count;
        std::cin >> operation >> query.n >> count;
        query.allowed.resize(count);
        for (int& length : query.allowed) std::cin >> length;
        maximum = std::max(maximum, query.n);
    }
    EgfCombinations combinations(maximum);
    for (const Query& query : queries) {
        std::vector<egf64> count(query.n + 1, 0);
        count[0] = 1;
        for (int size = 1; size <= query.n; ++size) {
            for (int length : query.allowed) {
                if (length > size) continue;
                egf64 ways;
                if (EGF_MISSING_CYCLE_ORDER) {
                    ways = combinations.choose(size - 1, length - 1);
                } else if (EGF_WRONG_DISTINGUISHED_CHOICE) {
                    ways = combinations.choose(size, length)
                        * combinations.factorial[length - 1] % EGF_MOD;
                } else {
                    ways = combinations.factorial[size - 1]
                        * combinations.inverseFactorial[size - length] % EGF_MOD;
                }
                count[size] = (count[size] + ways * count[size - length]) % EGF_MOD;
            }
        }
        std::cout << count[query.n] << '\n';
    }
    return 0;
}
