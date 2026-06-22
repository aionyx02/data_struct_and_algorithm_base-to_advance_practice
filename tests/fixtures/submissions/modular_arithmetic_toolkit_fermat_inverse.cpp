// Known-wrong fixture for M01: computes the modular inverse with Fermat's little
// theorem (a^(m-2)), which is only valid when m is prime and never reports NONE.
#include <iostream>
#include <string>

using ll = long long;
using i128 = __int128_t;

ll absll(ll value) { return value < 0 ? -value : value; }

ll gcdll(ll a, ll b) {
    a = absll(a);
    b = absll(b);
    while (b != 0) {
        ll t = a % b;
        a = b;
        b = t;
    }
    return a;
}

ll powmod(ll a, ll b, ll m) {
    if (m == 1) return 0;
    ll result = 1 % m;
    a %= m;
    if (a < 0) a += m;
    while (b > 0) {
        if (b & 1) result = static_cast<ll>(static_cast<i128>(result) * a % m);
        a = static_cast<ll>(static_cast<i128>(a) * a % m);
        b >>= 1;
    }
    return result;
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "gcd") {
            ll a, b;
            std::cin >> a >> b;
            std::cout << gcdll(a, b) << '\n';
        } else if (op == "pow") {
            ll a, b, m;
            std::cin >> a >> b >> m;
            std::cout << powmod(a, b, m) << '\n';
        } else if (op == "inv") {
            ll a, m;
            std::cin >> a >> m;
            // BUG: assumes m is prime, so the inverse is a^(m-2) and always exists.
            std::cout << powmod(a, m - 2, m) << '\n';
        } else if (op == "lcm") {
            ll a, b, m;
            std::cin >> a >> b >> m;
            ll A = absll(a);
            ll B = absll(b);
            if (A == 0 || B == 0) {
                std::cout << 0 << '\n';
            } else {
                ll g = gcdll(A, B);
                std::cout << (A / g) % m * (B % m) % m << '\n';
            }
        }
    }
    return 0;
}
