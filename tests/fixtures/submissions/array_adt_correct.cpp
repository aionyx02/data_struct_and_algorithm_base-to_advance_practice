#include <iostream>
#include <memory>
#include <string>

int main() {
    int size = 0;
    int operationCount = 0;
    std::cin >> size >> operationCount;

    auto values = std::make_unique<int[]>(size);
    for (int index = 0; index < size; ++index) {
        std::cin >> values[index];
    }

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "get") {
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
        } else if (command == "swap") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (left < 0 || left >= size || right < 0 || right >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int temporary = values[left];
                values[left] = values[right];
                values[right] = temporary;
            }
        } else if (command == "front") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[0] << '\n';
            }
        } else if (command == "back") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << values[size - 1] << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
