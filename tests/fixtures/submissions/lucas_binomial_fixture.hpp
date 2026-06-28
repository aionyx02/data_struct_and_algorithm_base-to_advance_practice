#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifndef LUCAS_SINGLE_DIGIT
#define LUCAS_SINGLE_DIGIT 0
#endif
#ifndef LUCAS_SKIP_INVALID_DIGIT
#define LUCAS_SKIP_INVALID_DIGIT 0
#endif

using u64 = std::uint64_t;
using u128 = __uint128_t;

u64 powerMod(u64 base, u64 exponent, u64 mod) {
    u64 result = 1 % mod;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<u64>(static_cast<u128>(result) * base % mod);
        base = static_cast<u64>(static_cast<u128>(base) * base % mod);
        exponent >>= 1;
    }
    return result;
}

struct LucasTable {
    u64 prime;
    std::vector<u64> factorial;
    std::vector<u64> inverseFactorial;

    explicit LucasTable(u64 p) : prime(p), factorial(p), inverseFactorial(p) {
        factorial[0] = 1;
        for (u64 i = 1; i < p; ++i) factorial[i] = factorial[i - 1] * i % p;
        inverseFactorial[p - 1] = powerMod(factorial[p - 1], p - 2, p);
        for (u64 i = p - 1; i > 0; --i) {
            inverseFactorial[i - 1] = inverseFactorial[i] * i % p;
        }
    }

    u64 choose(u64 n, u64 k) const {
        if (k > n) return 0;
        return factorial[n] * inverseFactorial[k] % prime
            * inverseFactorial[n - k] % prime;
    }
};

u64 lucas(u64 n, u64 k, const LucasTable& table) {
    if (k > n) return 0;
    u64 result = 1;
    do {
        u64 ni = n % table.prime;
        u64 ki = k % table.prime;
        if (ki > ni) {
            if (!LUCAS_SKIP_INVALID_DIGIT) return 0;
        } else {
            result = result * table.choose(ni, ki) % table.prime;
        }
        if (LUCAS_SINGLE_DIGIT) return result;
        n /= table.prime;
        k /= table.prime;
    } while (n > 0 || k > 0);
    return result;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    std::unordered_map<u64, LucasTable> tables;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        u64 n, k, prime;
        std::cin >> operation >> n >> k >> prime;
        auto found = tables.find(prime);
        if (found == tables.end()) found = tables.emplace(prime, LucasTable(prime)).first;
        std::cout << lucas(n, k, found->second) << '\n';
    }
    return 0;
}
