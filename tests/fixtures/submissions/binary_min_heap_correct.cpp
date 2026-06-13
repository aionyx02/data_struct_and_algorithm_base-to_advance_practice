#include <iostream>
#include <memory>
#include <string>
#include <utility>

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto heap = std::make_unique<int[]>(capacity);
    int size = 0;

    auto siftUp = [&](int index) {
        while (index > 0) {
            const int parent = (index - 1) / 2;
            if (heap[parent] <= heap[index]) break;
            std::swap(heap[parent], heap[index]);
            index = parent;
        }
    };
    auto siftDown = [&](int index) {
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

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push") {
            int value = 0;
            std::cin >> value;
            if (size == capacity) std::cout << "FULL\n";
            else {
                heap[size] = value;
                siftUp(size++);
            }
        } else if (command == "pop") {
            if (size == 0) std::cout << "EMPTY\n";
            else {
                std::cout << heap[0] << '\n';
                heap[0] = heap[--size];
                if (size != 0) siftDown(0);
            }
        } else if (command == "top") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << heap[0] << '\n';
        } else if (command == "change") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else {
                const int old = heap[index];
                heap[index] = value;
                if (value < old) siftUp(index);
                else siftDown(index);
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
