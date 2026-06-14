#include <iostream>
#include <memory>
#include <string>

// Known-wrong: uses a single difference tree and treats its prefix query as a
// range sum. That returns the point value at the index, not the running sum,
// so range_sum and prefix are wrong after any range add.
int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    auto b1 = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 2);

    auto update = [&](int index, long long delta) {
        for (int x = index; x <= n; x += x & -x) {
            b1[static_cast<std::size_t>(x)] += delta;
        }
    };
    auto query = [&](int index) {
        long long sum = 0;
        for (int x = index; x > 0; x -= x & -x) {
            sum += b1[static_cast<std::size_t>(x)];
        }
        return sum;
    };
    auto rangeAdd = [&](int left, int right, long long value) {
        update(left, value);
        update(right + 1, -value);
    };

    for (int index = 0; index < n; ++index) {
        long long value = 0;
        std::cin >> value;
        rangeAdd(index + 1, index + 1, value);
    }

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "range_add") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (left < 0 || left >= n || right < 0 || right >= n || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                rangeAdd(left + 1, right + 1, value);
            }
        } else if (command == "range_sum") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (left < 0 || left >= n || right < 0 || right >= n || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << query(right + 1) - query(left) << '\n';
            }
        } else if (command == "prefix") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << query(index + 1) << '\n';
            }
        } else if (command == "size") {
            std::cout << n << '\n';
        }
    }
}
