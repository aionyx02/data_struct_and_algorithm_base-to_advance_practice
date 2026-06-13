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
    Node head{0, nullptr, nullptr};
    Node tail{0, nullptr, nullptr};
    head.next = &tail;
    tail.previous = &head;
    int size = 0;

    auto insertBefore = [&](Node* next, int value) {
        Node* node = new Node{value, next->previous, next};
        next->previous->next = node;
        next->previous = node;
        ++size;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front" || command == "push_back") {
            int value = 0;
            std::cin >> value;
            insertBefore(command == "push_front" ? head.next : &tail, value);
        } else if (command == "pop_front" || command == "pop_back") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else {
                Node* removed =
                    command == "pop_front" ? head.next : tail.previous;
                std::cout << removed->value << '\n';
                removed->previous->next = removed->next;
                // Incorrect: removed->next->previous is not repaired.
                delete removed;
                --size;
            }
        } else if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                Node* next = head.next;
                for (int step = 0; step < index; ++step) next = next->next;
                insertBefore(next, value);
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                Node* removed = head.next;
                for (int step = 0; step < index; ++step) removed = removed->next;
                std::cout << removed->value << '\n';
                removed->previous->next = removed->next;
                delete removed;
                --size;
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            Node* current = head.next;
            for (int step = 0; step < index && current != &tail; ++step) {
                current = current->next;
            }
            if (index < 0 || current == &tail) std::cout << "OUT_OF_RANGE\n";
            else std::cout << current->value << '\n';
        } else if (command == "front") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << head.next->value << '\n';
        } else if (command == "back") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << tail.previous->value << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "clear") {
            head.next = &tail;
            tail.previous = &head;
            size = 0;
        }
    }
}
