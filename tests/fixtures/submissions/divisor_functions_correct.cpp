// Reference solution for M04-divisor-functions.
// Factorizes n by trial division and reports d(n), sigma(n), and phi(n).
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

void factor(ll n, std::vector<ll>& primes, std::vector<int>& exps) {
    for (ll p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            primes.push_back(p);
            exps.push_back(e);
        }
    }
    if (n > 1) {
        primes.push_back(n);
        exps.push_back(1);
    }
}

int main() {
    int q;
    if (!(std::cin >> q)) return 0;
    std::string op;
    for (int i = 0; i < q; ++i) {
        if (!(std::cin >> op)) break;
        ll n;
        std::cin >> n;
        std::vector<ll> primes;
        std::vector<int> exps;
        factor(n, primes, exps);
        if (op == "factorize") {
            if (n == 1 || primes.empty()) {
                std::cout << 1 << '\n';
            } else {
                for (std::size_t j = 0; j < primes.size(); ++j) {
                    if (j != 0) std::cout << ' ';
                    std::cout << primes[j] << '^' << exps[j];
                }
                std::cout << '\n';
            }
        } else if (op == "divisors") {
            ll d = 1;
            for (int e : exps) d *= (e + 1);
            std::cout << d << '\n';
        } else if (op == "sigma") {
            ll s = 1;
            for (std::size_t j = 0; j < primes.size(); ++j) {
                ll term = 1;
                ll power = 1;
                for (int k = 0; k < exps[j]; ++k) {
                    power *= primes[j];
                    term += power;
                }
                s *= term;
            }
            std::cout << s << '\n';
        } else if (op == "phi") {
            ll phi = n;
            for (ll p : primes) phi = phi / p * (p - 1);
            std::cout << phi << '\n';
        }
    }
    return 0;
}
