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
        } else if (command == "insert") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (index < 0 || index > size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (index == 0) {
                head = new Node{value, head};
                if (tail == nullptr) {
                    tail = head;
                }
                ++size;
            } else if (index == size) {
                Node* node = new Node{value, nullptr};
                tail->next = node;
                tail = node;
                ++size;
            } else {
                Node* previous = head;
                for (int step = 1; step < index; ++step) {
                    previous = previous->next;
                }
                previous->next = new Node{value, previous->next};
                ++size;
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                Node* removed = head;
                if (index == 0) {
                    head = head->next;
                } else {
                    Node* previous = head;
                    for (int step = 1; step < index; ++step) {
                        previous = previous->next;
                    }
                    removed = previous->next;
                    previous->next = removed->next;
                    if (removed == tail) {
                        tail = previous;
                    }
                }
                std::cout << removed->value << '\n';
                delete removed;
                --size;
                if (size == 0) {
                    tail = nullptr;
                }
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                Node* current = head;
                for (int step = 0; step < index; ++step) {
                    current = current->next;
                }
                std::cout << current->value << '\n';
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
