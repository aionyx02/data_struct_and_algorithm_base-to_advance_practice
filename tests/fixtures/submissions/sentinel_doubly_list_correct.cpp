#include <iostream>
#include <string>

struct Node {
    int value;
    Node* previous;
    Node* next;
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node headSentinel{0, nullptr, nullptr};
    Node tailSentinel{0, nullptr, nullptr};
    headSentinel.next = &tailSentinel;
    tailSentinel.previous = &headSentinel;
    int size = 0;

    auto insertBefore = [&](Node* next, int value) {
        Node* node = new Node{value, next->previous, next};
        next->previous->next = node;
        next->previous = node;
        ++size;
    };
    auto eraseNode = [&](Node* node) {
        node->previous->next = node->next;
        node->next->previous = node->previous;
        const int value = node->value;
        delete node;
        --size;
        return value;
    };
    auto nodeAt = [&](int index) {
        Node* current = nullptr;
        if (index < size / 2) {
            current = headSentinel.next;
            for (int step = 0; step < index; ++step) {
                current = current->next;
            }
        } else {
            current = tailSentinel.previous;
            for (int step = size - 1; step > index; --step) {
                current = current->previous;
            }
        }
        return current;
    };
    auto clear = [&]() {
        while (headSentinel.next != &tailSentinel) {
            eraseNode(headSentinel.next);
        }
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front" || command == "push_back") {
            int value = 0;
            std::cin >> value;
            insertBefore(
                command == "push_front" ? headSentinel.next : &tailSentinel,
                value
            );
        } else if (command == "pop_front" || command == "pop_back") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                Node* node = command == "pop_front"
                    ? headSentinel.next
                    : tailSentinel.previous;
                std::cout << eraseNode(node) << '\n';
            }
        } else if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                insertBefore(index == size ? &tailSentinel : nodeAt(index), value);
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << eraseNode(nodeAt(index)) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << nodeAt(index)->value << '\n';
            }
        } else if (command == "front") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << headSentinel.next->value << '\n';
        } else if (command == "back") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << tailSentinel.previous->value << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "clear") {
            clear();
        }
    }
    clear();
}
