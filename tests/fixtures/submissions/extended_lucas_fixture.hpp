#include <cstdint>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#ifndef EXLUCAS_FERMAT_INVERSE
#define EXLUCAS_FERMAT_INVERSE 0
#endif
#ifndef EXLUCAS_RADICAL_ONLY
#define EXLUCAS_RADICAL_ONLY 0
#endif

using i64 = long long;
using i128 = __int128_t;

i64 exPower(i64 base, i64 exponent, i64 mod) {
    i64 result = 1 % mod;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<i64>(static_cast<i128>(result) * base % mod);
        base = static_cast<i64>(static_cast<i128>(base) * base % mod);
        exponent >>= 1;
    }
    return result;
}

i64 extendedGcd(i64 a, i64 b, i64& x, i64& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    i64 x1, y1;
    i64 gcd = extendedGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return gcd;
}

i64 modularInverse(i64 value, i64 mod) {
    if (EXLUCAS_FERMAT_INVERSE) return exPower(value, mod - 2, mod);
    i64 x, y;
    extendedGcd(value, mod, x, y);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}

struct PrimePower {
    i64 prime;
    i64 modulus;
    int exponent;
    std::vector<i64> prefix;

    PrimePower(i64 p, i64 pk, int e) : prime(p), modulus(pk), exponent(e), prefix(pk + 1) {
        prefix[0] = 1;
        for (i64 i = 1; i <= modulus; ++i) {
            prefix[i] = prefix[i - 1];
            if (i % prime != 0) prefix[i] = prefix[i] * i % modulus;
        }
    }
};

i64 primeExponentInFactorial(i64 n, i64 prime) {
    i64 exponent = 0;
    while (n > 0) {
        n /= prime;
        exponent += n;
    }
    return exponent;
}

i64 factorialWithoutPrime(i64 n, const PrimePower& power) {
    if (n == 0) return 1;
    i64 result = exPower(power.prefix[power.modulus], n / power.modulus, power.modulus);
    result = result * power.prefix[n % power.modulus] % power.modulus;
    return result * factorialWithoutPrime(n / power.prime, power) % power.modulus;
}

i64 binomialPrimePower(i64 n, i64 k, const PrimePower& power) {
    if (k < 0 || k > n) return 0;
    i64 exponent = primeExponentInFactorial(n, power.prime)
        - primeExponentInFactorial(k, power.prime)
        - primeExponentInFactorial(n - k, power.prime);
    if (exponent >= power.exponent) return 0;
    i64 numerator = factorialWithoutPrime(n, power);
    i64 denominator = factorialWithoutPrime(k, power)
        * factorialWithoutPrime(n - k, power) % power.modulus;
    i64 result = numerator * modularInverse(denominator, power.modulus) % power.modulus;
    return result * exPower(power.prime, exponent, power.modulus) % power.modulus;
}

i64 extendedLucas(i64 n, i64 k, i64 originalModulus) {
    if (originalModulus == 1 || k < 0 || k > n) return 0;
    i64 remaining = originalModulus;
    std::vector<PrimePower> powers;
    for (i64 prime = 2; prime * prime <= remaining; ++prime) {
        if (remaining % prime != 0) continue;
        i64 primePower = 1;
        int exponent = 0;
        while (remaining % prime == 0) {
            remaining /= prime;
            primePower *= prime;
            ++exponent;
        }
        if (EXLUCAS_RADICAL_ONLY) {
            primePower = prime;
            exponent = 1;
        }
        powers.emplace_back(prime, primePower, exponent);
    }
    if (remaining > 1) powers.emplace_back(remaining, remaining, 1);

    i64 result = 0;
    i64 combined = 1;
    for (const PrimePower& power : powers) {
        i64 residue = binomialPrimePower(n, k, power);
        i64 delta = (residue - result % power.modulus + power.modulus) % power.modulus;
        i64 step = delta * modularInverse(combined % power.modulus, power.modulus)
            % power.modulus;
        result = static_cast<i64>((static_cast<i128>(result) + static_cast<i128>(combined) * step)
                                  % (combined * power.modulus));
        combined *= power.modulus;
    }
    return result % originalModulus;
}

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        i64 n, k, modulus;
        std::cin >> operation >> n >> k >> modulus;
        std::cout << extendedLucas(n, k, modulus) << '\n';
    }
    return 0;
}
