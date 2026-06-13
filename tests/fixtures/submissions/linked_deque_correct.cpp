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

    auto clear = [&]() {
        while (head != nullptr) {
            Node* node = head;
            head = head->next;
            delete node;
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
            if (head == nullptr) tail = node;
            else head->previous = node;
            head = node;
            ++size;
        } else if (command == "push_back") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, tail, nullptr};
            if (tail == nullptr) head = node;
            else tail->next = node;
            tail = node;
            ++size;
        } else if (command == "pop_front") {
            if (head == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                Node* node = head;
                std::cout << node->value << '\n';
                head = head->next;
                if (head == nullptr) tail = nullptr;
                else head->previous = nullptr;
                delete node;
                --size;
            }
        } else if (command == "pop_back") {
            if (tail == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                Node* node = tail;
                std::cout << node->value << '\n';
                tail = tail->previous;
                if (tail == nullptr) head = nullptr;
                else tail->next = nullptr;
                delete node;
                --size;
            }
        } else if (command == "front") {
            if (head == nullptr) std::cout << "EMPTY\n";
            else std::cout << head->value << '\n';
        } else if (command == "back") {
            if (tail == nullptr) std::cout << "EMPTY\n";
            else std::cout << tail->value << '\n';
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
