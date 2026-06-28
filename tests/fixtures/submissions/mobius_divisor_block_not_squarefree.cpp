// Known-wrong fixture for M10: when p divides i the sieve treats the multiple as
// still squarefree (mu = -mu[i]) instead of zeroing it, so mu and its prefix
// sums are wrong on every non-squarefree number.
#include <iostream>
#include <string>
#include <vector>

const int LIMIT = 1000000;
int mu[LIMIT + 1];
long long muPrefix[LIMIT + 1];
int spf[LIMIT + 1];

void sieve() {
    std::vector<int> primes;
    mu[1] = 1;
    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            mu[i] = -1;
        }
        for (int p : primes) {
            long long v = static_cast<long long>(i) * p;
            if (v > LIMIT) break;
            spf[v] = p;
            if (i % p == 0) {
                mu[v] = -mu[i];  // BUG: should be 0 (v is not squarefree).
                break;
            }
            mu[v] = -mu[i];
        }
    }
    for (int i = 1; i <= LIMIT; ++i) muPrefix[i] = muPrefix[i - 1] + mu[i];
}

long long floorSum(long long n) {
    long long sum = 0;
    for (long long l = 1; l <= n;) {
        long long q = n / l;
        long long r = n / q;
        sum += q * (r - l + 1);
        l = r + 1;
    }
    return sum;
}

int main() {
    sieve();
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        if (op == "mu") {
            long long x;
            std::cin >> x;
            std::cout << mu[x] << '\n';
        } else if (op == "mu_prefix") {
            long long m;
            std::cin >> m;
            std::cout << muPrefix[m] << '\n';
        } else if (op == "floor_sum") {
            long long m;
            std::cin >> m;
            std::cout << floorSum(m) << '\n';
        }
    }
    return 0;
}
