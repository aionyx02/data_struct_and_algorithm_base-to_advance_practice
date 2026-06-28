// Reference solution for M12-mobius-inversion-gcd.
// Linear-sieves mu and phi up to 1e6 with prefix sums, then evaluates the two
// double sums by 2D divisor blocking, fully reduced modulo 1e9+7.
#include <iostream>
#include <string>
#include <vector>

const int LIMIT = 1000000;
const long long MOD = 1000000007;
int spf[LIMIT + 1];
int mu[LIMIT + 1];
int phi[LIMIT + 1];
long long muPrefixMod[LIMIT + 1];
long long phiPrefixMod[LIMIT + 1];

void sieve() {
    std::vector<int> primes;
    mu[1] = 1;
    phi[1] = 1;
    for (int i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes.push_back(i);
            mu[i] = -1;
            phi[i] = i - 1;
        }
        for (int p : primes) {
            long long v = static_cast<long long>(i) * p;
            if (v > LIMIT) break;
            int vv = static_cast<int>(v);
            spf[vv] = p;
            if (i % p == 0) {
                mu[vv] = 0;
                phi[vv] = phi[i] * p;
                break;
            }
            mu[vv] = -mu[i];
            phi[vv] = phi[i] * (p - 1);
        }
    }
    for (int i = 1; i <= LIMIT; ++i) {
        muPrefixMod[i] = ((muPrefixMod[i - 1] + mu[i]) % MOD + MOD) % MOD;
        phiPrefixMod[i] = (phiPrefixMod[i - 1] + phi[i]) % MOD;
    }
}

long long solve(long long n, long long m, bool useMu) {
    long long k = n < m ? n : m;
    long long res = 0;
    for (long long l = 1; l <= k;) {
        long long qn = n / l;
        long long qm = m / l;
        long long rn = n / qn;
        long long rm = m / qm;
        long long r = rn < rm ? rn : rm;
        if (r > k) r = k;
        long long block = useMu
            ? ((muPrefixMod[r] - muPrefixMod[l - 1]) % MOD + MOD) % MOD
            : ((phiPrefixMod[r] - phiPrefixMod[l - 1]) % MOD + MOD) % MOD;
        long long t = block * (qn % MOD) % MOD * (qm % MOD) % MOD;
        res = (res + t) % MOD;
        l = r + 1;
    }
    return res;
}

int main() {
    sieve();
    int n;
    if (!(std::cin >> n)) return 0;
    std::string op;
    for (int i = 0; i < n; ++i) {
        if (!(std::cin >> op)) break;
        long long a, b;
        std::cin >> a >> b;
        if (op == "coprime") {
            std::cout << solve(a, b, true) << '\n';
        } else if (op == "gcd_sum") {
            std::cout << solve(a, b, false) << '\n';
        }
    }
    return 0;
}
