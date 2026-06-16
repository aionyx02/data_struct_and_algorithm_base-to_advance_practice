#include <iostream>
#include <memory>
#include <string>

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;

    auto tree = std::make_unique<long long[]>(
        static_cast<std::size_t>(size) * 2
    );
    for (int index = 0; index < size; ++index) {
        std::cin >> tree[static_cast<std::size_t>(size + index)];
    }
    for (int index = size - 1; index > 0; --index) {
        tree[static_cast<std::size_t>(index)] =
            tree[static_cast<std::size_t>(index * 2)] +
            tree[static_cast<std::size_t>(index * 2 + 1)];
    }

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };
    // Known-wrong: updates change only the leaf and leave all parent aggregates
    // stale.
    auto set = [&](int index, long long value) {
        tree[static_cast<std::size_t>(size + index)] = value;
    };
    auto range = [&](int left, int right) {
        long long sum = 0;
        int begin = left + size;
        int end = right + size + 1;
        while (begin < end) {
            if ((begin & 1) != 0) {
                sum += tree[static_cast<std::size_t>(begin++)];
            }
            if ((end & 1) != 0) {
                sum += tree[static_cast<std::size_t>(--end)];
            }
            begin /= 2;
            end /= 2;
        }
        return sum;
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
        } else if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                set(
                    index,
                    tree[static_cast<std::size_t>(size + index)] + delta
                );
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout
                    << tree[static_cast<std::size_t>(size + index)] << '\n';
            }
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << range(left, right) << '\n';
            }
        } else if (command == "all") {
            std::cout << tree[1] << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
