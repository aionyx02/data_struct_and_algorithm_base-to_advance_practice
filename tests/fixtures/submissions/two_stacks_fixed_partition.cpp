#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto values = std::make_unique<int[]>(capacity);
    const int leftLimit = capacity / 2;
    int leftSize = 0;
    int rightSize = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_left" || command == "push_right") {
            int value = 0;
            std::cin >> value;
            if (command == "push_left") {
                if (leftSize == leftLimit) std::cout << "FULL\n";
                else values[leftSize++] = value;
            } else {
                if (rightSize == capacity - leftLimit) std::cout << "FULL\n";
                else values[capacity - 1 - rightSize++] = value;
            }
        } else if (command == "pop_left") {
            if (leftSize == 0) std::cout << "EMPTY\n";
            else std::cout << values[--leftSize] << '\n';
        } else if (command == "pop_right") {
            if (rightSize == 0) std::cout << "EMPTY\n";
            else std::cout << values[capacity - rightSize--] << '\n';
        } else if (command == "top_left") {
            if (leftSize == 0) std::cout << "EMPTY\n";
            else std::cout << values[leftSize - 1] << '\n';
        } else if (command == "top_right") {
            if (rightSize == 0) std::cout << "EMPTY\n";
            else std::cout << values[capacity - rightSize] << '\n';
        } else if (command == "size_left") {
            std::cout << leftSize << '\n';
        } else if (command == "size_right") {
            std::cout << rightSize << '\n';
        } else if (command == "free") {
            std::cout << capacity - leftSize - rightSize << '\n';
        } else if (command == "clear") {
            leftSize = rightSize = 0;
        }
    }
}
