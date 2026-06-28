#include "generator_registry.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace judge::generators {
namespace {

using ll = long long;
using i128 = __int128_t;

// ----- Trusted oracle: the same primitives the reference solution implements --

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

ll extgcd(ll a, ll b, ll& x, ll& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll lcmMod(ll a, ll b, ll m) {
    ll A = absll(a);
    ll B = absll(b);
    if (A == 0 || B == 0) return 0;
    ll g = gcdll(A, B);
    return (A / g) % m * (B % m) % m;
}

ll randomRange(std::mt19937_64& random, ll low, ll high) {
    return std::uniform_int_distribution<ll>(low, high)(random);
}

// Appends one operation to the input stream and its answer to the output stream.
void emitGcd(std::ostringstream& in, std::ostringstream& out, ll a, ll b) {
    in << "gcd " << a << ' ' << b << '\n';
    out << gcdll(a, b) << '\n';
}

void emitPow(std::ostringstream& in, std::ostringstream& out, ll a, ll b, ll m) {
    in << "pow " << a << ' ' << b << ' ' << m << '\n';
    out << powmod(a, b, m) << '\n';
}

void emitInv(std::ostringstream& in, std::ostringstream& out, ll a, ll m) {
    in << "inv " << a << ' ' << m << '\n';
    ll reduced = ((a % m) + m) % m;
    ll x, y;
    ll g = extgcd(reduced, m, x, y);
    if (g != 1) {
        out << "NONE" << '\n';
    } else {
        out << ((x % m) + m) % m << '\n';
    }
}

void emitLcm(std::ostringstream& in, std::ostringstream& out, ll a, ll b, ll m) {
    in << "lcm " << a << ' ' << b << ' ' << m << '\n';
    out << lcmMod(a, b, m) << '\n';
}

constexpr ll kValueLimit = 2'000'000'000;
constexpr ll kExponentLimit = 1'000'000'000'000'000'000LL;

GeneratedCase generateModularArithmeticToolkit(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "M01-modular-arithmetic-toolkit operation_limit is too small"
        );
    }

    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    // Pinned prefix: forces the known-wrong fixtures to diverge every case.
    // - inv with composite modulus / non-coprime base traps the Fermat bug.
    // - lcm with operands larger than the modulus traps the pre-mod bug.
    emitInv(in, out, 5, 14);            // Fermat would print 1, not 3.
    emitInv(in, out, 4, 14);            // Fermat prints a value, not NONE.
    emitInv(in, out, 3, 9);             // gcd != 1 -> NONE.
    emitLcm(in, out, 100, 60, 7);       // pre-mod would print 4, not 6.
    emitLcm(in, out, 0, 12345, 99);     // zero operand edge.
    emitPow(in, out, 0, 0, 7);          // 0^0 = 1.
    emitPow(in, out, 5, kExponentLimit, 1);  // modulus 1 edge -> 0.
    emitGcd(in, out, 0, 0);             // gcd(0, 0) = 0.
    int emitted = 8;

    while (emitted < operationCount) {
        const int kind = static_cast<int>(randomRange(random, 0, 3));
        if (kind == 0) {
            emitGcd(in, out,
                    randomRange(random, -kValueLimit, kValueLimit),
                    randomRange(random, -kValueLimit, kValueLimit));
        } else if (kind == 1) {
            emitPow(in, out,
                    randomRange(random, -kValueLimit, kValueLimit),
                    randomRange(random, 0, kExponentLimit),
                    randomRange(random, 1, kValueLimit));
        } else if (kind == 2) {
            // Occasionally use a small modulus so NONE / composite cases appear.
            const ll m = randomRange(random, 0, 4) == 0
                             ? randomRange(random, 2, 50)
                             : randomRange(random, 2, kValueLimit);
            emitInv(in, out, randomRange(random, -kValueLimit, kValueLimit), m);
        } else {
            emitLcm(in, out,
                    randomRange(random, -kValueLimit, kValueLimit),
                    randomRange(random, -kValueLimit, kValueLimit),
                    randomRange(random, 1, kValueLimit));
        }
        ++emitted;
    }

    return {in.str(), out.str()};
}

// ----- M02: linear congruence solver -----------------------------------------

ll invMod(ll a, ll m) {
    if (m == 1) return 0;
    ll x, y;
    extgcd(((a % m) + m) % m, m, x, y);
    return ((x % m) + m) % m;
}

void emitSolve(std::ostringstream& in, std::ostringstream& out, ll a, ll b, ll m) {
    in << "solve " << a << ' ' << b << ' ' << m << '\n';
    ll g = gcdll(a, m);
    ll bb = ((b % m) + m) % m;
    if (bb % g != 0) {
        out << "NO SOLUTION" << '\n';
    } else {
        ll mp = m / g;
        ll ap = ((a / g) % mp + mp) % mp;
        ll bp = (bb / g) % mp;
        ll x0 = static_cast<ll>(static_cast<i128>(bp) * invMod(ap, mp) % mp);
        x0 = ((x0 % mp) + mp) % mp;
        out << g << ' ' << x0 << '\n';
    }
}

void emitCrt(std::ostringstream& in, std::ostringstream& out,
             ll r1, ll m1, ll r2, ll m2) {
    in << "crt " << r1 << ' ' << m1 << ' ' << r2 << ' ' << m2 << '\n';
    ll g = gcdll(m1, m2);
    ll diff = r2 - r1;
    if (diff % g != 0) {
        out << "NO SOLUTION" << '\n';
    } else {
        ll lcm = m1 / g * m2;
        ll m2g = m2 / g;
        ll factor = ((diff / g) % m2g + m2g) % m2g;
        ll t = static_cast<ll>(
            static_cast<i128>(factor) * invMod((m1 / g) % m2g, m2g) % m2g);
        t = ((t % m2g) + m2g) % m2g;
        ll x = static_cast<ll>((static_cast<i128>(m1) * t + r1) % lcm);
        x = ((x % lcm) + lcm) % lcm;
        out << x << ' ' << lcm << '\n';
    }
}

GeneratedCase generateLinearCongruenceSolver(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 6) {
        throw std::runtime_error(
            "M02-linear-congruence-solver operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    // Pinned prefix: a multi-solution solve traps the count-one bug; a non-coprime
    // crt and the unsolvable cases trap the coprime-only CRT bug.
    emitSolve(in, out, 6, 4, 10);     // gcd 2 -> count must be 2, not 1.
    emitCrt(in, out, 2, 6, 8, 15);    // non-coprime moduli, solvable.
    emitSolve(in, out, 2, 1, 4);      // unsolvable.
    emitCrt(in, out, 1, 2, 0, 4);     // unsolvable.
    int emitted = 4;

    while (emitted < operationCount) {
        if (randomRange(random, 0, 1) == 0) {
            emitSolve(in, out,
                      randomRange(random, 0, 1'000'000'000),
                      randomRange(random, 0, 1'000'000'000),
                      randomRange(random, 1, 1'000'000'000));
        } else {
            emitCrt(in, out,
                    randomRange(random, 0, 1'000'000'000),
                    randomRange(random, 1, 1'000'000'000),
                    randomRange(random, 0, 1'000'000'000),
                    randomRange(random, 1, 1'000'000'000));
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M03: prime sieve queries ----------------------------------------------

struct Sieve {
    static constexpr int kLimit = 1'000'000;
    std::vector<int> spf;
    std::vector<int> primes;
    std::vector<int> piPrefix;
    Sieve() : spf(kLimit + 1, 0), piPrefix(kLimit + 1, 0) {
        for (int i = 2; i <= kLimit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                long long v = static_cast<long long>(i) * p;
                if (v > kLimit) break;
                spf[static_cast<int>(v)] = p;
                if (p == spf[i]) break;
            }
        }
        for (int i = 2; i <= kLimit; ++i) {
            piPrefix[i] = piPrefix[i - 1] + (spf[i] == i ? 1 : 0);
        }
    }
};

const Sieve& sharedSieve() {
    static const Sieve sieve;
    return sieve;
}

GeneratedCase generatePrimeSieveQueries(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M03-prime-sieve-queries operation_limit is too small"
        );
    }
    const Sieve& s = sharedSieve();
    const int primeCount = static_cast<int>(s.primes.size());

    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emitIsPrime = [&](long long x) {
        in << "is_prime " << x << '\n';
        out << ((x >= 2 && x <= Sieve::kLimit && s.spf[x] == x) ? "yes" : "no")
            << '\n';
    };
    auto emitCount = [&](long long a, long long b) {
        in << "count " << a << ' ' << b << '\n';
        if (b < 2 || a > b) {
            out << 0 << '\n';
        } else {
            long long lo = a < 2 ? 2 : a;
            out << s.piPrefix[b] - s.piPrefix[lo - 1] << '\n';
        }
    };
    auto emitNth = [&](long long k) {
        in << "nth " << k << '\n';
        if (k < 1 || k > primeCount) {
            out << "NONE" << '\n';
        } else {
            out << s.primes[k - 1] << '\n';
        }
    };
    auto emitSpf = [&](long long x) {
        in << "spf " << x << '\n';
        if (x < 2 || x > Sieve::kLimit) {
            out << "NONE" << '\n';
        } else {
            out << s.spf[x] << '\n';
        }
    };

    // Pinned prefix: traps the "1 is prime" and "count excludes endpoint" bugs.
    emitIsPrime(1);   // must be no.
    emitCount(2, 10); // must include the endpoint prime 2 -> 4.
    emitNth(25);
    emitSpf(84);
    int emitted = 4;

    while (emitted < operationCount) {
        switch (static_cast<int>(randomRange(random, 0, 3))) {
            case 0:
                emitIsPrime(randomRange(random, 0, Sieve::kLimit));
                break;
            case 1: {
                long long a = randomRange(random, 0, Sieve::kLimit);
                long long b = randomRange(random, 0, Sieve::kLimit);
                if (a > b) std::swap(a, b);
                emitCount(a, b);
                break;
            }
            case 2:
                emitNth(randomRange(random, 1, primeCount + 50));
                break;
            default:
                emitSpf(randomRange(random, 0, Sieve::kLimit));
                break;
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M04: factorization and divisor functions ------------------------------

void factorInto(ll n, std::vector<ll>& primes, std::vector<int>& exps) {
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

GeneratedCase generateDivisorFunctions(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M04-divisor-functions operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, ll n) {
        in << op << ' ' << n << '\n';
        std::vector<ll> primes;
        std::vector<int> exps;
        factorInto(n, primes, exps);
        if (op == "factorize") {
            if (n == 1 || primes.empty()) {
                out << 1 << '\n';
            } else {
                for (std::size_t j = 0; j < primes.size(); ++j) {
                    if (j != 0) out << ' ';
                    out << primes[j] << '^' << exps[j];
                }
                out << '\n';
            }
        } else if (op == "divisors") {
            ll d = 1;
            for (int e : exps) d *= (e + 1);
            out << d << '\n';
        } else if (op == "sigma") {
            ll s = 1;
            for (std::size_t j = 0; j < primes.size(); ++j) {
                ll term = 1, power = 1;
                for (int k = 0; k < exps[j]; ++k) {
                    power *= primes[j];
                    term += power;
                }
                s *= term;
            }
            out << s << '\n';
        } else {  // phi
            ll phi = n;
            for (ll p : primes) phi = phi / p * (p - 1);
            out << phi << '\n';
        }
    };

    // Pinned prefix: divisors of a composite traps the missing "+1"; phi of a
    // prime power traps dropping the p^(e-1) factor.
    emit("divisors", 12);
    emit("phi", 8);
    emit("factorize", 360);
    emit("sigma", 12);
    int emitted = 4;

    static const char* kOps[] = {"factorize", "divisors", "sigma", "phi"};
    while (emitted < operationCount) {
        const std::string op = kOps[randomRange(random, 0, 3)];
        emit(op, randomRange(random, 1, 1'000'000'000'000LL));
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M05: big number primality and factorization ---------------------------

using u64 = unsigned long long;
using u128 = __uint128_t;

u64 mulmodu(u64 a, u64 b, u64 m) { return static_cast<u64>(static_cast<u128>(a) * b % m); }

u64 powmodu(u64 a, u64 b, u64 m) {
    u64 r = 1 % m;
    a %= m;
    while (b) {
        if (b & 1) r = mulmodu(r, a, m);
        a = mulmodu(a, a, m);
        b >>= 1;
    }
    return r;
}

bool isPrimeU(u64 n) {
    if (n < 2) return false;
    for (u64 p : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        if (n % p == 0) return n == p;
    }
    u64 d = n - 1;
    int s = 0;
    while (!(d & 1)) { d >>= 1; ++s; }
    for (u64 a : {2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull, 31ull, 37ull}) {
        u64 x = powmodu(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int i = 0; i < s - 1; ++i) {
            x = mulmodu(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

u64 gcdu64(u64 a, u64 b) {
    while (b) { u64 t = a % b; a = b; b = t; }
    return a;
}

u64 pollardRhoU(u64 n) {
    if (n % 2 == 0) return 2;
    for (u64 c = 1;; ++c) {
        u64 x = 2, y = 2, d = 1;
        auto f = [&](u64 v) { return static_cast<u64>((static_cast<u128>(v) * v + c) % n); };
        while (d == 1) {
            x = f(x);
            y = f(f(y));
            u64 diff = x > y ? x - y : y - x;
            d = gcdu64(diff, n);
        }
        if (d != n) return d;
    }
}

void factorU(u64 n, std::vector<u64>& out) {
    if (n == 1) return;
    if (isPrimeU(n)) { out.push_back(n); return; }
    u64 d = pollardRhoU(n);
    factorU(d, out);
    factorU(n / d, out);
}

GeneratedCase generateBigFactorization(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M05-big-factorization operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, u64 n) {
        in << op << ' ' << n << '\n';
        if (op == "is_prime") {
            out << (isPrimeU(n) ? "yes" : "no") << '\n';
            return;
        }
        std::vector<u64> f;
        factorU(n, f);
        if (op == "max_factor") {
            if (f.empty()) out << "NONE" << '\n';
            else out << *std::max_element(f.begin(), f.end()) << '\n';
        } else if (op == "num_factors") {
            std::sort(f.begin(), f.end());
            f.erase(std::unique(f.begin(), f.end()), f.end());
            out << f.size() << '\n';
        } else {  // factor_count
            out << f.size() << '\n';
        }
    };

    // Pinned prefix: a base-2 pseudoprime traps the single-base bug; a number
    // with a repeated prime traps distinct-vs-multiplicity confusion.
    emit("is_prime", 2047);     // composite, base-2 strong pseudoprime.
    emit("num_factors", 12);    // 2 distinct, not 3.
    emit("factor_count", 12);
    emit("max_factor", 600851475143ull);
    int emitted = 4;

    static const char* kOps[] = {"is_prime", "max_factor", "num_factors", "factor_count"};
    while (emitted < operationCount) {
        const std::string op = kOps[randomRange(random, 0, 3)];
        u64 n = static_cast<u64>(
            std::uniform_int_distribution<u64>(1ull, 1'000'000'000'000'000'000ull)(random));
        emit(op, n);
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M06: modular inverse tables and binomials -----------------------------

struct BinomTables {
    static constexpr int kN = 1'000'000;
    static constexpr ll kP = 1'000'000'007;
    std::vector<ll> fact, invfact, inv;
    BinomTables() : fact(kN + 1), invfact(kN + 1), inv(kN + 1) {
        inv[1] = 1;
        for (int i = 2; i <= kN; ++i) {
            inv[i] = (kP - (kP / i) * inv[kP % i] % kP) % kP;
        }
        fact[0] = 1;
        for (int i = 1; i <= kN; ++i) fact[i] = fact[i - 1] * i % kP;
        invfact[0] = 1;
        for (int i = 1; i <= kN; ++i) invfact[i] = invfact[i - 1] * inv[i] % kP;
    }
    ll binom(ll n, ll r) const {
        if (r < 0 || r > n) return 0;
        return fact[n] * invfact[r] % kP * invfact[n - r] % kP;
    }
};

const BinomTables& sharedBinomTables() {
    static const BinomTables tables;
    return tables;
}

GeneratedCase generateModularBinomial(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M06-modular-binomial operation_limit is too small"
        );
    }
    const BinomTables& t = sharedBinomTables();
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emitInv = [&](ll x) { in << "inv " << x << '\n'; out << t.inv[x] << '\n'; };
    auto emitFact = [&](ll n) { in << "fact " << n << '\n'; out << t.fact[n] << '\n'; };
    auto emitInvFact = [&](ll n) {
        in << "inv_fact " << n << '\n';
        out << t.invfact[n] << '\n';
    };
    auto emitBinom = [&](ll n, ll r) {
        in << "binom " << n << ' ' << r << '\n';
        out << t.binom(n, r) << '\n';
    };

    // Pinned prefix: inv of a non-unit traps the sign-error recurrence; binom
    // with r == n traps the off-by-one boundary.
    emitInv(2);
    emitBinom(5, 5);
    emitBinom(5, 6);
    emitFact(10);
    int emitted = 4;

    while (emitted < operationCount) {
        switch (static_cast<int>(randomRange(random, 0, 3))) {
            case 0:
                emitInv(randomRange(random, 1, BinomTables::kN));
                break;
            case 1:
                emitFact(randomRange(random, 0, BinomTables::kN));
                break;
            case 2:
                emitInvFact(randomRange(random, 0, BinomTables::kN));
                break;
            default: {
                ll n = randomRange(random, 0, BinomTables::kN);
                // Bias r toward valid range but allow r > n for the 0 branch.
                ll r = randomRange(random, 0, n + 2);
                emitBinom(n, r);
                break;
            }
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M07: discrete logarithm (BSGS) ----------------------------------------

ll bsgsOracle(ll a, ll b, ll p) {
    a %= p;
    b %= p;
    if (b == 1 % p) return 0;
    if (a == 0) return b == 0 ? 1 : -1;
    ll n = static_cast<ll>(std::ceil(std::sqrt(static_cast<double>(p))));
    std::unordered_map<ll, ll> table;
    ll cur = b;
    for (ll j = 0; j < n; ++j) {
        table[cur] = j;
        cur = static_cast<ll>(static_cast<i128>(cur) * a % p);
    }
    ll an = powmod(a, n, p);
    cur = 1;
    for (ll i = 1; i <= n; ++i) {
        cur = static_cast<ll>(static_cast<i128>(cur) * an % p);
        auto it = table.find(cur);
        if (it != table.end()) return i * n - it->second;
    }
    return -1;
}

GeneratedCase generateDiscreteLogarithm(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M07-discrete-logarithm operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](ll a, ll b, ll p) {
        in << "log " << a << ' ' << b << ' ' << p << '\n';
        ll x = bsgsOracle(a, b, p);
        if (x < 0) out << "NONE" << '\n';
        else out << x << '\n';
    };

    // Pinned prefix: b=0 traps the "0 for zero" bug; b=1 traps the dropped
    // x=0 fast path (which would return the order instead of 0).
    emit(2, 0, 5);
    emit(5, 1, 1'000'000'007);
    emit(2, 3, 5);
    emit(3, 4, 7);
    int emitted = 4;

    static const ll kPrimes[] = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                 998244353, 1'000'000'007};
    const int primeCount = static_cast<int>(sizeof(kPrimes) / sizeof(kPrimes[0]));
    while (emitted < operationCount) {
        ll p = kPrimes[randomRange(random, 0, primeCount - 1)];
        ll a = randomRange(random, 1, p - 1);
        ll b = randomRange(random, 0, p - 1);
        emit(a, b, p);
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M08: multiplicative order and primitive root --------------------------

std::vector<ll> primeFactorsOf(ll n) {
    std::vector<ll> f;
    for (ll p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            f.push_back(p);
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) f.push_back(n);
    return f;
}

ll orderOf(ll a, ll p) {
    ll d = p - 1;
    for (ll q : primeFactorsOf(p - 1)) {
        while (d % q == 0 && powmod(a, d / q, p) == 1) d /= q;
    }
    return d;
}

bool isPrimitiveRoot(ll a, ll p) {
    if (p == 2) return a % p == 1;
    for (ll q : primeFactorsOf(p - 1)) {
        if (powmod(a, (p - 1) / q, p) == 1) return false;
    }
    return true;
}

ll smallestPrimitiveRoot(ll p) {
    if (p == 2) return 1;
    for (ll g = 2; g < p; ++g) {
        if (isPrimitiveRoot(g, p)) return g;
    }
    return -1;
}

ll eulerPhi(ll n) {
    ll r = n;
    for (ll p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            r -= r / p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) r -= r / n;
    return r;
}

GeneratedCase generatePrimitiveRoot(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M08-primitive-root operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emitOrder = [&](ll a, ll p) {
        in << "order " << a << ' ' << p << '\n';
        out << orderOf(a, p) << '\n';
    };
    auto emitRoot = [&](ll p) {
        in << "primitive_root " << p << '\n';
        out << smallestPrimitiveRoot(p) << '\n';
    };
    auto emitNum = [&](ll p) {
        in << "num_primitive_roots " << p << '\n';
        out << eulerPhi(p - 1) << '\n';
    };
    auto emitIsPrim = [&](ll a, ll p) {
        in << "is_primitive " << a << ' ' << p << '\n';
        out << (isPrimitiveRoot(a, p) ? "yes" : "no") << '\n';
    };

    // Pinned prefix: order 1 traps "assume primitive"; is_primitive of a
    // non-generator traps the Fermat-only check.
    emitOrder(1, 7);
    emitIsPrim(2, 7);
    emitRoot(7);
    emitNum(7);
    int emitted = 4;

    static const ll kPrimes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
                                 998244353, 1'000'000'007};
    const int primeCount = static_cast<int>(sizeof(kPrimes) / sizeof(kPrimes[0]));
    while (emitted < operationCount) {
        ll p = kPrimes[randomRange(random, 0, primeCount - 1)];
        switch (static_cast<int>(randomRange(random, 0, 3))) {
            case 0: emitOrder(randomRange(random, 1, p - 1), p); break;
            case 1: emitRoot(p); break;
            case 2: emitNum(p); break;
            default: emitIsPrim(randomRange(random, 1, p - 1), p); break;
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M09: quadratic residue and modular square root ------------------------

ll tonelliOracle(ll n, ll p) {
    n %= p;
    if (n == 0) return 0;
    if (powmod(n, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) return powmod(n, (p + 1) / 4, p);
    ll q = p - 1, s = 0;
    while (q % 2 == 0) { q /= 2; ++s; }
    ll z = 2;
    while (powmod(z, (p - 1) / 2, p) != p - 1) ++z;
    ll m = s;
    ll c = powmod(z, q, p);
    ll t = powmod(n, q, p);
    ll r = powmod(n, (q + 1) / 2, p);
    while (t != 1) {
        ll i = 0, t2 = t;
        while (t2 != 1) { t2 = static_cast<ll>(static_cast<i128>(t2) * t2 % p); ++i; }
        ll b = c;
        for (ll j = 0; j < m - i - 1; ++j) b = static_cast<ll>(static_cast<i128>(b) * b % p);
        m = i;
        c = static_cast<ll>(static_cast<i128>(b) * b % p);
        t = static_cast<ll>(static_cast<i128>(t) * c % p);
        r = static_cast<ll>(static_cast<i128>(r) * b % p);
    }
    return r;
}

GeneratedCase generateQuadraticResidue(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M09-quadratic-residue operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emitLegendre = [&](ll a, ll p) {
        in << "legendre " << a << ' ' << p << '\n';
        ll v = ((a % p) + p) % p;
        if (v == 0) out << 0 << '\n';
        else out << (powmod(v, (p - 1) / 2, p) == 1 ? 1 : -1) << '\n';
    };
    auto emitIsQr = [&](ll a, ll p) {
        in << "is_qr " << a << ' ' << p << '\n';
        ll v = ((a % p) + p) % p;
        out << ((v == 0 || powmod(v, (p - 1) / 2, p) == 1) ? "yes" : "no") << '\n';
    };
    auto emitSqrt = [&](ll a, ll p) {
        in << "sqrt " << a << ' ' << p << '\n';
        ll r = tonelliOracle(a, p);
        if (r < 0) out << "NONE" << '\n';
        else out << std::min(r, p - r) << '\n';
    };

    // Pinned prefix: legendre 0 traps the missing-zero bug; sqrt of a residue
    // with two distinct roots traps returning the larger root.
    emitLegendre(0, 7);
    emitSqrt(2, 7);
    emitSqrt(3, 7);
    emitIsQr(2, 7);
    int emitted = 4;

    static const ll kPrimes[] = {3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41,
                                 998244353, 1'000'000'007};
    const int primeCount = static_cast<int>(sizeof(kPrimes) / sizeof(kPrimes[0]));
    while (emitted < operationCount) {
        ll p = kPrimes[randomRange(random, 0, primeCount - 1)];
        ll a = randomRange(random, 0, p - 1);
        switch (static_cast<int>(randomRange(random, 0, 2))) {
            case 0: emitLegendre(a, p); break;
            case 1: emitIsQr(a, p); break;
            default: emitSqrt(a, p); break;
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M10-M12: shared multiplicative sieve (mu, phi, d, sigma) ---------------

struct MultiSieve {
    static constexpr int kLimit = 1'000'000;
    std::vector<int> spf;
    std::vector<int> mu;
    std::vector<int> phi;
    std::vector<int> numDiv;     // d(n): number of divisors
    std::vector<int> expSmall;   // exponent of the smallest prime factor
    std::vector<ll> sigma;       // sum of divisors
    std::vector<ll> gSmall;      // 1 + p + ... + p^expSmall, for the smallest p
    std::vector<ll> muPrefix;
    std::vector<ll> phiPrefix;     // raw prefix sum of phi (fits in 64 bits)
    std::vector<ll> phiPrefixMod;
    static constexpr ll kMod = 1'000'000'007;

    MultiSieve()
        : spf(kLimit + 1, 0),
          mu(kLimit + 1, 0),
          phi(kLimit + 1, 0),
          numDiv(kLimit + 1, 0),
          expSmall(kLimit + 1, 0),
          sigma(kLimit + 1, 0),
          gSmall(kLimit + 1, 0),
          muPrefix(kLimit + 1, 0),
          phiPrefix(kLimit + 1, 0),
          phiPrefixMod(kLimit + 1, 0) {
        std::vector<int> primes;
        mu[1] = 1;
        phi[1] = 1;
        numDiv[1] = 1;
        sigma[1] = 1;
        expSmall[1] = 0;
        gSmall[1] = 1;
        for (int i = 2; i <= kLimit; ++i) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.push_back(i);
                mu[i] = -1;
                phi[i] = i - 1;
                numDiv[i] = 2;
                expSmall[i] = 1;
                sigma[i] = static_cast<ll>(i) + 1;
                gSmall[i] = static_cast<ll>(i) + 1;
            }
            for (int p : primes) {
                long long v = static_cast<long long>(i) * p;
                if (v > kLimit) break;
                int vv = static_cast<int>(v);
                spf[vv] = p;
                if (i % p == 0) {
                    mu[vv] = 0;
                    phi[vv] = phi[i] * p;
                    expSmall[vv] = expSmall[i] + 1;
                    numDiv[vv] = numDiv[i] / (expSmall[i] + 1) * (expSmall[i] + 2);
                    gSmall[vv] = gSmall[i] * p + 1;
                    sigma[vv] = sigma[i] / gSmall[i] * gSmall[vv];
                    break;
                }
                mu[vv] = -mu[i];
                phi[vv] = phi[i] * (p - 1);
                expSmall[vv] = 1;
                numDiv[vv] = numDiv[i] * 2;
                gSmall[vv] = static_cast<ll>(p) + 1;
                sigma[vv] = sigma[i] * (static_cast<ll>(p) + 1);
            }
        }
        for (int i = 1; i <= kLimit; ++i) {
            muPrefix[i] = muPrefix[i - 1] + mu[i];
            phiPrefix[i] = phiPrefix[i - 1] + phi[i];
            phiPrefixMod[i] = (phiPrefixMod[i - 1] + phi[i]) % kMod;
        }
    }
};

const MultiSieve& sharedMultiSieve() {
    static const MultiSieve sieve;
    return sieve;
}

ll divisorBlockSum(ll n) {
    ll sum = 0;
    for (ll l = 1; l <= n;) {
        ll q = n / l;
        ll r = n / q;
        sum += q * (r - l + 1);
        l = r + 1;
    }
    return sum;
}

GeneratedCase generateMobiusDivisorBlock(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M10-mobius-divisor-block operation_limit is too small"
        );
    }
    const MultiSieve& s = sharedMultiSieve();
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emitMu = [&](ll x) {
        in << "mu " << x << '\n';
        out << s.mu[x] << '\n';
    };
    auto emitMuPrefix = [&](ll n) {
        in << "mu_prefix " << n << '\n';
        out << s.muPrefix[n] << '\n';
    };
    auto emitFloorSum = [&](ll n) {
        in << "floor_sum " << n << '\n';
        out << divisorBlockSum(n) << '\n';
    };

    // Pinned prefix: floor_sum 1 traps the dropped-+1 block bug; mu 4 traps the
    // "still squarefree" bug; the rest exercise both families on small inputs.
    emitFloorSum(1);
    emitMu(4);
    emitMuPrefix(10);
    emitFloorSum(6);
    int emitted = 4;

    while (emitted < operationCount) {
        switch (static_cast<int>(randomRange(random, 0, 2))) {
            case 0:
                emitMu(randomRange(random, 1, MultiSieve::kLimit));
                break;
            case 1:
                emitMuPrefix(randomRange(random, 1, MultiSieve::kLimit));
                break;
            default:
                emitFloorSum(randomRange(random, 1, 1'000'000'000LL));
                break;
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

GeneratedCase generateMultiplicativeSieve(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M11-multiplicative-sieve operation_limit is too small"
        );
    }
    const MultiSieve& s = sharedMultiSieve();
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, ll x) {
        in << op << ' ' << x << '\n';
        if (op == "phi") {
            out << s.phi[x] << '\n';
        } else if (op == "mu") {
            out << s.mu[x] << '\n';
        } else if (op == "d") {
            out << s.numDiv[x] << '\n';
        } else {
            out << s.sigma[x] << '\n';
        }
    };

    // Pinned prefix: d 4 traps the "double divisors on prime powers" bug; phi 4
    // traps multiplying by (p-1) instead of p when p | i.
    emit("d", 4);
    emit("phi", 4);
    emit("sigma", 12);
    emit("mu", 30);
    int emitted = 4;

    static const char* kOps[] = {"phi", "mu", "d", "sigma"};
    while (emitted < operationCount) {
        emit(kOps[randomRange(random, 0, 3)],
             randomRange(random, 1, MultiSieve::kLimit));
        ++emitted;
    }
    return {in.str(), out.str()};
}

GeneratedCase generateMobiusInversionGcd(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M12-mobius-inversion-gcd operation_limit is too small"
        );
    }
    const MultiSieve& s = sharedMultiSieve();
    const ll MOD = MultiSieve::kMod;
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto solve = [&](ll n, ll m, bool useMu) -> ll {
        ll k = n < m ? n : m;
        ll res = 0;
        for (ll l = 1; l <= k;) {
            ll qn = n / l;
            ll qm = m / l;
            ll rn = n / qn;
            ll rm = m / qm;
            ll r = rn < rm ? rn : rm;
            if (r > k) r = k;
            ll block = useMu
                ? ((s.muPrefix[r] - s.muPrefix[l - 1]) % MOD + MOD) % MOD
                : ((s.phiPrefixMod[r] - s.phiPrefixMod[l - 1]) % MOD + MOD) % MOD;
            ll t = block * (qn % MOD) % MOD * (qm % MOD) % MOD;
            res = (res + t) % MOD;
            l = r + 1;
        }
        return res;
    };
    auto emit = [&](const std::string& op, ll n, ll m) {
        in << op << ' ' << n << ' ' << m << '\n';
        out << solve(n, m, op == "coprime") << '\n';
    };

    // Pinned prefix: coprime on the largest n=m traps the missing-mod overflow;
    // an asymmetric gcd_sum traps the single-quotient block step.
    emit("coprime", 1'000'000, 1'000'000);
    emit("gcd_sum", 1000, 999);
    emit("coprime", 3, 3);
    emit("gcd_sum", 2, 2);
    int emitted = 4;

    while (emitted < operationCount) {
        ll n = randomRange(random, 1, MultiSieve::kLimit);
        ll m = randomRange(random, 1, MultiSieve::kLimit);
        emit(randomRange(random, 0, 1) == 0 ? "coprime" : "gcd_sum", n, m);
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M13: Du-sieve oracle for phi/mu prefix sums ----------------------------

std::unordered_map<ll, ll>& duPhiMemo() {
    static std::unordered_map<ll, ll> memo;
    return memo;
}
std::unordered_map<ll, ll>& duMuMemo() {
    static std::unordered_map<ll, ll> memo;
    return memo;
}

ll duPhi(ll n) {
    const MultiSieve& s = sharedMultiSieve();
    if (n <= MultiSieve::kLimit) return s.phiPrefix[n];
    auto& memo = duPhiMemo();
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    ll res = n * (n + 1) / 2;
    for (ll l = 2; l <= n;) {
        ll q = n / l;
        ll r = n / q;
        res -= (r - l + 1) * duPhi(q);
        l = r + 1;
    }
    memo[n] = res;
    return res;
}

ll duMu(ll n) {
    const MultiSieve& s = sharedMultiSieve();
    if (n <= MultiSieve::kLimit) return s.muPrefix[n];
    auto& memo = duMuMemo();
    auto it = memo.find(n);
    if (it != memo.end()) return it->second;
    ll res = 1;
    for (ll l = 2; l <= n;) {
        ll q = n / l;
        ll r = n / q;
        res -= (r - l + 1) * duMu(q);
        l = r + 1;
    }
    memo[n] = res;
    return res;
}

GeneratedCase generateDuSieve(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error("M13-du-sieve operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, ll n) {
        in << op << ' ' << n << '\n';
        out << (op == "phi_prefix" ? duPhi(n) : duMu(n)) << '\n';
    };

    // Pinned prefix: the two largest n exercise the recurrence (n above the
    // sieve threshold) and trap the wrong harmonic base and the dropped block
    // term; the small n confirm the precomputed-prefix fast path.
    emit("phi_prefix", 1'000'000'000);
    emit("mu_prefix", 1'000'000'000);
    emit("phi_prefix", 2'000'000);
    emit("mu_prefix", 7);
    int emitted = 4;

    while (emitted < operationCount) {
        // Bias toward values above the sieve threshold so the recurrence runs.
        ll n = randomRange(random, MultiSieve::kLimit + 1, 1'000'000'000LL);
        emit(randomRange(random, 0, 1) == 0 ? "phi_prefix" : "mu_prefix", n);
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M14: Min_25 prime-count / prime-sum stress oracle ---------------------

struct SmallPrimePrefix {
    static constexpr int kLimit = 1'000'000;
    std::vector<int> count;
    std::vector<ll> sumMod;

    SmallPrimePrefix() : count(kLimit + 1, 0), sumMod(kLimit + 1, 0) {
        const MultiSieve& s = sharedMultiSieve();
        for (int i = 1; i <= kLimit; ++i) {
            bool prime = i >= 2 && s.spf[i] == i;
            count[i] = count[i - 1] + (prime ? 1 : 0);
            sumMod[i] = (sumMod[i - 1] + (prime ? i : 0)) % MultiSieve::kMod;
        }
    }
};

const SmallPrimePrefix& smallPrimePrefix() {
    static const SmallPrimePrefix prefix;
    return prefix;
}

GeneratedCase generateMin25PrimeSums(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error("M14-min25-prime-sums operation_limit is too small");
    }
    const SmallPrimePrefix& prefix = smallPrimePrefix();
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, int n) {
        in << op << ' ' << n << '\n';
        out << (op == "pi" ? prefix.count[n] : prefix.sumMod[n]) << '\n';
    };

    // 1 traps an unremoved identity; prime squares trap a strict > p*p update.
    emit("pi", 1);
    emit("prime_sum", 1);
    emit("pi", 49);
    emit("prime_sum", 25);
    int emitted = 4;
    while (emitted < operationCount) {
        emit(randomRange(random, 0, 1) == 0 ? "pi" : "prime_sum",
             static_cast<int>(randomRange(random, 1, SmallPrimePrefix::kLimit)));
        ++emitted;
    }
    return {in.str(), out.str()};
}

// ----- M15: powerful-number representation oracle ---------------------------

ll floorSqrt(ll n) {
    ll root = static_cast<ll>(std::sqrt(static_cast<long double>(n)));
    while (static_cast<i128>(root + 1) * (root + 1) <= n) ++root;
    while (static_cast<i128>(root) * root > n) --root;
    return root;
}

std::pair<ll, ll> powerfulSummary(ll n) {
    const MultiSieve& s = sharedMultiSieve();
    const ll mod = MultiSieve::kMod;
    const ll inv6 = 166'666'668;
    ll count = 0;
    ll sum = 0;
    for (ll b = 1; static_cast<i128>(b) * b * b <= n; ++b) {
        if (s.mu[b] == 0) continue;
        ll cube = b * b * b;
        ll a = floorSqrt(n / cube);
        count += a;
        ll squareSum = (a % mod) * ((a + 1) % mod) % mod;
        squareSum = squareSum * ((2 * (a % mod) + 1) % mod) % mod * inv6 % mod;
        sum = (sum + (cube % mod) * squareSum) % mod;
    }
    return {count, sum};
}

GeneratedCase generatePowerfulNumberSieve(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error("M15-powerful-number-sieve operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';

    auto emit = [&](const std::string& op, ll n) {
        auto [count, sum] = powerfulSummary(n);
        in << op << ' ' << n << '\n';
        out << (op == "count" ? count : sum) << '\n';
    };

    // 1 traps dropping the identity; 64 traps duplicate b=4 if square-free is
    // not enforced; 72 checks a mixed 2^3*3^2 powerful number.
    emit("count", 1);
    emit("sum", 64);
    emit("count", 64);
    emit("sum", 72);
    int emitted = 4;
    while (emitted < operationCount) {
        emit(randomRange(random, 0, 1) == 0 ? "count" : "sum",
             randomRange(random, 1, 1'000'000'000LL));
        ++emitted;
    }
    return {in.str(), out.str()};
}

}  // namespace

void registerNumberTheoryGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace(
        "M01-modular-arithmetic-toolkit",
        generateModularArithmeticToolkit
    );
    registry.emplace(
        "M02-linear-congruence-solver",
        generateLinearCongruenceSolver
    );
    registry.emplace(
        "M03-prime-sieve-queries",
        generatePrimeSieveQueries
    );
    registry.emplace(
        "M04-divisor-functions",
        generateDivisorFunctions
    );
    registry.emplace(
        "M05-big-factorization",
        generateBigFactorization
    );
    registry.emplace(
        "M06-modular-binomial",
        generateModularBinomial
    );
    registry.emplace(
        "M07-discrete-logarithm",
        generateDiscreteLogarithm
    );
    registry.emplace(
        "M08-primitive-root",
        generatePrimitiveRoot
    );
    registry.emplace(
        "M09-quadratic-residue",
        generateQuadraticResidue
    );
    registry.emplace(
        "M10-mobius-divisor-block",
        generateMobiusDivisorBlock
    );
    registry.emplace(
        "M11-multiplicative-sieve",
        generateMultiplicativeSieve
    );
    registry.emplace(
        "M12-mobius-inversion-gcd",
        generateMobiusInversionGcd
    );
    registry.emplace(
        "M13-du-sieve",
        generateDuSieve
    );
    registry.emplace(
        "M14-min25-prime-sums",
        generateMin25PrimeSums
    );
    registry.emplace(
        "M15-powerful-number-sieve",
        generatePowerfulNumberSieve
    );
}

}  // namespace judge::generators
