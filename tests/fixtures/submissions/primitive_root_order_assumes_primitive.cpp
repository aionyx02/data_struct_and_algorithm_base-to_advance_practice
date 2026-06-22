// Known-wrong fixture for M08: order assumes a is always primitive.
// Multiplicative order, primitive-root search, and primitive-root counting
// modulo a prime p.
#include <iostream>
#include <string>
#include <vector>

using ll = long long;
using i128 = __int128_t;

ll powmod(ll a, ll b, ll m) {
    ll r = 1 % m;
    a %= m;
    if (a < 0) a += m;
    while (b) {
        if (b & 1) r = static_cast<ll>(static_cast<i128>(r) * a % m);
        a = static_cast<ll>(static_cast<i128>(a) * a % m);
        b >>= 1;
    }
    return r;
}

std::vector<ll> primeFactors(ll n) {
    std::vector<ll> f;
    for (ll p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            f.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) f.push_back(n);
    return f;
}

ll order(ll a, ll p) { (void)a; return p - 1; }

bool isPrimitive(ll a, ll p) {
    if (p == 2) return a % p == 1;
    for (ll q : primeFactors(p - 1)) {
        if (powmod(a, (p - 1) / q, p) == 1) return false;
    }
    return true;
}

ll primitiveRoot(ll p) {
    if (p == 2) return 1;
    for (ll g = 2; g < p; ++g) {
        if (isPrimitive(g, p)) return g;
    }
    return -1;
}

ll phi(ll n) {
    ll r = n;
    for (ll p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            r -= r / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) r -= r / n;
    return r;
}

int main() {
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "order") {
            ll a, p;
            std::cin >> a >> p;
            std::cout << order(a, p) << '\n';
        } else if (op == "primitive_root") {
            ll p;
            std::cin >> p;
            std::cout << primitiveRoot(p) << '\n';
        } else if (op == "num_primitive_roots") {
            ll p;
            std::cin >> p;
            std::cout << phi(p - 1) << '\n';
        } else if (op == "is_primitive") {
            ll a, p;
            std::cin >> a >> p;
            std::cout << (isPrimitive(a, p) ? "yes" : "no") << '\n';
        }
    }
    return 0;
}
