// Known-wrong fixture for M11: when p divides i the sieve still doubles the
// divisor count as if p were a brand-new prime, instead of advancing the
// smallest-prime exponent. So d() is wrong on every non-squarefree number.
#include <iostream>
#include <string>
#include <vector>

const int LIMIT = 1000000;
int spf[LIMIT + 1];
int mu[LIMIT + 1];
int phi[LIMIT + 1];
int numDiv[LIMIT + 1];
int expSmall[LIMIT + 1];
long long sigma[LIMIT + 1];
long long gSmall[LIMIT + 1];

void sieve() {
    std::vector<int> primes;
    mu[1] = phi[1] = numDiv[1] = 1;
    sigma[1] = 1;
    gSmall[1] = 1;
    expSmall[1] = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            mu[i] = -1;
            phi[i] = i - 1;
            numDiv[i] = 2;
            expSmall[i] = 1;
            sigma[i] = static_cast<long long>(i) + 1;
            gSmall[i] = static_cast<long long>(i) + 1;
        }
        for (int p : primes) {
            long long v = static_cast<long long>(i) * p;
            if (v > LIMIT) break;
            int vv = static_cast<int>(v);
            spf[vv] = p;
            if (i % p == 0) {
                mu[vv] = 0;
                phi[vv] = phi[i] * p;
                expSmall[vv] = expSmall[i] + 1;
                numDiv[vv] = numDiv[i] * 2;  // BUG: should be numDiv[i]/(e+1)*(e+2).
                gSmall[vv] = gSmall[i] * p + 1;
                sigma[vv] = sigma[i] / gSmall[i] * gSmall[vv];
                break;
            }
            mu[vv] = -mu[i];
            phi[vv] = phi[i] * (p - 1);
            expSmall[vv] = 1;
            numDiv[vv] = numDiv[i] * 2;
            gSmall[vv] = static_cast<long long>(p) + 1;
            sigma[vv] = sigma[i] * (static_cast<long long>(p) + 1);
        }
    }
}

int main() {
    sieve();
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        long long x;
        std::cin >> x;
        if (op == "phi") {
            std::cout << phi[x] << '\n';
        } else if (op == "mu") {
            std::cout << mu[x] << '\n';
        } else if (op == "d") {
            std::cout << numDiv[x] << '\n';
        } else if (op == "sigma") {
            std::cout << sigma[x] << '\n';
        }
    }
    return 0;
}
