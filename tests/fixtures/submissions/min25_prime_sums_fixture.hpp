#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifndef MIN25_INCLUDE_ONE
#define MIN25_INCLUDE_ONE 0
#endif
#ifndef MIN25_SKIP_PRIME_SQUARE
#define MIN25_SKIP_PRIME_SQUARE 0
#endif

using i64 = long long;
using i128 = __int128_t;

constexpr i64 MOD = 1'000'000'007;

struct PrimeSummary {
    i64 n;
    i64 root;
    std::vector<i64> values;
    std::vector<i64> counts;
    std::vector<i64> sums;
    std::vector<int> smallId;
    std::vector<int> largeId;

    explicit PrimeSummary(i64 limit) : n(limit) {
        root = static_cast<i64>(std::sqrt(static_cast<long double>(n)));
        while (static_cast<i128>(root + 1) * (root + 1) <= n) ++root;
        while (static_cast<i128>(root) * root > n) --root;
        smallId.assign(root + 1, -1);
        largeId.assign(root + 1, -1);

        for (i64 left = 1; left <= n;) {
            i64 value = n / left;
            i64 right = n / value;
            int id = static_cast<int>(values.size());
            values.push_back(value);
            counts.push_back(value - (MIN25_INCLUDE_ONE ? 0 : 1));
            i64 triangular = static_cast<i64>(
                static_cast<i128>(value) * (value + 1) / 2 % MOD
            );
            sums.push_back((triangular - (MIN25_INCLUDE_ONE ? 0 : 1) + MOD) % MOD);
            if (value <= root) smallId[value] = id;
            else largeId[n / value] = id;
            left = right + 1;
        }

        std::vector<bool> composite(root + 1, false);
        i64 primeCountBefore = 0;
        i64 primeSumBefore = 0;
        for (i64 p = 2; p <= root; ++p) {
            if (composite[p]) continue;
            if (static_cast<i128>(p) * p <= root) {
                for (i64 multiple = p * p; multiple <= root; multiple += p) {
                    composite[multiple] = true;
                }
            }
            i64 square = p * p;
            for (std::size_t i = 0; i < values.size(); ++i) {
                bool update = MIN25_SKIP_PRIME_SQUARE
                    ? values[i] > square
                    : values[i] >= square;
                if (!update) break;
                int dividedId = idOf(values[i] / p);
                counts[i] -= counts[dividedId] - primeCountBefore;
                i64 delta = (sums[dividedId] - primeSumBefore + MOD) % MOD;
                sums[i] = (sums[i] - (p % MOD) * delta) % MOD;
                if (sums[i] < 0) sums[i] += MOD;
            }
            ++primeCountBefore;
            primeSumBefore = (primeSumBefore + p) % MOD;
        }
    }

    int idOf(i64 value) const {
        return value <= root ? smallId[value] : largeId[n / value];
    }

    std::pair<i64, i64> answer() const {
        if (n < 2) return {counts[0], sums[0]};
        return {counts[0], sums[0]};
    }
};

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    std::unordered_map<i64, std::pair<i64, i64>> cache;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        i64 n;
        std::cin >> operation >> n;
        auto found = cache.find(n);
        if (found == cache.end()) {
            found = cache.emplace(n, PrimeSummary(n).answer()).first;
        }
        std::cout << (operation == "pi" ? found->second.first : found->second.second)
                  << '\n';
    }
    return 0;
}
