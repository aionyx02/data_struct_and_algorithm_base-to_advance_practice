#include <iostream>
#include <string>

struct Node {
    int value;
    Node* next;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;

    void pushBack(int value) {
        Node* node = new Node{value, nullptr};
        if (tail == nullptr) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        ++size;
    }

    void concat(List& source) {
        if (source.head == nullptr) {
            return;
        }
        if (tail == nullptr) {
            head = source.head;
            tail = source.tail;
        } else {
            tail->next = source.head;
            tail = source.tail;
        }
        size += source.size;
        source.head = source.tail = nullptr;
        source.size = 0;
    }

    void popFront() {
        if (head == nullptr) {
            std::cout << "EMPTY\n";
            return;
        }
        Node* removed = head;
        head = head->next;
        std::cout << removed->value << '\n';
        delete removed;
        --size;
        if (head == nullptr) {
            tail = nullptr;
        }
    }

    void clear() {
        while (head != nullptr) {
            Node* removed = head;
            head = head->next;
            delete removed;
        }
        tail = nullptr;
        size = 0;
    }
};

void queryEnd(const List& list, bool front) {
    const Node* node = front ? list.head : list.tail;
    if (node == nullptr) {
        std::cout << "EMPTY\n";
    } else {
        std::cout << node->value << '\n';
    }
}

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    List a;
    List b;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push_a" || command == "push_b") {
            int value = 0;
            std::cin >> value;
            (command == "push_a" ? a : b).pushBack(value);
        } else if (command == "concat_ab") {
            a.concat(b);
        } else if (command == "concat_ba") {
            b.concat(a);
        } else if (command == "pop_a") {
            a.popFront();
        } else if (command == "pop_b") {
            b.popFront();
        } else if (command == "front_a") {
            queryEnd(a, true);
        } else if (command == "back_a") {
            queryEnd(a, false);
        } else if (command == "front_b") {
            queryEnd(b, true);
        } else if (command == "back_b") {
            queryEnd(b, false);
        } else if (command == "size_a") {
            std::cout << a.size << '\n';
        } else if (command == "size_b") {
            std::cout << b.size << '\n';
        } else if (command == "empty_a") {
            std::cout << (a.size == 0 ? "true" : "false") << '\n';
        } else if (command == "empty_b") {
            std::cout << (b.size == 0 ? "true" : "false") << '\n';
        } else if (command == "clear_a") {
            a.clear();
        } else if (command == "clear_b") {
            b.clear();
        }
    }
    a.clear();
    b.clear();
}
