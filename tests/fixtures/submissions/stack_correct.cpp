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
                std::cout << "FULL\n";
            } else {
                values[size++] = value;
            }
        } else if (command == "pop") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[--size] << '\n';
            }
        } else if (command == "top") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[size - 1] << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        }
    }
}
