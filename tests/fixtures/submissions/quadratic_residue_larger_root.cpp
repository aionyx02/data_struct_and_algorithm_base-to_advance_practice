// Known-wrong fixture for M09: sqrt returns the larger root, not the smaller.
// Legendre symbol, quadratic-residue test, and modular square root via
// Tonelli-Shanks, modulo an odd prime p.
#include <iostream>
#include <string>
#include <algorithm>

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

// A square root of n modulo odd prime p, or -1 if n is a non-residue.
ll tonelli(ll n, ll p) {
    n %= p;
    if (n == 0) return 0;
    if (powmod(n, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return powmod(n, (p + 1) / 4, p);
    ll q = p - 1, s = 0;
    while (q % 2 == 0) { q /= 2; ++s; }
    ll z = 2;
    while (powmod(z, (p - 1) / 2, p) != p - 1) ++z;
    ll m = s;
    ll c = powmod(z, q, p);
    ll t = powmod(n, q, p);
    ll r = powmod(n, (q + 1) / 2, p);
    while (t != 1) {
        ll i = 0, t2 = t;
        while (t2 != 1) { t2 = static_cast<ll>(static_cast<i128>(t2) * t2 % p); ++i; }
        ll b = c;
        for (ll j = 0; j < m - i - 1; ++j) b = static_cast<ll>(static_cast<i128>(b) * b % p);
        m = i;
        c = static_cast<ll>(static_cast<i128>(b) * b % p);
        t = static_cast<ll>(static_cast<i128>(t) * c % p);
        r = static_cast<ll>(static_cast<i128>(r) * b % p);
    }
    return r;
}

int main() {
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        ll a, p;
        std::cin >> a >> p;
        if (op == "legendre") {
            ll v = a % p;
            if (v < 0) v += p;
            if (v == 0) std::cout << 0 << '\n';
            else std::cout << (powmod(v, (p - 1) / 2, p) == 1 ? 1 : -1) << '\n';
        } else if (op == "is_qr") {
            ll v = a % p;
            if (v < 0) v += p;
            std::cout << ((v == 0 || powmod(v, (p - 1) / 2, p) == 1) ? "yes" : "no")
                      << '\n';
        } else if (op == "sqrt") {
            ll r = tonelli(a, p);
            if (r < 0) std::cout << "NONE" << '\n';
            else std::cout << std::max(r, p - r) << '\n';
        }
    }
    return 0;
}
