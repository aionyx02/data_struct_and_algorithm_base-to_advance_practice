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
    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;

    auto nodeAt = [&](int index) {
        Node* current = nullptr;
        if (index < size / 2) {
            current = head;
            for (int step = 0; step < index; ++step) {
                current = current->next;
            }
        } else {
            current = tail;
            for (int step = size - 1; step > index; --step) {
                current = current->previous;
            }
        }
        return current;
    };
    auto unlink = [&](Node* removed) {
        if (removed->previous == nullptr) {
            head = removed->next;
        } else {
            removed->previous->next = removed->next;
        }
        if (removed->next == nullptr) {
            tail = removed->previous;
        } else {
            removed->next->previous = removed->previous;
        }
        const int value = removed->value;
        delete removed;
        --size;
        return value;
    };
    auto clear = [&]() {
        while (head != nullptr) {
            Node* removed = head;
            head = head->next;
            delete removed;
        }
        tail = nullptr;
        size = 0;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr, head};
            if (head == nullptr) {
                tail = node;
            } else {
                head->previous = node;
            }
            head = node;
            ++size;
        } else if (command == "push_back") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, tail, nullptr};
            if (tail == nullptr) {
                head = node;
            } else {
                tail->next = node;
            }
            tail = node;
            ++size;
        } else if (command == "pop_front") {
            if (head == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << unlink(head) << '\n';
            }
        } else if (command == "pop_back") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << unlink(tail) << '\n';
            }
        } else if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (index == 0) {
                Node* node = new Node{value, nullptr, head};
                if (head == nullptr) {
                    tail = node;
                } else {
                    head->previous = node;
                }
                head = node;
                ++size;
            } else if (index == size) {
                Node* node = new Node{value, tail, nullptr};
                tail->next = node;
                tail = node;
                ++size;
            } else {
                Node* next = nodeAt(index);
                Node* node = new Node{value, next->previous, next};
                next->previous->next = node;
                next->previous = node;
                ++size;
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << unlink(nodeAt(index)) << '\n';
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
            if (head == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << head->value << '\n';
            }
        } else if (command == "back") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << tail->value << '\n';
            }
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
