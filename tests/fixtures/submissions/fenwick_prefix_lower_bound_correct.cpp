#include <iostream>
#include <memory>
#include <string>

int main() {
    int n = 0;
    int q = 0;
    std::cin >> n >> q;

    auto bit = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 1);
    auto cur = std::make_unique<long long[]>(static_cast<std::size_t>(n) + 1);

    int logStep = 1;
    while (logStep * 2 <= n) {
        logStep *= 2;
    }

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
    // Smallest zero-based index whose prefix sum is at least target, or -1.
    auto lowerBound = [&](long long target) {
        int position = 0;
        long long remaining = target;
        for (int step = logStep; step > 0; step >>= 1) {
            const int next = position + step;
            if (next <= n && bit[static_cast<std::size_t>(next)] < remaining) {
                position = next;
                remaining -= bit[static_cast<std::size_t>(next)];
            }
        }
        if (position >= n) {
            return -1;
        }
        return position;
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
        } else if (command == "lower_bound") {
            long long target = 0;
            std::cin >> target;
            const int answer = lowerBound(target);
            if (answer < 0) {
                std::cout << "NONE\n";
            } else {
                std::cout << answer << '\n';
            }
        } else if (command == "size") {
            std::cout << n << '\n';
        }
    }
}
