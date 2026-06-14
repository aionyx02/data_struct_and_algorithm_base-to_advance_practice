#include <iostream>
#include <memory>
#include <string>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto values = std::make_unique<int[]>(capacity);
    int size = 0;
    long long totalCopies = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push") {
            int value = 0;
            std::cin >> value;
            int copies = 0;
            if (size == capacity) {
                const int newCapacity = capacity * 2;
                auto expanded = std::make_unique<int[]>(newCapacity);
                for (int index = 0; index < size; ++index) {
                    expanded[index] = values[index];
                }
                copies = size + 1;
                totalCopies += copies;
                values = std::move(expanded);
                capacity = newCapacity;
            }
            values[size++] = value;
            std::cout << copies << '\n';
        } else if (command == "pop") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << values[--size] << '\n';
        } else if (command == "reserve") {
            int requested = 0;
            std::cin >> requested;
            if (requested <= capacity) {
                std::cout << "UNCHANGED\n";
            } else {
                auto expanded = std::make_unique<int[]>(requested);
                for (int index = 0; index < size; ++index) {
                    expanded[index] = values[index];
                }
                totalCopies += size;
                values = std::move(expanded);
                capacity = requested;
                std::cout << size << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "capacity") {
            std::cout << capacity << '\n';
        } else if (command == "total_copies") {
            std::cout << totalCopies << '\n';
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
