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

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "enqueue") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr};
            if (frontNode == nullptr) {
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
                --size;
                if (frontNode != nullptr) {
                    delete removed;
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
            frontNode = nullptr;
            size = 0;
        }
    }
}
