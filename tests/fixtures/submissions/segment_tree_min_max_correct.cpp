#include <algorithm>
#include <iostream>
#include <limits>
#include <memory>
#include <string>

struct Aggregate {
    long long minimum;
    long long maximum;
};

Aggregate combine(Aggregate left, Aggregate right) {
    return {
        std::min(left.minimum, right.minimum),
        std::max(left.maximum, right.maximum)
    };
}

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;
    auto tree = std::make_unique<Aggregate[]>(
        static_cast<std::size_t>(size) * 2
    );
    for (int index = 0; index < size; ++index) {
        long long value = 0;
        std::cin >> value;
        tree[static_cast<std::size_t>(size + index)] = {value, value};
    }
    for (int index = size - 1; index > 0; --index) {
        tree[static_cast<std::size_t>(index)] = combine(
            tree[static_cast<std::size_t>(index * 2)],
            tree[static_cast<std::size_t>(index * 2 + 1)]
        );
    }

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    auto set = [&](int index, long long value) {
        int position = size + index;
        tree[static_cast<std::size_t>(position)] = {value, value};
        while (position > 1) {
            position /= 2;
            tree[static_cast<std::size_t>(position)] = combine(
                tree[static_cast<std::size_t>(position * 2)],
                tree[static_cast<std::size_t>(position * 2 + 1)]
            );
        }
    };
    auto query = [&](int left, int right) {
        const Aggregate identity{
            std::numeric_limits<long long>::max(),
            std::numeric_limits<long long>::min()
        };
        Aggregate leftResult = identity;
        Aggregate rightResult = identity;
        int begin = left + size;
        int end = right + size + 1;
        while (begin < end) {
            if ((begin & 1) != 0) {
                leftResult = combine(
                    leftResult, tree[static_cast<std::size_t>(begin++)]
                );
            }
            if ((end & 1) != 0) {
                rightResult = combine(
                    tree[static_cast<std::size_t>(--end)], rightResult
                );
            }
            begin /= 2;
            end /= 2;
        }
        return combine(leftResult, rightResult);
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                set(index, value);
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout
                    << tree[static_cast<std::size_t>(size + index)].minimum
                    << '\n';
            }
        } else if (
            command == "minimum" || command == "maximum" ||
            command == "bounds"
        ) {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const Aggregate result = query(left, right);
                if (command == "minimum") {
                    std::cout << result.minimum << '\n';
                } else if (command == "maximum") {
                    std::cout << result.maximum << '\n';
                } else {
                    std::cout << result.minimum << ' ' << result.maximum
                              << '\n';
                }
            }
        } else if (command == "all") {
            std::cout << tree[1].minimum << ' ' << tree[1].maximum << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
