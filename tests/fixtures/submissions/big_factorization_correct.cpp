// Reference solution for M05-big-factorization.
// Deterministic Miller-Rabin primality plus Pollard's Rho factorization for
// n up to 1e18.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using u64 = unsigned long long;
using u128 = __uint128_t;

u64 mulmod(u64 a, u64 b, u64 m) { return static_cast<u64>(static_cast<u128>(a) * b % m); }

u64 powmod(u64 a, u64 b, u64 m) {
    u64 r = 1 % m;
    a %= m;
    while (b) {
        if (b & 1) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        b >>= 1;
    }
    return r;
}

bool isPrime(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (u64 a : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        u64 x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < s - 1; ++i) {
            x = mulmod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

u64 gcdu(u64 a, u64 b) {
    while (b) { u64 t = a % b; a = b; b = t; }
    return a;
}

u64 pollardRho(u64 n) {
    if (n % 2 == 0) return 2;
    for (u64 c = 1;; ++c) {
        u64 x = 2, y = 2, d = 1;
        auto f = [&](u64 v) { return static_cast<u64>((static_cast<u128>(v) * v + c) % n); };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            u64 diff = x > y ? x - y : y - x;
            d = gcdu(diff, n);
        }
        if (d != n) return d;
    }
}

void factor(u64 n, std::vector<u64>& out) {
    if (n == 1) return;
    if (isPrime(n)) { out.push_back(n); return; }
    u64 d = pollardRho(n);
    factor(d, out);
    factor(n / d, out);
}

int main() {
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        u64 n;
        std::cin >> n;
        if (op == "is_prime") {
            std::cout << (isPrime(n) ? "yes" : "no") << '\n';
        } else {
            std::vector<u64> f;
            factor(n, f);
            if (op == "max_factor") {
                if (f.empty()) std::cout << "NONE" << '\n';
                else std::cout << *std::max_element(f.begin(), f.end()) << '\n';
            } else if (op == "num_factors") {
                std::sort(f.begin(), f.end());
                f.erase(std::unique(f.begin(), f.end()), f.end());
                std::cout << f.size() << '\n';
            } else if (op == "factor_count") {
                std::cout << f.size() << '\n';
            }
        }
    }
    return 0;
}
