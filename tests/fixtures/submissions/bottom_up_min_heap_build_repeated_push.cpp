#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    int size = 0;
    int operationCount = 0;
    std::cin >> size >> operationCount;
    auto heap = std::make_unique<int[]>(size == 0 ? 1 : size);
    int built = 0;
    for (int inputIndex = 0; inputIndex < size; ++inputIndex) {
        int value = 0;
        std::cin >> value;
        int index = built++;
        heap[index] = value;
        while (index > 0) {
            const int parent = (index - 1) / 2;
            if (heap[parent] <= heap[index]) break;
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    }
    auto validIndex = [&](int index) { return index >= 0 && index < size; };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "array") {
            if (size == 0) std::cout << "EMPTY\n";
            else {
                for (int index = 0; index < size; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << heap[index];
                }
                std::cout << '\n';
            }
        } else if (command == "top") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << heap[0] << '\n';
        } else if (command == "at") {
            int index = 0;
            std::cin >> index;
            if (!validIndex(index)) std::cout << "OUT_OF_RANGE\n";
            else std::cout << heap[index] << '\n';
        } else if (command == "parent" || command == "left" ||
                   command == "right") {
            int index = 0;
            std::cin >> index;
            if (!validIndex(index)) std::cout << "OUT_OF_RANGE\n";
            else {
                const int relative = command == "parent"
                    ? (index == 0 ? -1 : (index - 1) / 2)
                    : (command == "left" ? 2 * index + 1 : 2 * index + 2);
                if (!validIndex(relative)) std::cout << "NONE\n";
                else std::cout << relative << '\n';
            }
        } else if (command == "size") std::cout << size << '\n';
        else if (command == "valid") std::cout << "true\n";
    }
}
