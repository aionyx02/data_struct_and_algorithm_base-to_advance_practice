#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    int size = 0;
    int operationCount = 0;
    std::cin >> size >> operationCount;
    auto heap = std::make_unique<int[]>(size == 0 ? 1 : size);
    for (int index = 0; index < size; ++index) std::cin >> heap[index];
    auto siftDown = [&](int start) {
        int index = start;
        while (true) {
            int smallest = index;
            const int left = 2 * index + 1;
            const int right = left + 1;
            if (left < size && heap[left] < heap[smallest]) smallest = left;
            if (right < size && heap[right] < heap[smallest]) smallest = right;
            if (smallest == index) break;
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    };
    for (int index = size / 2 - 1; index >= 0; --index) siftDown(index);
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "pop") {
            if (size == 0) std::cout << "EMPTY\n";
            else {
                std::cout << heap[0] << '\n';
                heap[0] = heap[--size];
                if (size != 0) siftDown(0);
            }
        } else if (command == "top") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << heap[0] << '\n';
        } else if (command == "array") {
            if (size == 0) std::cout << "EMPTY\n";
            else {
                for (int index = 0; index < size; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << heap[index];
                }
                std::cout << '\n';
            }
        } else if (command == "at") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << heap[index] << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "clear") {
            size = 0;
        }
    }
}
