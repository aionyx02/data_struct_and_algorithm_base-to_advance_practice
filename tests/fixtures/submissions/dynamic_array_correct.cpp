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
        if (command == "push") {
            int value = 0;
            std::cin >> value;
            if (size == capacity) {
                const int newCapacity = capacity * 2;
                auto expanded = std::make_unique<int[]>(newCapacity);
                for (int index = 0; index < size; ++index) {
                    expanded[index] = values[index];
                }
                values = std::move(expanded);
                capacity = newCapacity;
            }
            values[size++] = value;
        } else if (command == "pop") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[--size] << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << values[index] << '\n';
            }
        } else if (command == "set") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                values[index] = value;
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "capacity") {
            std::cout << capacity << '\n';
        } else if (command == "clear") {
            size = 0;
        }
    }
}
