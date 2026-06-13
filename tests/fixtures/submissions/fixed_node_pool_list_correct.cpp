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
    int freeHead = capacity == 0 ? -1 : 0;
    for (int index = 0; index < capacity; ++index) {
        nodes[index].next = index + 1 < capacity ? index + 1 : -1;
    }
    int head = -1;
    int tail = -1;
    int size = 0;

    auto acquire = [&]() {
        const int index = freeHead;
        if (index != -1) {
            freeHead = nodes[index].next;
            nodes[index].next = -1;
        }
        return index;
    };
    auto release = [&](int index) {
        nodes[index].next = freeHead;
        freeHead = index;
    };
    auto nodeAt = [&](int index) {
        int current = head;
        for (int step = 0; step < index; ++step) {
            current = nodes[current].next;
        }
        return current;
    };
    auto clear = [&]() {
        while (head != -1) {
            const int removed = head;
            head = nodes[head].next;
            release(removed);
        }
        tail = -1;
        size = 0;
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
                continue;
            }
            const int node = acquire();
            if (node == -1) {
                std::cout << "FULL\n";
                continue;
            }
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
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
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
            release(removed);
            --size;
            if (size == 0) {
                tail = -1;
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
            if (head == -1) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << nodes[head].value << '\n';
            }
        } else if (command == "back") {
            if (tail == -1) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << nodes[tail].value << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "free") {
            std::cout << capacity - size << '\n';
        } else if (command == "clear") {
            clear();
        }
    }
}
