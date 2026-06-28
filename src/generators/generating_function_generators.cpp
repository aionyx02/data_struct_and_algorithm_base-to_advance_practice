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

ll powerMod(ll base, ll exponent) {
    ll result = 1;
    while (exponent > 0) {
        if (exponent & 1) result = static_cast<ll>(static_cast<i128>(result) * base % kMod);
        base = static_cast<ll>(static_cast<i128>(base) * base % kMod);
        exponent >>= 1;
    }
    return result;
}

ll restrictedCyclePermutationsBrute(int n, const std::vector<int>& allowed) {
    std::vector<bool> isAllowed(n + 1, false);
    for (int length : allowed) {
        if (length <= n) isAllowed[length] = true;
    }
    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);
    ll count = 0;
    do {
        std::vector<bool> seen(n, false);
        bool valid = true;
        for (int start = 0; start < n && valid; ++start) {
            if (seen[start]) continue;
            int length = 0;
            int current = start;
            while (!seen[current]) {
                seen[current] = true;
                current = permutation[current];
                ++length;
            }
            if (!isAllowed[length]) valid = false;
        }
        if (valid) ++count;
    } while (std::next_permutation(permutation.begin(), permutation.end()));
    return count % kMod;
}

GeneratedCase generateEgfCycleRestrictions(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error("M25-egf-cycle-restrictions operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](int n, std::vector<int> allowed) {
        std::sort(allowed.begin(), allowed.end());
        allowed.erase(std::unique(allowed.begin(), allowed.end()), allowed.end());
        in << "cycles " << n << ' ' << allowed.size();
        for (int length : allowed) in << ' ' << length;
        in << '\n';
        out << restrictedCyclePermutationsBrute(n, allowed) << '\n';
    };

    emit(3, {3});
    emit(4, {1, 3});
    emit(0, {});
    emit(5, {1, 2});
    int emitted = 4;
    while (emitted < operationCount) {
        int n = static_cast<int>(randomRange(random, 0, 8));
        std::vector<int> allowed;
        for (int length = 1; length <= std::max(1, n); ++length) {
            if (randomRange(random, 0, 1) == 1) allowed.push_back(length);
        }
        emit(n, allowed);
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> multiply(
    const std::vector<ll>& left,
    const std::vector<ll>& right,
    int limit
) {
    std::vector<ll> result(limit + 1, 0);
    for (int i = 0; i < static_cast<int>(left.size()) && i <= limit; ++i) {
        for (int j = 0; j < static_cast<int>(right.size()) && i + j <= limit; ++j) {
            result[i + j] = (result[i + j] + left[i] * right[j]) % kMod;
        }
    }
    return result;
}

ll compositionCoefficientBrute(
    const std::vector<ll>& outer,
    const std::vector<ll>& inner,
    int target
) {
    std::vector<ll> power(target + 1, 0);
    power[0] = 1;
    ll answer = 0;
    for (int degree = 0; degree < static_cast<int>(outer.size()); ++degree) {
        answer = (answer + outer[degree] * power[target]) % kMod;
        power = multiply(power, inner, target);
    }
    return answer;
}

GeneratedCase generateFormalComposition(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M26-formal-composition operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](std::vector<ll> outer, std::vector<ll> inner, int target) {
        int degree = static_cast<int>(outer.size()) - 1;
        in << "compose " << degree << ' ' << target;
        for (ll value : outer) in << ' ' << value;
        for (ll value : inner) in << ' ' << value;
        in << '\n';
        out << compositionCoefficientBrute(outer, inner, target) << '\n';
    };

    emit({5, 7, 11, 13}, {0, 1, 0, 0}, 0);
    emit({0, 1, 0}, {0, 1, 1}, 2);
    emit({1, 2, 3}, {0, 2, 1}, 2);
    emit({3, 1, 4, 1}, {0, 1, 1, 1}, 3);
    int emitted = 4;
    while (emitted < operationCount) {
        int degree = static_cast<int>(randomRange(random, 0, 8));
        std::vector<ll> outer(degree + 1);
        std::vector<ll> inner(degree + 1);
        for (ll& value : outer) value = randomRange(random, 0, 20);
        inner[0] = 0;
        for (int i = 1; i <= degree; ++i) inner[i] = randomRange(random, 0, 10);
        emit(outer, inner, static_cast<int>(randomRange(random, 0, degree)));
        ++emitted;
    }
    return {in.str(), out.str()};
}

std::vector<ll> expByDefinition(const std::vector<ll>& series) {
    int degree = static_cast<int>(series.size()) - 1;
    std::vector<ll> answer(degree + 1, 0);
    std::vector<ll> power(degree + 1, 0);
    power[0] = 1;
    answer[0] = 1;
    ll inverseFactorial = 1;
    for (int count = 1; count <= degree; ++count) {
        power = multiply(power, series, degree);
        inverseFactorial = inverseFactorial * powerMod(count, kMod - 2) % kMod;
        for (int i = 0; i <= degree; ++i) {
            answer[i] = (answer[i] + power[i] * inverseFactorial) % kMod;
        }
    }
    return answer;
}

std::vector<ll> logByDefinition(const std::vector<ll>& series) {
    int degree = static_cast<int>(series.size()) - 1;
    std::vector<ll> shifted = series;
    shifted[0] = (shifted[0] + kMod - 1) % kMod;
    std::vector<ll> answer(degree + 1, 0);
    std::vector<ll> power(degree + 1, 0);
    power[0] = 1;
    for (int count = 1; count <= degree; ++count) {
        power = multiply(power, shifted, degree);
        ll inverse = powerMod(count, kMod - 2);
        for (int i = 0; i <= degree; ++i) {
            ll term = power[i] * inverse % kMod;
            answer[i] = (answer[i] + (count & 1 ? term : kMod - term)) % kMod;
        }
    }
    return answer;
}

GeneratedCase generateFormalExpLog(std::mt19937_64& random, int operationCount) {
    if (operationCount < 4) {
        throw std::runtime_error("M27-formal-exp-log operation_limit is too small");
    }
    std::ostringstream in;
    std::ostringstream out;
    in << operationCount << '\n';
    auto emit = [&](const std::string& operation, std::vector<ll> series) {
        int degree = static_cast<int>(series.size()) - 1;
        in << operation << ' ' << degree;
        for (ll value : series) in << ' ' << value;
        in << '\n';
        auto answer = operation == "exp" ? expByDefinition(series) : logByDefinition(series);
        for (int i = 0; i <= degree; ++i) {
            if (i) out << ' ';
            out << answer[i];
        }
        out << '\n';
    };

    emit("exp", {0, 1, 1, 0, 0});
    emit("log", {1, 1, 0, 0, 0});
    emit("exp", {0, 2, 3, 4});
    emit("log", {1, 2, 3, 4});
    int emitted = 4;
    while (emitted < operationCount) {
        int degree = static_cast<int>(randomRange(random, 0, 8));
        std::vector<ll> series(degree + 1);
        bool useExp = randomRange(random, 0, 1) == 0;
        series[0] = useExp ? 0 : 1;
        for (int i = 1; i <= degree; ++i) series[i] = randomRange(random, 0, 20);
        emit(useExp ? "exp" : "log", series);
        ++emitted;
    }
    return {in.str(), out.str()};
}

}  // namespace

void registerGeneratingFunctionGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("M25-egf-cycle-restrictions", generateEgfCycleRestrictions);
    registry.emplace("M26-formal-composition", generateFormalComposition);
    registry.emplace("M27-formal-exp-log", generateFormalExpLog);
}

}  // namespace judge::generators
