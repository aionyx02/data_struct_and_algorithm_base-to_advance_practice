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

    auto key = [&](int row, int column) { return row * columns + column; };
    auto position = [&](int row, int column) {
        const int target = key(row, column);
        int index = 0;
        while (index < size && key(terms[index].row, terms[index].column) < target) {
            ++index;
        }
        return index;
    };
    auto valid = [&](int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
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
            const int index = position(row, column);
            const bool found =
                index < size && terms[index].row == row && terms[index].column == column;
            if (found && value == 0) {
                for (int next = index + 1; next < size; ++next) {
                    terms[next - 1] = terms[next];
                }
                --size;
            } else if (found) {
                terms[index].value = value;
            } else if (value != 0) {
                if (size == capacity) {
                    std::cout << "FULL\n";
                } else {
                    for (int next = size; next > index; --next) {
                        terms[next] = terms[next - 1];
                    }
                    terms[index] = {row, column, value};
                    ++size;
                }
            }
        } else if (command == "get") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int index = position(row, column);
                std::cout << (index < size && terms[index].row == row &&
                                      terms[index].column == column
                                  ? terms[index].value
                                  : 0)
                          << '\n';
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
