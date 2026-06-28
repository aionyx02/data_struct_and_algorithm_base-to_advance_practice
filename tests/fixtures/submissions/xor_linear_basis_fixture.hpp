#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef XOR_BASIS_COUNTS_DEPENDENT
#define XOR_BASIS_COUNTS_DEPENDENT 0
#endif
#ifndef XOR_BASIS_ZERO_NOT_REPRESENTABLE
#define XOR_BASIS_ZERO_NOT_REPRESENTABLE 0
#endif

using xor64 = long long;

struct XorBasisFixture {
    std::vector<xor64> basis = std::vector<xor64>(61, 0);

    void insert(xor64 value) {
        xor64 current = value;
        for (int bit = 60; bit >= 0; --bit) {
            if (((current >> bit) & 1LL) == 0) continue;
            if (basis[bit] == 0) {
                basis[bit] = current;
                return;
            }
            current ^= basis[bit];
        }
    }

    int rank(const std::vector<xor64>& values) const {
        if (XOR_BASIS_COUNTS_DEPENDENT) {
            int count = 0;
            for (xor64 value : values) {
                if (value != 0) ++count;
            }
            return count;
        }
        int count = 0;
        for (xor64 value : basis) {
            if (value != 0) ++count;
        }
        return count;
    }

    xor64 maximum() const {
        xor64 answer = 0;
        for (int bit = 60; bit >= 0; --bit) {
            answer = std::max(answer, answer ^ basis[bit]);
        }
        return answer;
    }

    bool represents(xor64 target) const {
        if (target == 0 && XOR_BASIS_ZERO_NOT_REPRESENTABLE) return false;
        xor64 current = target;
        for (int bit = 60; bit >= 0; --bit) {
            if (((current >> bit) & 1LL) != 0) current ^= basis[bit];
        }
        return current == 0;
    }
};

int main() {
    int queryCount;
    if (!(std::cin >> queryCount)) return 0;
    for (int query = 0; query < queryCount; ++query) {
        std::string operation;
        int count;
        xor64 target;
        std::cin >> operation >> count >> target;
        std::vector<xor64> values(count, 0);
        XorBasisFixture basis;
        for (xor64& value : values) {
            std::cin >> value;
            basis.insert(value);
        }
        std::cout << basis.rank(values) << ' ' << basis.maximum() << ' '
                  << (basis.represents(target) ? "YES" : "NO") << '\n';
    }
    return 0;
}
