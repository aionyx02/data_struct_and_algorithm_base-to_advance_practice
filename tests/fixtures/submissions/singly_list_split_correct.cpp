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

    void clear() {
        while (head != nullptr) {
            Node* removed = head;
            head = head->next;
            delete removed;
        }
        tail = nullptr;
        size = 0;
    }

    void split(int index, List& target) {
        if (index == 0) {
            target.head = head;
            target.tail = tail;
            target.size = size;
            head = tail = nullptr;
            size = 0;
            return;
        }
        if (index == size) {
            return;
        }
        Node* newTail = head;
        for (int step = 1; step < index; ++step) {
            newTail = newTail->next;
        }
        target.head = newTail->next;
        target.tail = tail;
        target.size = size - index;
        newTail->next = nullptr;
        tail = newTail;
        size = index;
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
        } else if (command == "split_a" || command == "split_b") {
            int index = 0;
            std::cin >> index;
            List& source = command == "split_a" ? a : b;
            List& target = command == "split_a" ? b : a;
            if (index < 0 || index > source.size) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (target.size != 0) {
                std::cout << "TARGET_NOT_EMPTY\n";
            } else {
                source.split(index, target);
            }
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
