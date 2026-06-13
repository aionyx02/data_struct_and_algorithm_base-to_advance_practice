#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;

    auto values = std::make_unique<int[]>(capacity);
    int frontIndex = 0;
    int size = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front") {
            int value = 0;
            std::cin >> value;
            if (size == capacity) {
                std::cout << "FULL\n";
            } else {
                frontIndex = (frontIndex - 1 + capacity) % capacity;
                values[frontIndex] = value;
                ++size;
            }
        } else if (command == "push_back") {
            int value = 0;
            std::cin >> value;
            if (size == capacity) {
                std::cout << "FULL\n";
            } else {
                values[(frontIndex + size) % capacity] = value;
                ++size;
            }
        } else if (command == "pop_front") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[frontIndex] << '\n';
                frontIndex = (frontIndex + 1) % capacity;
                --size;
            }
        } else if (command == "pop_back") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                const int backIndex = (frontIndex + size - 1) % capacity;
                std::cout << values[backIndex] << '\n';
                --size;
            }
        } else if (command == "front") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[frontIndex] << '\n';
            }
        } else if (command == "back") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[(frontIndex + size - 1) % capacity] << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        }
    }
}
