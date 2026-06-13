#include <iostream>
#include <string>

struct Node {
    int value;
    Node* next;
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* frontNode = nullptr;
    Node* backNode = nullptr;
    int size = 0;

    auto clear = [&]() {
        while (frontNode != nullptr) {
            Node* removed = frontNode;
            frontNode = frontNode->next;
            delete removed;
        }
        backNode = nullptr;
        size = 0;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "enqueue") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr};
            if (backNode == nullptr) {
                frontNode = backNode = node;
            } else {
                backNode->next = node;
                backNode = node;
            }
            ++size;
        } else if (command == "dequeue") {
            if (frontNode == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                Node* removed = frontNode;
                frontNode = frontNode->next;
                std::cout << removed->value << '\n';
                delete removed;
                --size;
                if (frontNode == nullptr) {
                    backNode = nullptr;
                }
            }
        } else if (command == "front") {
            if (frontNode == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << frontNode->value << '\n';
            }
        } else if (command == "back") {
            if (backNode == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << backNode->value << '\n';
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
