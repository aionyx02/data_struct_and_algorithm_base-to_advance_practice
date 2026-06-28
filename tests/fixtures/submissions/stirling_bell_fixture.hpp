#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#ifndef STIRLING_WRONG_FIRST_COEFFICIENT
#define STIRLING_WRONG_FIRST_COEFFICIENT 0
#endif
#ifndef STIRLING_MISSING_EMPTY_BASE
#define STIRLING_MISSING_EMPTY_BASE 0
#endif

constexpr int STIRLING_MOD = 1'000'000'007;

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; int n; int k; };
    std::vector<Query> queries;
    queries.reserve(queryCount);
    int maximum = 0;
    for (int query = 0; query < queryCount; ++query) {
        Query current;
        std::cin >> current.operation >> current.n;
        current.k = 0;
        if (current.operation != "bell") std::cin >> current.k;
        maximum = std::max(maximum, current.n);
        queries.push_back(current);
    }

    int width = maximum + 1;
    std::vector<int> first(static_cast<std::size_t>(width) * width, 0);
    std::vector<int> second(static_cast<std::size_t>(width) * width, 0);
    auto at = [width](std::vector<int>& table, int n, int k) -> int& {
        return table[static_cast<std::size_t>(n) * width + k];
    };
    if (!STIRLING_MISSING_EMPTY_BASE) {
        at(first, 0, 0) = 1;
        at(second, 0, 0) = 1;
    }
    for (int n = 1; n <= maximum; ++n) {
        for (int k = 1; k <= n; ++k) {
            long long firstCoefficient = STIRLING_WRONG_FIRST_COEFFICIENT ? k : n - 1;
            at(first, n, k) = static_cast<int>((at(first, n - 1, k - 1)
                + firstCoefficient * at(first, n - 1, k)) % STIRLING_MOD);
            at(second, n, k) = static_cast<int>((at(second, n - 1, k - 1)
                + static_cast<long long>(k) * at(second, n - 1, k)) % STIRLING_MOD);
        }
    }

    for (const Query& query : queries) {
        if (query.operation == "stirling1") {
            std::cout << (query.k <= query.n ? at(first, query.n, query.k) : 0) << '\n';
        } else if (query.operation == "stirling2") {
            std::cout << (query.k <= query.n ? at(second, query.n, query.k) : 0) << '\n';
        } else {
            long long bell = 0;
            for (int k = 0; k <= query.n; ++k) bell += at(second, query.n, k);
            std::cout << bell % STIRLING_MOD << '\n';
        }
    }
    return 0;
}
