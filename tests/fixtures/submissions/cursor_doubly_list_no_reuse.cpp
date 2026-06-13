#include <iostream>
#include <memory>
#include <string>

struct CursorNode {
    int value = 0;
    int previous = -1;
    int next = -1;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto nodes = std::make_unique<CursorNode[]>(capacity);
    int used = 0;
    int head = -1;
    int tail = -1;
    int size = 0;

    auto nodeAt = [&](int index) {
        int cursor = head;
        for (int step = 0; step < index; ++step) cursor = nodes[cursor].next;
        return cursor;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front" || command == "push_back" ||
            command == "insert") {
            int index = command == "push_front" ? 0 : size;
            int value = 0;
            if (command == "insert") std::cin >> index >> value;
            else std::cin >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (used == capacity) {
                std::cout << "FULL\n";
            } else {
                const int cursor = used++;
                nodes[cursor].value = value;
                const int next = index == size ? -1 : nodeAt(index);
                const int previous = next == -1 ? tail : nodes[next].previous;
                nodes[cursor].previous = previous;
                nodes[cursor].next = next;
                if (previous == -1) head = cursor;
                else nodes[previous].next = cursor;
                if (next == -1) tail = cursor;
                else nodes[next].previous = cursor;
                ++size;
            }
        } else if (command == "pop_front" || command == "pop_back" ||
                   command == "erase") {
            int index = command == "pop_front" ? 0 : size - 1;
            if (command == "erase") std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << (command == "erase" ? "OUT_OF_RANGE\n" : "EMPTY\n");
            } else {
                const int cursor = nodeAt(index);
                const int previous = nodes[cursor].previous;
                const int next = nodes[cursor].next;
                if (previous == -1) head = next;
                else nodes[previous].next = next;
                if (next == -1) tail = previous;
                else nodes[next].previous = previous;
                std::cout << nodes[cursor].value << '\n';
                --size;
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << nodes[nodeAt(index)].value << '\n';
        } else if (command == "front") {
            if (head == -1) std::cout << "EMPTY\n";
            else std::cout << nodes[head].value << '\n';
        } else if (command == "back") {
            if (tail == -1) std::cout << "EMPTY\n";
            else std::cout << nodes[tail].value << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "free") {
            std::cout << capacity - used << '\n';
        } else if (command == "clear") {
            head = tail = -1;
            size = 0;
        }
    }
}
