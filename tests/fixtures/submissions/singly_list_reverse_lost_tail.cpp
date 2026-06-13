#include <iostream>
#include <string>

struct Node {
    int value;
    Node* next;
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_front") {
            int value = 0;
            std::cin >> value;
            head = new Node{value, head};
            if (tail == nullptr) {
                tail = head;
            }
            ++size;
        } else if (command == "push_back") {
            int value = 0;
            std::cin >> value;
            Node* node = new Node{value, nullptr};
            if (tail == nullptr) {
                head = tail = node;
            } else {
                tail->next = node;
                tail = node;
            }
            ++size;
        } else if (command == "reverse") {
            Node* previous = nullptr;
            Node* current = head;
            while (current != nullptr) {
                Node* next = current->next;
                current->next = previous;
                previous = current;
                current = next;
            }
            head = previous;
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            Node* current = head;
            for (int step = 0; step < index && current != nullptr; ++step) {
                current = current->next;
            }
            if (index < 0 || current == nullptr) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << current->value << '\n';
            }
        } else if (command == "front") {
            std::cout << (head == nullptr ? "EMPTY" : std::to_string(head->value))
                      << '\n';
        } else if (command == "back") {
            std::cout << (tail == nullptr ? "EMPTY" : std::to_string(tail->value))
                      << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "empty") {
            std::cout << (size == 0 ? "true" : "false") << '\n';
        } else if (command == "clear") {
            head = tail = nullptr;
            size = 0;
        }
    }
}
