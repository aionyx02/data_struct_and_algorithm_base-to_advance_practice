#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef NEWTON_MISSING_ALTERNATION
#define NEWTON_MISSING_ALTERNATION 0
#endif
#ifndef NEWTON_INTEGER_DIVISION
#define NEWTON_INTEGER_DIVISION 0
#endif

using newton64 = long long;
constexpr newton64 NEWTON_MOD = 1'000'000'007;

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; int degree; std::vector<newton64> sums; };
    std::vector<Query> queries;
    queries.reserve(queryCount);
    int maximum = 0;
    for (int query = 0; query < queryCount; ++query) {
        Query current;
        std::cin >> current.operation >> current.degree;
        current.sums.assign(current.degree + 1, 0);
        for (int i = 1; i <= current.degree; ++i) std::cin >> current.sums[i];
        maximum = std::max(maximum, current.degree);
        queries.push_back(std::move(current));
    }
    std::vector<newton64> inverse(maximum + 1, 0);
    if (maximum >= 1) inverse[1] = 1;
    for (int i = 2; i <= maximum; ++i) {
        inverse[i] = NEWTON_MOD - (NEWTON_MOD / i) * inverse[NEWTON_MOD % i]
            % NEWTON_MOD;
    }

    for (const Query& query : queries) {
        std::vector<newton64> symmetric(query.degree + 1, 0);
        symmetric[0] = 1;
        for (int degree = 1; degree <= query.degree; ++degree) {
            newton64 sum = 0;
            for (int i = 1; i <= degree; ++i) {
                newton64 term = symmetric[degree - i] * query.sums[i] % NEWTON_MOD;
                bool negative = query.operation == "elementary"
                    && !NEWTON_MISSING_ALTERNATION && (i % 2 == 0);
                sum = (sum + (negative ? NEWTON_MOD - term : term)) % NEWTON_MOD;
            }
            symmetric[degree] = NEWTON_INTEGER_DIVISION
                ? sum / degree
                : sum * inverse[degree] % NEWTON_MOD;
        }
        std::cout << symmetric[query.degree] << '\n';
    }
    return 0;
}
