#include <iostream>
#include <string>

struct Node {
    int value;
    Node* next;
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* tail = nullptr;
    int size = 0;

    auto clear = [&]() {
        while (tail != nullptr) {
            Node* removed = tail->next;
            if (removed == tail) {
                tail = nullptr;
            } else {
                tail->next = removed->next;
            }
            delete removed;
        }
        size = 0;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr};
            if (tail == nullptr) {
                node->next = node;
                tail = node;
            } else {
                node->next = tail->next;
                tail->next = node;
            }
            ++size;
        } else if (command == "push_back") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr};
            if (tail == nullptr) {
                node->next = node;
                tail = node;
            } else {
                node->next = tail->next;
                tail->next = node;
                tail = node;
            }
            ++size;
        } else if (command == "pop_front") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                Node* removed = tail->next;
                std::cout << removed->value << '\n';
                if (removed == tail) {
                    tail = nullptr;
                } else {
                    tail->next = removed->next;
                }
                delete removed;
                --size;
            }
        } else if (command == "front") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << tail->next->value << '\n';
            }
        } else if (command == "back") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << tail->value << '\n';
            }
        } else if (command == "rotate") {
            if (tail != nullptr) {
                tail = tail->next;
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
