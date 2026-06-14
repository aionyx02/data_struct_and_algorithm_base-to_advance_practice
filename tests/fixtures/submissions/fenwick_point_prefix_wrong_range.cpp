#include <iostream>
#include <memory>
#include <string>

// Known-wrong: computes range [l, r] as prefix(r) - prefix(l), dropping the
// element at l. This off-by-one differs from prefix(r) - prefix(l - 1).
int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    auto bit = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 1);
    auto cur = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 1);

    auto add = [&](int index, long long delta) {
        for (int x = index + 1; x <= n; x += x & -x) {
            bit[static_cast<std::size_t>(x)] += delta;
        }
    };
    auto prefix = [&](int last) {
        long long sum = 0;
        for (int x = last + 1; x > 0; x -= x & -x) {
            sum += bit[static_cast<std::size_t>(x)];
        }
        return sum;
    };

    for (int index = 0; index < n; ++index) {
        std::cin >> cur[static_cast<std::size_t>(index)];
        add(index, cur[static_cast<std::size_t>(index)]);
    }

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            cur[static_cast<std::size_t>(index)] += value;
            add(index, value);
        } else if (command == "set") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                add(index, value - cur[static_cast<std::size_t>(index)]);
                cur[static_cast<std::size_t>(index)] = value;
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << cur[static_cast<std::size_t>(index)] << '\n';
            }
        } else if (command == "prefix") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(index) << '\n';
            }
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (left < 0 || left >= n || right < 0 || right >= n || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(right) - prefix(left) << '\n';
            }
        } else if (command == "size") {
            std::cout << n << '\n';
        }
    }
}
