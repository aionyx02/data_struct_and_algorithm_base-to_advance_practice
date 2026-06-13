#include <iostream>
#include <string>

struct Node {
    int value;
    Node* next;
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* topNode = nullptr;
    int size = 0;

    auto clear = [&]() {
        while (topNode != nullptr) {
            Node* removed = topNode;
            topNode = topNode->next;
            delete removed;
        }
        size = 0;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push") {
            int value = 0;
            std::cin >> value;
            topNode = new Node{value, topNode};
            ++size;
        } else if (command == "pop") {
            if (topNode == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                Node* removed = topNode;
                topNode = topNode->next;
                std::cout << removed->value << '\n';
                delete removed;
                --size;
            }
        } else if (command == "top") {
            if (topNode == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                std::cout << topNode->value << '\n';
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
