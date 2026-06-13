#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;

    auto values = std::make_unique<int[]>(capacity);
    int frontIndex = 0;
    int nextIndex = 0;
    int size = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "enqueue") {
            int value = 0;
            std::cin >> value;
            if (nextIndex == capacity) {
                std::cout << "FULL\n";
            } else {
                values[nextIndex++] = value;
                ++size;
            }
        } else if (command == "dequeue") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[frontIndex++] << '\n';
                --size;
            }
        } else if (command == "front") {
            std::cout << (size == 0 ? 0 : values[frontIndex]) << '\n';
        } else if (command == "back") {
            std::cout << (size == 0 ? 0 : values[nextIndex - 1]) << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        }
    }
}
