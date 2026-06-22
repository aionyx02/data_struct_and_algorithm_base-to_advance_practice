// Known-wrong fixture for M03: treats 1 as prime in is_prime.
// Builds a linear sieve up to 1e6 and answers primality, counting, k-th prime,
// and smallest-prime-factor queries.
#include <iostream>
#include <string>

const int LIMIT = 1000000;
int spf[LIMIT + 1];
int primes[80000];
int primeCount = 0;
int piPrefix[LIMIT + 1];

void sieve() {
    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primeCount++] = i;
        }
        for (int j = 0; j < primeCount; ++j) {
            long long v = static_cast<long long>(i) * primes[j];
            if (v > LIMIT) break;
            spf[v] = primes[j];
            if (primes[j] == spf[i]) break;
        }
    }
    for (int i = 2; i <= LIMIT; ++i) {
        piPrefix[i] = piPrefix[i - 1] + (spf[i] == i ? 1 : 0);
    }
}

int main() {
    sieve();
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "is_prime") {
            long long x;
            std::cin >> x;
            std::cout << ((x == 1 || (x >= 2 && x <= LIMIT && spf[x] == x)) ? "yes" : "no")
                      << '\n';
        } else if (op == "count") {
            long long a, b;
            std::cin >> a >> b;
            if (b < 2 || a > b) {
                std::cout << 0 << '\n';
            } else {
                long long lo = a < 2 ? 2 : a;
                std::cout << piPrefix[b] - piPrefix[lo - 1] << '\n';
            }
        } else if (op == "nth") {
            long long k;
            std::cin >> k;
            if (k < 1 || k > primeCount) {
                std::cout << "NONE" << '\n';
            } else {
                std::cout << primes[k - 1] << '\n';
            }
        } else if (op == "spf") {
            long long x;
            std::cin >> x;
            if (x < 2 || x > LIMIT) {
                std::cout << "NONE" << '\n';
            } else {
                std::cout << spf[x] << '\n';
            }
        }
    }
    return 0;
}
