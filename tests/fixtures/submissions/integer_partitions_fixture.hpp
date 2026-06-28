#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef PARTITION_WRONG_PENTAGONAL_SIGN
#define PARTITION_WRONG_PENTAGONAL_SIGN 0
#endif
#ifndef PARTITION_DISTINCT_ASCENDING
#define PARTITION_DISTINCT_ASCENDING 0
#endif

constexpr int PARTITION_MOD = 1'000'000'007;

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    struct Query { std::string operation; int n; };
    std::vector<Query> queries(queryCount);
    int maximum = 0;
    for (Query& query : queries) {
        std::cin >> query.operation >> query.n;
        maximum = std::max(maximum, query.n);
    }

    std::vector<int> partition(maximum + 1, 0);
    partition[0] = 1;
    for (int n = 1; n <= maximum; ++n) {
        long long value = 0;
        for (int k = 1;; ++k) {
            int first = k * (3 * k - 1) / 2;
            int second = k * (3 * k + 1) / 2;
            if (first > n) break;
            int sign = (k & 1) ? 1 : -1;
            value += sign * partition[n - first];
            if (second <= n) {
                int secondSign = PARTITION_WRONG_PENTAGONAL_SIGN ? -sign : sign;
                value += secondSign * partition[n - second];
            }
        }
        value %= PARTITION_MOD;
        if (value < 0) value += PARTITION_MOD;
        partition[n] = static_cast<int>(value);
    }

    std::vector<int> distinct(maximum + 1, 0);
    distinct[0] = 1;
    for (int part = 1; part <= maximum; ++part) {
        if (PARTITION_DISTINCT_ASCENDING) {
            for (int sum = part; sum <= maximum; ++sum) {
                distinct[sum] = (distinct[sum] + distinct[sum - part]) % PARTITION_MOD;
            }
        } else {
            for (int sum = maximum; sum >= part; --sum) {
                distinct[sum] = (distinct[sum] + distinct[sum - part]) % PARTITION_MOD;
            }
        }
    }

    for (const Query& query : queries) {
        std::cout << (query.operation == "partition" ? partition[query.n] : distinct[query.n])
                  << '\n';
    }
    return 0;
}
