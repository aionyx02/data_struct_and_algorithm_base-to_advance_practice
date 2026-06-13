#include <iostream>
#include <memory>
#include <string>

struct CursorNode {
    int value = 0;
    int previous = -1;
    int next = -1;
    int freeNext = -1;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto nodes = std::make_unique<CursorNode[]>(capacity);
    int freeHead = capacity == 0 ? -1 : 0;
    for (int index = 0; index < capacity; ++index) {
        nodes[index].freeNext = index + 1 < capacity ? index + 1 : -1;
    }
    int head = -1;
    int tail = -1;
    int size = 0;

    auto acquire = [&]() {
        const int cursor = freeHead;
        if (cursor != -1) {
            freeHead = nodes[cursor].freeNext;
            nodes[cursor].freeNext = -1;
        }
        return cursor;
    };
    auto release = [&](int cursor) {
        nodes[cursor].previous = -1;
        nodes[cursor].next = -1;
        nodes[cursor].freeNext = freeHead;
        freeHead = cursor;
    };
    auto nodeAt = [&](int index) {
        int cursor = -1;
        if (index < size / 2) {
            cursor = head;
            for (int step = 0; step < index; ++step) cursor = nodes[cursor].next;
        } else {
            cursor = tail;
            for (int step = size - 1; step > index; --step) {
                cursor = nodes[cursor].previous;
            }
        }
        return cursor;
    };
    auto linkBefore = [&](int next, int cursor) {
        const int previous = next == -1 ? tail : nodes[next].previous;
        nodes[cursor].previous = previous;
        nodes[cursor].next = next;
        if (previous == -1) head = cursor;
        else nodes[previous].next = cursor;
        if (next == -1) tail = cursor;
        else nodes[next].previous = cursor;
        ++size;
    };
    auto unlink = [&](int cursor) {
        const int previous = nodes[cursor].previous;
        const int next = nodes[cursor].next;
        if (previous == -1) head = next;
        else nodes[previous].next = next;
        if (next == -1) tail = previous;
        else nodes[next].previous = previous;
        const int value = nodes[cursor].value;
        release(cursor);
        --size;
        return value;
    };
    auto clear = [&]() {
        while (head != -1) unlink(head);
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
                continue;
            }
            const int cursor = acquire();
            if (cursor == -1) {
                std::cout << "FULL\n";
                continue;
            }
            nodes[cursor].value = value;
            linkBefore(index == size ? -1 : nodeAt(index), cursor);
        } else if (command == "pop_front" || command == "pop_back") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << unlink(command == "pop_front" ? head : tail) << '\n';
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << unlink(nodeAt(index)) << '\n';
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
            std::cout << capacity - size << '\n';
        } else if (command == "clear") {
            clear();
        }
    }
}
