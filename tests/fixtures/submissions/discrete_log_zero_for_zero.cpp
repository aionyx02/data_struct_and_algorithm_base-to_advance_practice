// Known-wrong fixture for M07: returns 0 for b=0 instead of NONE.
// Baby-step giant-step discrete logarithm modulo a prime p.
#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>

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

// Smallest x >= 0 with a^x === b (mod p) for prime p and 1 <= a <= p-1,
// or -1 if none exists.
ll bsgs(ll a, ll b, ll p) {
    a %= p;
    b %= p;
    if (b == 1 % p || b == 0) return 0;
    if (a == 0) return b == 0 ? 1 : -1;
    ll n = static_cast<ll>(std::ceil(std::sqrt(static_cast<double>(p))));
    std::unordered_map<ll, ll> table;
    ll cur = b;
    for (ll j = 0; j < n; ++j) {
        table[cur] = j;  // keep the largest j so we recover the smallest x
        cur = static_cast<ll>(static_cast<i128>(cur) * a % p);
    }
    ll an = powmod(a, n, p);
    cur = 1;
    for (ll i = 1; i <= n; ++i) {
        cur = static_cast<ll>(static_cast<i128>(cur) * an % p);
        auto it = table.find(cur);
        if (it != table.end()) return i * n - it->second;
    }
    return -1;
}

int main() {
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        ll a, b, p;
        std::cin >> a >> b >> p;
        ll x = bsgs(a, b, p);
        if (x < 0) std::cout << "NONE" << '\n';
        else std::cout << x << '\n';
    }
    return 0;
}
