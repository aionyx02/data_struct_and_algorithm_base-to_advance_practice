// Known-wrong fixture for M06: linear inverse recurrence drops the negation.
// Precomputes linear inverses, factorials, and inverse factorials modulo
// p = 1e9+7, then answers inverse, factorial, and binomial queries.
#include <iostream>
#include <string>

using ll = long long;

const int N = 1000000;
const ll P = 1000000007;
ll fact[N + 1];
ll invfact[N + 1];
ll inv[N + 1];

void init() {
    inv[1] = 1;
    for (int i = 2; i <= N; ++i) {
        inv[i] = (P / i) * inv[P % i] % P;
    }
    fact[0] = 1;
    for (int i = 1; i <= N; ++i) fact[i] = fact[i - 1] * i % P;
    invfact[0] = 1;
    for (int i = 1; i <= N; ++i) invfact[i] = invfact[i - 1] * inv[i] % P;
}

ll binom(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % P * invfact[n - r] % P;
}

int main() {
    init();
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "inv") {
            ll x;
            std::cin >> x;
            std::cout << inv[x] << '\n';
        } else if (op == "fact") {
            ll n;
            std::cin >> n;
            std::cout << fact[n] << '\n';
        } else if (op == "inv_fact") {
            ll n;
            std::cin >> n;
            std::cout << invfact[n] << '\n';
        } else if (op == "binom") {
            ll n, r;
            std::cin >> n >> r;
            std::cout << binom(n, r) << '\n';
        }
    }
    return 0;
}
