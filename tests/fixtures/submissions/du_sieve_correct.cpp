// Reference solution for M13-du-sieve.
// Linear-sieves phi and mu up to a threshold, then answers prefix sums of phi
// and mu for n up to 1e9 with the memoised Du-sieve recurrence + divisor blocks.
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const int SIEVE_MAX = 1500000;
std::vector<int> primes;
std::vector<bool> isComposite;
std::vector<int> phiSmall;
std::vector<int> muSmall;
std::vector<long long> phiPrefix;
std::vector<long long> muPrefix;
std::unordered_map<long long, long long> phiMemo;
std::unordered_map<long long, long long> muMemo;

void sieve() {
    isComposite.assign(SIEVE_MAX + 1, false);
    phiSmall.assign(SIEVE_MAX + 1, 0);
    muSmall.assign(SIEVE_MAX + 1, 0);
    phiPrefix.assign(SIEVE_MAX + 1, 0);
    muPrefix.assign(SIEVE_MAX + 1, 0);
    phiSmall[1] = 1;
    muSmall[1] = 1;
    for (int i = 2; i <= SIEVE_MAX; ++i) {
        if (!isComposite[i]) {
            primes.push_back(i);
            phiSmall[i] = i - 1;
            muSmall[i] = -1;
        }
        for (int p : primes) {
            long long v = static_cast<long long>(i) * p;
            if (v > SIEVE_MAX) break;
            int vv = static_cast<int>(v);
            isComposite[vv] = true;
            if (i % p == 0) {
                phiSmall[vv] = phiSmall[i] * p;
                muSmall[vv] = 0;
                break;
            }
            phiSmall[vv] = phiSmall[i] * (p - 1);
            muSmall[vv] = -muSmall[i];
        }
    }
    for (int i = 1; i <= SIEVE_MAX; ++i) {
        phiPrefix[i] = phiPrefix[i - 1] + phiSmall[i];
        muPrefix[i] = muPrefix[i - 1] + muSmall[i];
    }
}

long long sPhi(long long n) {
    if (n <= SIEVE_MAX) return phiPrefix[n];
    auto it = phiMemo.find(n);
    if (it != phiMemo.end()) return it->second;
    long long res = n * (n + 1) / 2;
    for (long long l = 2; l <= n;) {
        long long q = n / l;
        long long r = n / q;
        res -= (r - l + 1) * sPhi(q);
        l = r + 1;
    }
    phiMemo[n] = res;
    return res;
}

long long sMu(long long n) {
    if (n <= SIEVE_MAX) return muPrefix[n];
    auto it = muMemo.find(n);
    if (it != muMemo.end()) return it->second;
    long long res = 1;
    for (long long l = 2; l <= n;) {
        long long q = n / l;
        long long r = n / q;
        res -= (r - l + 1) * sMu(q);
        l = r + 1;
    }
    muMemo[n] = res;
    return res;
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
        if (op == "phi_prefix") {
            std::cout << sPhi(x) << '\n';
        } else if (op == "mu_prefix") {
            std::cout << sMu(x) << '\n';
        }
    }
    return 0;
}
