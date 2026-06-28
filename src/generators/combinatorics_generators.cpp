#include "generator_registry.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

using ll = long long;
using i128 = __int128_t;
constexpr ll kMod = 1'000'000'007;

ll randomRange(std::mt19937_64& random, ll low, ll high) {
    return std::uniform_int_distribution<ll>(low, high)(random);
}

ll powMod(ll base, ll exponent, ll mod) {
    ll result = 1 % mod;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<ll>(static_cast<i128>(result) * base % mod);
        base = static_cast<ll>(static_cast<i128>(base) * base % mod);
        exponent >>= 1;
    }
    return result;
}

ll smallBinomialPrime(ll n, ll k, ll prime) {
    if (k < 0 || k > n) return 0;
    k = std::min(k, n - k);
    ll result = 1;
    for (ll i = 1; i <= k; ++i) {
        result = result * ((n - k + i) % prime) % prime;
        result = result * powMod(i, prime - 2, prime) % prime;
    }
    return result;
}

ll lucas(ll n, ll k, ll prime) {
    if (k < 0 || k > n) return 0;
    ll result = 1;
    while (n > 0 || k > 0) {
        ll ni = n % prime;
        ll ki = k % prime;
        if (ki > ni) return 0;
        result = result * smallBinomialPrime(ni, ki, prime) % prime;
        n /= prime;
        k /= prime;
    }
    return result;
}

GeneratedCase generateLucasBinomial(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M16-lucas-binomial operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](ll n, ll k, ll prime) {
        in << "lucas " << n << ' ' << k << ' ' << prime << '\n';
        out << lucas(n, k, prime) << '\n';
    };

    emit(7, 1, 7);       // A low digit violation must force zero.
    emit(17, 7, 7);      // Higher digits matter; least-digit-only gives one.
    emit(100, 50, 13);
    emit(1'000'000'000'000LL, 123'456'789LL, 29);
    int emitted = 4;
    static constexpr ll kPrimes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    while (emitted < operationCount) {
        ll n = randomRange(random, 0, 1'000'000'000'000LL);
        ll k = randomRange(random, 0, n + 2);
        ll prime = kPrimes[randomRange(random, 0, 10)];
        emit(n, k, prime);
        ++emitted;
    }
    return {in.str(), out.str()};
}

i128 exactBinomial(int n, int k) {
    if (k < 0 || k > n) return 0;
    k = std::min(k, n - k);
    i128 result = 1;
    for (int i = 1; i <= k; ++i) result = result * (n - k + i) / i;
    return result;
}

GeneratedCase generateExtendedLucas(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M17-extended-lucas operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](int n, int k, int mod) {
        in << "binom " << n << ' ' << k << ' ' << mod << '\n';
        out << static_cast<ll>(exactBinomial(n, k) % mod) << '\n';
    };

    emit(8, 4, 8);       // Prime-power multiplicity and non-Fermat inverses.
    emit(10, 3, 12);
    emit(20, 10, 72);
    emit(5, 8, 100);
    int emitted = 4;
    while (emitted < operationCount) {
        int n = static_cast<int>(randomRange(random, 0, 60));
        int k = static_cast<int>(randomRange(random, 0, n + 3));
        int mod = static_cast<int>(randomRange(random, 1, 500));
        emit(n, k, mod);
        ++emitted;
    }
    return {in.str(), out.str()};
}

ll ontoBrute(int n, int k) {
    if (k == 0) return n == 0 ? 1 : 0;
    ll count = 0;
    auto visit = [&](auto&& self, int position, int mask) -> void {
        if (position == n) {
            if (mask == (1 << k) - 1) ++count;
            return;
        }
        for (int target = 0; target < k; ++target) {
            self(self, position + 1, mask | (1 << target));
        }
    };
    visit(visit, 0, 0);
    return count % kMod;
}

ll derangementBrute(int n) {
    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);
    ll count = 0;
    do {
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            if (permutation[i] == i) {
                valid = false;
                break;
            }
        }
        if (valid) ++count;
    } while (std::next_permutation(permutation.begin(), permutation.end()));
    return count % kMod;
}

GeneratedCase generateInclusionExclusionCounting(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "M18-inclusion-exclusion-counting operation_limit is too small"
        );
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emitOnto = [&](int n, int k) {
        in << "onto " << n << ' ' << k << '\n';
        out << ontoBrute(n, k) << '\n';
    };
    auto emitDerangement = [&](int n) {
        in << "derangement " << n << '\n';
        out << derangementBrute(n) << '\n';
    };

    emitOnto(0, 0);
    emitOnto(3, 2);
    emitOnto(4, 3);
    emitDerangement(0);
    int emitted = 4;
    while (emitted < operationCount) {
        if (randomRange(random, 0, 1) == 0) {
            emitOnto(static_cast<int>(randomRange(random, 0, 6)),
                     static_cast<int>(randomRange(random, 0, 6)));
        } else {
            emitDerangement(static_cast<int>(randomRange(random, 0, 9)));
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> dyckPeakCounts(int n) {
    std::vector<ll> peaks(n + 1, 0);
    auto visit = [&](auto&& self, int opened, int closed, bool previousOpen, int peakCount) -> void {
        if (opened == n && closed == n) {
            ++peaks[peakCount];
            return;
        }
        if (opened < n) self(self, opened + 1, closed, true, peakCount);
        if (closed < opened) {
            self(self, opened, closed + 1, false, peakCount + (previousOpen ? 1 : 0));
        }
    };
    visit(visit, 0, 0, false, 0);
    return peaks;
}

ll ballotBrute(int horizontal, int vertical) {
    if (vertical > horizontal) return 0;
    std::vector<std::vector<ll>> dp(horizontal + 1, std::vector<ll>(vertical + 1, 0));
    dp[0][0] = 1;
    for (int x = 0; x <= horizontal; ++x) {
        for (int y = 0; y <= vertical; ++y) {
            if (y > x || (x == 0 && y == 0)) continue;
            if (x > 0) dp[x][y] += dp[x - 1][y];
            if (y > 0) dp[x][y] += dp[x][y - 1];
        }
    }
    return dp[horizontal][vertical] % kMod;
}

GeneratedCase generateCatalanFamilies(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M19-catalan-families operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emitCatalan = [&](int n) {
        in << "catalan " << n << '\n';
        if (n <= 8) {
            auto peaks = dyckPeakCounts(n);
            out << std::accumulate(peaks.begin(), peaks.end(), 0LL) % kMod << '\n';
        } else {
            out << static_cast<ll>((exactBinomial(2 * n, n) / (n + 1)) % kMod) << '\n';
        }
    };
    auto emitNarayana = [&](int n, int peakCount) {
        auto peaks = dyckPeakCounts(n);
        in << "narayana " << n << ' ' << peakCount << '\n';
        out << (peakCount >= 0 && peakCount < static_cast<int>(peaks.size())
                    ? peaks[peakCount] % kMod : 0) << '\n';
    };
    auto emitBallot = [&](int horizontal, int vertical) {
        in << "ballot " << horizontal << ' ' << vertical << '\n';
        out << ballotBrute(horizontal, vertical) << '\n';
    };

    emitCatalan(0);
    emitCatalan(20);  // Forces modular rather than ordinary integer division.
    emitNarayana(4, 2);
    emitBallot(2, 1);
    int emitted = 4;
    while (emitted < operationCount) {
        int kind = static_cast<int>(randomRange(random, 0, 2));
        if (kind == 0) {
            emitCatalan(static_cast<int>(randomRange(random, 0, 8)));
        } else if (kind == 1) {
            int n = static_cast<int>(randomRange(random, 1, 8));
            emitNarayana(n, static_cast<int>(randomRange(random, 0, n + 1)));
        } else {
            int horizontal = static_cast<int>(randomRange(random, 0, 10));
            int vertical = static_cast<int>(randomRange(random, 0, 10));
            emitBallot(horizontal, vertical);
        }
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> stirlingFirstBrute(int n) {
    std::vector<ll> counts(n + 1, 0);
    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);
    do {
        std::vector<bool> seen(n, false);
        int cycles = 0;
        for (int start = 0; start < n; ++start) {
            if (seen[start]) continue;
            ++cycles;
            int current = start;
            while (!seen[current]) {
                seen[current] = true;
                current = permutation[current];
            }
        }
        ++counts[cycles];
    } while (std::next_permutation(permutation.begin(), permutation.end()));
    return counts;
}

std::vector<ll> stirlingSecondBrute(int n) {
    std::vector<ll> counts(n + 1, 0);
    if (n == 0) {
        counts[0] = 1;
        return counts;
    }
    auto visit = [&](auto&& self, int position, int maximumLabel) -> void {
        if (position == n) {
            ++counts[maximumLabel + 1];
            return;
        }
        for (int label = 0; label <= maximumLabel + 1; ++label) {
            self(self, position + 1, std::max(maximumLabel, label));
        }
    };
    visit(visit, 1, 0);
    return counts;
}

GeneratedCase generateStirlingBell(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M20-stirling-bell operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emitFirst = [&](int n, int k) {
        auto counts = stirlingFirstBrute(n);
        in << "stirling1 " << n << ' ' << k << '\n';
        out << (k >= 0 && k <= n ? counts[k] : 0) << '\n';
    };
    auto emitSecond = [&](int n, int k) {
        auto counts = stirlingSecondBrute(n);
        in << "stirling2 " << n << ' ' << k << '\n';
        out << (k >= 0 && k <= n ? counts[k] : 0) << '\n';
    };
    auto emitBell = [&](int n) {
        auto counts = stirlingSecondBrute(n);
        in << "bell " << n << '\n';
        out << std::accumulate(counts.begin(), counts.end(), 0LL) % kMod << '\n';
    };

    emitFirst(4, 2);
    emitSecond(0, 0);
    emitSecond(4, 2);
    emitBell(0);
    int emitted = 4;
    while (emitted < operationCount) {
        int n = static_cast<int>(randomRange(random, 0, 8));
        int kind = static_cast<int>(randomRange(random, 0, 2));
        if (kind == 0) emitFirst(n, static_cast<int>(randomRange(random, 0, n + 1)));
        else if (kind == 1) emitSecond(n, static_cast<int>(randomRange(random, 0, n + 1)));
        else emitBell(n);
        ++emitted;
    }
    return {in.str(), out.str()};
}

ll partitionBrute(int remaining, int maximumPart, bool distinct) {
    if (remaining == 0) return 1;
    if (remaining < 0 || maximumPart == 0) return 0;
    ll count = 0;
    int upper = std::min(remaining, maximumPart);
    for (int part = upper; part >= 1; --part) {
        count += partitionBrute(remaining - part, distinct ? part - 1 : part, distinct);
    }
    return count;
}

GeneratedCase generateIntegerPartitions(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M21-integer-partitions operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](const std::string& operation, int n) {
        in << operation << ' ' << n << '\n';
        out << partitionBrute(n, n, operation == "distinct") % kMod << '\n';
    };

    emit("partition", 0);
    emit("partition", 5);
    emit("distinct", 5);
    emit("distinct", 2);
    int emitted = 4;
    while (emitted < operationCount) {
        emit(randomRange(random, 0, 1) == 0 ? "partition" : "distinct",
             static_cast<int>(randomRange(random, 0, 30)));
        ++emitted;
    }
    return {in.str(), out.str()};
}

ll recurrenceBrute(
    const std::vector<ll>& initial,
    const std::vector<ll>& coefficients,
    int index
) {
    if (index < static_cast<int>(initial.size())) return initial[index];
    std::vector<ll> values = initial;
    for (int position = static_cast<int>(initial.size()); position <= index; ++position) {
        ll value = 0;
        for (int offset = 1; offset <= static_cast<int>(coefficients.size()); ++offset) {
            value = (value + coefficients[offset - 1] * values[position - offset]) % kMod;
        }
        values.push_back(value);
    }
    return values[index];
}

GeneratedCase generateRationalOgf(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M22-rational-ogf-recurrence operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](const std::vector<ll>& initial,
                    const std::vector<ll>& coefficients,
                    int index) {
        in << "recurrence " << initial.size() << ' ' << index;
        for (ll value : initial) in << ' ' << value;
        for (ll coefficient : coefficients) in << ' ' << coefficient;
        in << '\n';
        out << recurrenceBrute(initial, coefficients, index) << '\n';
    };

    emit({0, 1}, {1, 1}, 10);
    emit({5, 7, 11}, {2, 3, 4}, 1);
    emit({1, 2}, {2, 3}, 8);
    emit({3}, {2}, 20);
    int emitted = 4;
    while (emitted < operationCount) {
        int order = static_cast<int>(randomRange(random, 1, 5));
        std::vector<ll> initial(order);
        std::vector<ll> coefficients(order);
        for (ll& value : initial) value = randomRange(random, 0, 20);
        for (ll& coefficient : coefficients) coefficient = randomRange(random, 0, 10);
        emit(initial, coefficients, static_cast<int>(randomRange(random, 0, 40)));
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> multiplyPolynomial(
    const std::vector<ll>& left,
    const std::vector<ll>& right,
    int degreeLimit
) {
    std::vector<ll> product(degreeLimit + 1, 0);
    for (int i = 0; i < static_cast<int>(left.size()) && i <= degreeLimit; ++i) {
        for (int j = 0; j < static_cast<int>(right.size()) && i + j <= degreeLimit; ++j) {
            product[i + j] = (product[i + j] + left[i] * right[j]) % kMod;
        }
    }
    return product;
}

ll lagrangeForestBrute(int arity, int nodes, int components) {
    std::vector<ll> tree(nodes + 1, 0);
    for (int iteration = 0; iteration <= nodes; ++iteration) {
        std::vector<ll> base = tree;
        base[0] = (base[0] + 1) % kMod;
        std::vector<ll> power(nodes + 1, 0);
        power[0] = 1;
        for (int copy = 0; copy < arity; ++copy) {
            power = multiplyPolynomial(power, base, nodes - 1);
        }
        std::vector<ll> next(nodes + 1, 0);
        for (int degree = 1; degree <= nodes; ++degree) next[degree] = power[degree - 1];
        tree.swap(next);
    }
    std::vector<ll> forest(nodes + 1, 0);
    forest[0] = 1;
    for (int component = 0; component < components; ++component) {
        forest = multiplyPolynomial(forest, tree, nodes);
    }
    return forest[nodes];
}

GeneratedCase generateLagrangeForests(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M23-lagrange-forests operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](int arity, int nodes, int components) {
        in << "forest " << arity << ' ' << nodes << ' ' << components << '\n';
        out << lagrangeForestBrute(arity, nodes, components) << '\n';
    };

    emit(2, 4, 2);
    emit(3, 5, 1);
    emit(1, 6, 3);
    emit(4, 7, 4);
    int emitted = 4;
    while (emitted < operationCount) {
        int nodes = static_cast<int>(randomRange(random, 1, 10));
        emit(static_cast<int>(randomRange(random, 1, 4)), nodes,
             static_cast<int>(randomRange(random, 1, nodes)));
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> powerSums(const std::vector<ll>& values, int degree) {
    std::vector<ll> sums(degree + 1, 0);
    for (ll value : values) {
        ll power = 1;
        for (int i = 1; i <= degree; ++i) {
            power = power * value % kMod;
            sums[i] = (sums[i] + power) % kMod;
        }
    }
    return sums;
}

ll elementaryBrute(const std::vector<ll>& values, int degree) {
    std::vector<ll> coefficients(degree + 1, 0);
    coefficients[0] = 1;
    for (ll value : values) {
        for (int i = degree; i >= 1; --i) {
            coefficients[i] = (coefficients[i] + value * coefficients[i - 1]) % kMod;
        }
    }
    return coefficients[degree];
}

ll completeBrute(const std::vector<ll>& values, int degree) {
    std::vector<ll> coefficients(degree + 1, 0);
    coefficients[0] = 1;
    for (ll value : values) {
        for (int i = 1; i <= degree; ++i) {
            coefficients[i] = (coefficients[i] + value * coefficients[i - 1]) % kMod;
        }
    }
    return coefficients[degree];
}

GeneratedCase generateNewtonIdentities(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M24-newton-identities operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](const std::string& operation,
                    const std::vector<ll>& values,
                    int degree) {
        auto sums = powerSums(values, degree);
        in << operation << ' ' << degree;
        for (int i = 1; i <= degree; ++i) in << ' ' << sums[i];
        in << '\n';
        out << (operation == "elementary"
                    ? elementaryBrute(values, degree)
                    : completeBrute(values, degree)) << '\n';
    };

    emit("elementary", {kMod - 1, 1}, 2);
    emit("elementary", {1, 2}, 2);
    emit("complete", {1, 2}, 2);
    emit("elementary", {3, 4, 5}, 0);
    int emitted = 4;
    while (emitted < operationCount) {
        int variableCount = static_cast<int>(randomRange(random, 1, 6));
        int degree = static_cast<int>(randomRange(random, 0, 8));
        std::vector<ll> values(variableCount);
        for (ll& value : values) value = randomRange(random, 0, 20);
        emit(randomRange(random, 0, 1) == 0 ? "elementary" : "complete",
             values, degree);
        ++emitted;
    }
    return {in.str(), out.str()};
}

}  // namespace

void registerCombinatoricsGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("M16-lucas-binomial", generateLucasBinomial);
    registry.emplace("M17-extended-lucas", generateExtendedLucas);
    registry.emplace(
        "M18-inclusion-exclusion-counting",
        generateInclusionExclusionCounting
    );
    registry.emplace("M19-catalan-families", generateCatalanFamilies);
    registry.emplace("M20-stirling-bell", generateStirlingBell);
    registry.emplace("M21-integer-partitions", generateIntegerPartitions);
    registry.emplace("M22-rational-ogf-recurrence", generateRationalOgf);
    registry.emplace("M23-lagrange-forests", generateLagrangeForests);
    registry.emplace("M24-newton-identities", generateNewtonIdentities);
}

}  // namespace judge::generators
