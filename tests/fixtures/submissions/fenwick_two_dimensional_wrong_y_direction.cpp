#include <iostream>
#include <memory>
#include <string>

// Known-wrong: the inner update loop walks toward zero, so updates never reach
// the Fenwick ancestors for larger column prefixes.
int main() {
    int rows = 0;
    int columns = 0;
    int q = 0;
    std::cin >> rows >> columns >> q;

    const int bitColumns = columns + 1;
    auto bit = std::make_unique<long long[]>(
        static_cast<std::size_t>(rows + 1) * bitColumns
    );
    auto values = std::make_unique<long long[]>(
        static_cast<std::size_t>(rows) * columns
    );

    auto bitIndex = [&](int row, int column) {
        return static_cast<std::size_t>(row) * bitColumns + column;
    };
    auto valueIndex = [&](int row, int column) {
        return static_cast<std::size_t>(row) * columns + column;
    };
    auto valid = [&](int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    };
    auto update = [&](int row, int column, long long delta) {
        for (int x = row + 1; x <= rows; x += x & -x) {
            for (int y = column + 1; y > 0; y -= y & -y) {
                bit[bitIndex(x, y)] += delta;
            }
        }
    };
    auto prefix = [&](int row, int column) {
        long long sum = 0;
        for (int x = row + 1; x > 0; x -= x & -x) {
            for (int y = column + 1; y > 0; y -= y & -y) {
                sum += bit[bitIndex(x, y)];
            }
        }
        return sum;
    };
    auto rectangle = [&](int top, int left, int bottom, int right) {
        return prefix(bottom, right) - prefix(top - 1, right) -
               prefix(bottom, left - 1) + prefix(top - 1, left - 1);
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            int row = 0;
            int column = 0;
            long long delta = 0;
            std::cin >> row >> column >> delta;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                values[valueIndex(row, column)] += delta;
                update(row, column, delta);
            }
        } else if (command == "set") {
            int row = 0;
            int column = 0;
            long long value = 0;
            std::cin >> row >> column >> value;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const std::size_t index = valueIndex(row, column);
                const long long delta = value - values[index];
                values[index] = value;
                update(row, column, delta);
            }
        } else if (command == "get") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << values[valueIndex(row, column)] << '\n';
            }
        } else if (command == "prefix") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << prefix(row, column) << '\n';
            }
        } else if (command == "rectangle") {
            int top = 0;
            int left = 0;
            int bottom = 0;
            int right = 0;
            std::cin >> top >> left >> bottom >> right;
            if (!valid(top, left) || !valid(bottom, right) || top > bottom ||
                left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << rectangle(top, left, bottom, right) << '\n';
            }
        } else if (command == "shape") {
            std::cout << rows << ' ' << columns << '\n';
        }
    }
}
