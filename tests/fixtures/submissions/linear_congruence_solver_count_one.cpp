// Known-wrong fixture for M02: always reports a single solution, ignoring that
// a*x = b (mod m) has gcd(a, m) solutions.
#include <iostream>
#include <string>

using ll = long long;
using i128 = __int128_t;

ll gcd2(ll a, ll b) {
    a = a < 0 ? -a : a;
    b = b < 0 ? -b : b;
    while (b != 0) {
        ll t = a % b;
        a = b;
        b = t;
    }
    return a;
}

ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll inv(ll a, ll m) {
    if (m == 1) return 0;
    ll x, y;
    exgcd(((a % m) + m) % m, m, x, y);
    return ((x % m) + m) % m;
}

int main() {
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "solve") {
            ll a, b, m;
            std::cin >> a >> b >> m;
            ll g = gcd2(a, m);
            ll bb = ((b % m) + m) % m;
            if (bb % g != 0) {
                std::cout << "NO SOLUTION" << '\n';
            } else {
                ll mp = m / g;
                ll ap = ((a / g) % mp + mp) % mp;
                ll bp = (bb / g) % mp;
                ll x0 = static_cast<ll>(static_cast<i128>(bp) * inv(ap, mp) % mp);
                x0 = ((x0 % mp) + mp) % mp;
                // BUG: hard-codes the solution count to 1.
                std::cout << 1 << ' ' << x0 << '\n';
            }
        } else if (op == "crt") {
            ll r1, m1, r2, m2;
            std::cin >> r1 >> m1 >> r2 >> m2;
            ll g = gcd2(m1, m2);
            ll diff = r2 - r1;
            if (diff % g != 0) {
                std::cout << "NO SOLUTION" << '\n';
            } else {
                ll lcm = m1 / g * m2;
                ll m2g = m2 / g;
                ll factor = ((diff / g) % m2g + m2g) % m2g;
                ll t = static_cast<ll>(
                    static_cast<i128>(factor) * inv((m1 / g) % m2g, m2g) % m2g);
                t = ((t % m2g) + m2g) % m2g;
                ll x = static_cast<ll>((static_cast<i128>(m1) * t + r1) % lcm);
                x = ((x % lcm) + lcm) % lcm;
                std::cout << x << ' ' << lcm << '\n';
            }
        }
    }
    return 0;
}
