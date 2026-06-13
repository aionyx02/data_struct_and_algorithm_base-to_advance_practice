#include <iostream>
#include <memory>
#include <string>

struct Triplet {
    int row = 0;
    int column = 0;
    int value = 0;
};

int main() {
    int rows = 0;
    int columns = 0;
    int capacity = 0;
    int operationCount = 0;
    std::cin >> rows >> columns >> capacity >> operationCount;
    auto terms = std::make_unique<Triplet[]>(capacity);
    int size = 0;

    auto valid = [&](int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    };
    auto find = [&](int row, int column) {
        for (int index = 0; index < size; ++index) {
            if (terms[index].row == row && terms[index].column == column) return index;
        }
        return -1;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int row = 0;
            int column = 0;
            int value = 0;
            std::cin >> row >> column >> value;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int index = find(row, column);
            if (index != -1 && value == 0) {
                for (int next = index + 1; next < size; ++next) terms[next - 1] = terms[next];
                --size;
            } else if (index != -1) {
                terms[index].value = value;
            } else if (value != 0) {
                if (size == capacity) std::cout << "FULL\n";
                else terms[size++] = {row, column, value};
            }
        } else if (command == "get") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) std::cout << "OUT_OF_RANGE\n";
            else {
                const int index = find(row, column);
                std::cout << (index == -1 ? 0 : terms[index].value) << '\n';
            }
        } else if (command == "term") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << terms[index].row << ' ' << terms[index].column
                           << ' ' << terms[index].value << '\n';
        } else if (command == "nonzero") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            size = 0;
        }
    }
}
