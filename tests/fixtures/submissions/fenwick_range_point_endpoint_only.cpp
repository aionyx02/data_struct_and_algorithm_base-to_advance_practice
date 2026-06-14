#include <iostream>
#include <memory>
#include <string>

// Known-wrong: a range add only bumps the left endpoint of the difference
// array and never cancels at right + 1, so every value from left onward keeps
// the delta instead of just [left, right].
int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    auto bit = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 2);

    auto update = [&](int index, long long delta) {
        for (int x = index + 1; x <= n; x += x & -x) {
            bit[static_cast<std::size_t>(x)] += delta;
        }
    };
    auto point = [&](int index) {
        long long sum = 0;
        for (int x = index + 1; x > 0; x -= x & -x) {
            sum += bit[static_cast<std::size_t>(x)];
        }
        return sum;
    };
    auto rangeAdd = [&](int left, int /*right*/, long long value) {
        update(left, value);
    };

    for (int index = 0; index < n; ++index) {
        long long value = 0;
        std::cin >> value;
        update(index, value);
        if (index + 1 < n) {
            update(index + 1, -value);
        }
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
                rangeAdd(left, right, value);
            }
        } else if (command == "add") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                rangeAdd(index, index, value);
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= n) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << point(index) << '\n';
            }
        } else if (command == "size") {
            std::cout << n << '\n';
        }
    }
}
