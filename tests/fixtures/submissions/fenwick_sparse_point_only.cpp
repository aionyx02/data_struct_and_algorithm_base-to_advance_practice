#include <iostream>
#include <string>
#include <unordered_map>

// Known-wrong: only the point slot is stored. Sparse Fenwick queries still walk
// parent links, so most prefix sums never see the update.
int main() {
    long long universe = 0;
    int q = 0;
    std::cin >> universe >> q;

    std::unordered_map<long long, long long> bit;

    auto update = [&](long long coordinate, long long delta) {
        const long long index = coordinate + 1;
        const long long next = bit[index] + delta;
        if (next == 0) {
            bit.erase(index);
        } else {
            bit[index] = next;
        }
    };
    auto prefix = [&](long long coordinate) {
        long long sum = 0;
        for (long long index = coordinate + 1; index > 0;
             index -= index & -index) {
            const auto found = bit.find(index);
            if (found != bit.end()) {
                sum += found->second;
            }
        }
        return sum;
    };
    auto point = [&](long long coordinate) {
        return prefix(coordinate) -
               (coordinate == 0 ? 0 : prefix(coordinate - 1));
    };
    auto valid = [&](long long coordinate) {
        return coordinate >= 0 && coordinate < universe;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            long long coordinate = 0;
            long long delta = 0;
            std::cin >> coordinate >> delta;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                update(coordinate, delta);
            }
        } else if (command == "set") {
            long long coordinate = 0;
            long long value = 0;
            std::cin >> coordinate >> value;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                update(coordinate, value - point(coordinate));
            }
        } else if (command == "get") {
            long long coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << point(coordinate) << '\n';
            }
        } else if (command == "prefix") {
            long long coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(coordinate) << '\n';
            }
        } else if (command == "range") {
            long long left = 0;
            long long right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const long long before = left == 0 ? 0 : prefix(left - 1);
                std::cout << prefix(right) - before << '\n';
            }
        } else if (command == "nodes") {
            std::cout << bit.size() << '\n';
        }
    }
}
