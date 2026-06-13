#include <iostream>
#include <memory>
#include <string>

int main() {
    int rows = 0;
    int columns = 0;
    int operationCount = 0;
    std::cin >> rows >> columns >> operationCount;
    const int total = rows * columns;
    auto values = std::make_unique<int[]>(total);
    auto valid = [&](int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    };
    auto index = [&](int row, int column) { return column * rows + row; };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int row = 0;
            int column = 0;
            int value = 0;
            std::cin >> row >> column >> value;
            if (!valid(row, column)) std::cout << "OUT_OF_RANGE\n";
            else values[index(row, column)] = value;
        } else if (command == "get" || command == "index") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) std::cout << "OUT_OF_RANGE\n";
            else if (command == "get") std::cout << values[index(row, column)] << '\n';
            else std::cout << index(row, column) << '\n';
        } else if (command == "coord") {
            int linear = 0;
            std::cin >> linear;
            if (linear < 0 || linear >= total) std::cout << "OUT_OF_RANGE\n";
            else std::cout << linear % rows << ' ' << linear / rows << '\n';
        } else if (command == "fill") {
            int value = 0;
            std::cin >> value;
            for (int linear = 0; linear < total; ++linear) values[linear] = value;
        } else if (command == "rows") {
            std::cout << rows << '\n';
        } else if (command == "columns") {
            std::cout << columns << '\n';
        }
    }
}
