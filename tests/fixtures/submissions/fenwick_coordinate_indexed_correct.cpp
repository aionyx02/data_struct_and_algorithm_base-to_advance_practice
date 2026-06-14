#include <iostream>
#include <memory>
#include <string>

int main() {
    int minimum = 0;
    int maximum = 0;
    int q = 0;
    std::cin >> minimum >> maximum >> q;

    const int span = maximum - minimum + 1;
    auto bit = std::make_unique<long long[]>(static_cast<std::size_t>(span) + 1);
    auto values = std::make_unique<long long[]>(static_cast<std::size_t>(span));

    auto offset = [&](int coordinate) {
        return coordinate - minimum;
    };
    auto update = [&](int coordinate, long long delta) {
        for (int index = offset(coordinate) + 1; index <= span;
             index += index & -index) {
            bit[static_cast<std::size_t>(index)] += delta;
        }
    };
    auto prefix = [&](int coordinate) {
        long long sum = 0;
        for (int index = offset(coordinate) + 1; index > 0;
             index -= index & -index) {
            sum += bit[static_cast<std::size_t>(index)];
        }
        return sum;
    };
    auto valid = [&](int coordinate) {
        return coordinate >= minimum && coordinate <= maximum;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            int coordinate = 0;
            long long delta = 0;
            std::cin >> coordinate >> delta;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                values[static_cast<std::size_t>(offset(coordinate))] += delta;
                update(coordinate, delta);
            }
        } else if (command == "set") {
            int coordinate = 0;
            long long value = 0;
            std::cin >> coordinate >> value;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int index = offset(coordinate);
                const long long delta =
                    value - values[static_cast<std::size_t>(index)];
                values[static_cast<std::size_t>(index)] = value;
                update(coordinate, delta);
            }
        } else if (command == "get") {
            int coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << values[static_cast<std::size_t>(offset(coordinate))]
                          << '\n';
            }
        } else if (command == "prefix") {
            int coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(coordinate) << '\n';
            }
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const long long before =
                    left == minimum ? 0 : prefix(left - 1);
                std::cout << prefix(right) - before << '\n';
            }
        } else if (command == "bounds") {
            std::cout << minimum << ' ' << maximum << '\n';
        }
    }
}
