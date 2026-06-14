#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto values = std::make_unique<int[]>(capacity);
    int size = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (size == capacity) {
                std::cout << "FULL\n";
            } else {
                const int writes = size - index + 1;
                for (int cursor = size; cursor > index; --cursor) {
                    values[cursor] = values[cursor - 1];
                }
                values[index] = value;
                ++size;
                std::cout << writes << '\n';
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int removed = values[index];
                const int shifts = size - index - 1;
                for (int cursor = index; cursor + 1 < size; ++cursor) {
                    values[cursor] = values[cursor + 1];
                }
                --size;
                std::cout << removed << ' ' << shifts << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << values[index] << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "capacity") {
            std::cout << capacity << '\n';
        } else if (command == "values") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                for (int index = 0; index < size; ++index) {
                    if (index > 0) std::cout << ' ';
                    std::cout << values[index];
                }
                std::cout << '\n';
            }
        } else if (command == "clear") {
            size = 0;
        }
    }
}
