#include <cstdlib>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;
    auto values =
        std::make_unique<long long[]>(static_cast<std::size_t>(size));
    auto tree = std::make_unique<long long[]>(
        static_cast<std::size_t>(size) * 2
    );
    for (int index = 0; index < size; ++index) {
        std::cin >> values[static_cast<std::size_t>(index)];
        tree[static_cast<std::size_t>(size + index)] =
            std::llabs(values[static_cast<std::size_t>(index)]);
    }
    for (int index = size - 1; index > 0; --index) {
        tree[static_cast<std::size_t>(index)] = std::gcd(
            tree[static_cast<std::size_t>(index * 2)],
            tree[static_cast<std::size_t>(index * 2 + 1)]
        );
    }

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto set = [&](int index, long long value) {
        values[static_cast<std::size_t>(index)] = value;
        int position = size + index;
        tree[static_cast<std::size_t>(position)] = std::llabs(value);
        while (position > 1) {
            position /= 2;
            tree[static_cast<std::size_t>(position)] = std::gcd(
                tree[static_cast<std::size_t>(position * 2)],
                tree[static_cast<std::size_t>(position * 2 + 1)]
            );
        }
    };
    auto query = [&](int left, int right) {
        long long result = 0;
        int begin = left + size;
        int end = right + size + 1;
        while (begin < end) {
            if ((begin & 1) != 0) {
                result = std::gcd(
                    result, tree[static_cast<std::size_t>(begin++)]
                );
            }
            if ((end & 1) != 0) {
                result = std::gcd(
                    result, tree[static_cast<std::size_t>(--end)]
                );
            }
            begin /= 2;
            end /= 2;
        }
        return result;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set" || command == "add") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "set") {
                set(index, value);
            } else {
                set(index, values[static_cast<std::size_t>(index)] + value);
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << values[static_cast<std::size_t>(index)] << '\n';
            }
        } else if (command == "gcd") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << query(left, right) << '\n';
            }
        } else if (command == "all") {
            std::cout << tree[1] << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
