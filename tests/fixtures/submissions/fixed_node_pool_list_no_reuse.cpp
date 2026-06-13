#include <iostream>
#include <memory>
#include <string>

struct Node {
    int value = 0;
    int next = -1;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto nodes = std::make_unique<Node[]>(capacity);
    int used = 0;
    int head = -1;
    int tail = -1;
    int size = 0;

    auto nodeAt = [&](int index) {
        int current = head;
        for (int step = 0; step < index; ++step) {
            current = nodes[current].next;
        }
        return current;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (used == capacity) {
                std::cout << "FULL\n";
            } else {
                const int node = used++;
                nodes[node].value = value;
                if (index == 0) {
                    nodes[node].next = head;
                    head = node;
                    if (tail == -1) {
                        tail = node;
                    }
                } else {
                    const int previous = nodeAt(index - 1);
                    nodes[node].next = nodes[previous].next;
                    nodes[previous].next = node;
                    if (index == size) {
                        tail = node;
                    }
                }
                ++size;
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                int removed = head;
                if (index == 0) {
                    head = nodes[head].next;
                } else {
                    const int previous = nodeAt(index - 1);
                    removed = nodes[previous].next;
                    nodes[previous].next = nodes[removed].next;
                    if (removed == tail) {
                        tail = previous;
                    }
                }
                std::cout << nodes[removed].value << '\n';
                --size;
                if (size == 0) {
                    tail = -1;
                }
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << nodes[nodeAt(index)].value << '\n';
            }
        } else if (command == "front") {
            std::cout << (head == -1 ? "EMPTY" : std::to_string(nodes[head].value))
                      << '\n';
        } else if (command == "back") {
            std::cout << (tail == -1 ? "EMPTY" : std::to_string(nodes[tail].value))
                      << '\n';
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
