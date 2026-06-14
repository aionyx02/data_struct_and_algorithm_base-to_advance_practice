#include <iostream>
#include <memory>
#include <string>

int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    // Two Fenwick trees over 1-based indices. b1 holds the difference, b2 holds
    // the index * delta correction so a prefix sum stays exact under range add.
    auto b1 = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 2);
    auto b2 = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 2);

    auto update = [&](long long* tree, int index, long long delta) {
        for (int x = index; x <= n; x += x & -x) {
            tree[static_cast<std::size_t>(x)] += delta;
        }
    };
    auto query = [&](long long* tree, int index) {
        long long sum = 0;
        for (int x = index; x > 0; x -= x & -x) {
            sum += tree[static_cast<std::size_t>(x)];
        }
        return sum;
    };
    // left and right are 1-based inclusive.
    auto rangeAdd = [&](int left, int right, long long value) {
        update(b1.get(), left, value);
        update(b1.get(), right + 1, -value);
        update(b2.get(), left, value * (left - 1));
        update(b2.get(), right + 1, -value * right);
    };
    // prefix sum over 1-based indices [1, index].
    auto prefix = [&](int index) {
        return query(b1.get(), index) * index - query(b2.get(), index);
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
                std::cout << prefix(right + 1) - prefix(left) << '\n';
            }
        } else if (command == "prefix") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(index + 1) << '\n';
            }
        } else if (command == "size") {
            std::cout << n << '\n';
        }
    }
}
