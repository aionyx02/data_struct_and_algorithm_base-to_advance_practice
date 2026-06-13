#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto values = std::make_unique<int[]>(capacity);
    int leftTop = -1;
    int rightTop = capacity;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_left" || command == "push_right") {
            int value = 0;
            std::cin >> value;
            if (leftTop + 1 == rightTop) {
                std::cout << "FULL\n";
            } else if (command == "push_left") {
                values[++leftTop] = value;
            } else {
                values[--rightTop] = value;
            }
        } else if (command == "pop_left") {
            if (leftTop == -1) std::cout << "EMPTY\n";
            else std::cout << values[leftTop--] << '\n';
        } else if (command == "pop_right") {
            if (rightTop == capacity) std::cout << "EMPTY\n";
            else std::cout << values[rightTop++] << '\n';
        } else if (command == "top_left") {
            if (leftTop == -1) std::cout << "EMPTY\n";
            else std::cout << values[leftTop] << '\n';
        } else if (command == "top_right") {
            if (rightTop == capacity) std::cout << "EMPTY\n";
            else std::cout << values[rightTop] << '\n';
        } else if (command == "size_left") {
            std::cout << leftTop + 1 << '\n';
        } else if (command == "size_right") {
            std::cout << capacity - rightTop << '\n';
        } else if (command == "free") {
            std::cout << rightTop - leftTop - 1 << '\n';
        } else if (command == "clear") {
            leftTop = -1;
            rightTop = capacity;
        }
    }
}
