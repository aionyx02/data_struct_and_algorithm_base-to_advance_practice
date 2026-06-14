#include <iostream>
#include <memory>
#include <string>

// Known-wrong: `less x` reports the count of elements <= x (prefix(x)) instead
// of strictly less than x (prefix(x - 1)), so any rank query on a stored key is
// inflated by that key's own occurrences.
int main() {
    int universe = 0;
    int q = 0;
    std::cin >> universe >> q;

    // One extra slot so the buggy `less U` (which reads prefix(U)) stays in
    // bounds and produces a clean wrong answer rather than a crash.
    auto bit = std::make_unique<long long[]>(static_cast<std::size_t>(universe) + 2);
    auto cnt = std::make_unique<long long[]>(static_cast<std::size_t>(universe) + 1);
    long long total = 0;

    int logStep = 1;
    while (logStep * 2 <= universe) {
        logStep *= 2;
    }

    auto update = [&](int key, long long delta) {
        for (int x = key + 1; x <= universe; x += x & -x) {
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
    auto kth = [&](long long order) {
        if (order < 1 || order > total) {
            return -1;
        }
        int position = 0;
        long long remaining = order;
        for (int step = logStep; step > 0; step >>= 1) {
            const int next = position + step;
            if (next <= universe &&
                bit[static_cast<std::size_t>(next)] < remaining) {
                position = next;
                remaining -= bit[static_cast<std::size_t>(next)];
            }
        }
        return position;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            if (key < 0 || key >= universe) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                cnt[static_cast<std::size_t>(key)] += 1;
                update(key, 1);
                total += 1;
            }
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            if (key < 0 || key >= universe) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (cnt[static_cast<std::size_t>(key)] == 0) {
                std::cout << "NOT_FOUND\n";
            } else {
                cnt[static_cast<std::size_t>(key)] -= 1;
                update(key, -1);
                total -= 1;
            }
        } else if (command == "count") {
            int key = 0;
            std::cin >> key;
            if (key < 0 || key >= universe) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << cnt[static_cast<std::size_t>(key)] << '\n';
            }
        } else if (command == "less") {
            int bound = 0;
            std::cin >> bound;
            if (bound < 0 || bound > universe) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(bound) << '\n';
            }
        } else if (command == "kth") {
            long long order = 0;
            std::cin >> order;
            const int key = kth(order);
            if (key < 0) {
                std::cout << "NONE\n";
            } else {
                std::cout << key << '\n';
            }
        } else if (command == "size") {
            std::cout << total << '\n';
        }
    }
}
