#include <iostream>
#include <string>

struct Node {
    int value;
    Node* previous;
    Node* next;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
    int size = 0;

    void pushFront(int value) {
        Node* node = new Node{value, nullptr, head};
        if (head == nullptr) {
            tail = node;
        } else {
            head->previous = node;
        }
        head = node;
        ++size;
    }

    void pushBack(int value) {
        Node* node = new Node{value, tail, nullptr};
        if (tail == nullptr) {
            head = node;
        } else {
            tail->next = node;
        }
        tail = node;
        ++size;
    }

    Node* nodeAt(int index) const {
        Node* current = head;
        for (int step = 0; step < index; ++step) {
            current = current->next;
        }
        return current;
    }

    void splice(int index, List& source) {
        if (source.size == 0) {
            return;
        }
        if (size == 0) {
            head = source.head;
            tail = source.tail;
        } else if (index == 0) {
            source.tail->next = head;
            head = source.head;
        } else if (index == size) {
            tail->next = source.head;
            tail = source.tail;
        } else {
            Node* after = nodeAt(index);
            Node* before = after->previous;
            before->next = source.head;
            source.tail->next = after;
        }
        size += source.size;
        source.head = source.tail = nullptr;
        source.size = 0;
    }

    void pop(bool front) {
        Node* removed = front ? head : tail;
        if (removed == nullptr) {
            std::cout << "EMPTY\n";
            return;
        }
        std::cout << removed->value << '\n';
        if (front) {
            head = head->next;
            if (head != nullptr) {
                head->previous = nullptr;
            }
        } else {
            tail = tail->previous;
            if (tail != nullptr) {
                tail->next = nullptr;
            }
        }
        --size;
    }
};

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    List a;
    List b;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command.starts_with("push_front_") ||
            command.starts_with("push_back_")) {
            int value = 0;
            std::cin >> value;
            List& list = command.back() == 'a' ? a : b;
            if (command.starts_with("push_front_")) {
                list.pushFront(value);
            } else {
                list.pushBack(value);
            }
        } else if (command == "splice_b_to_a" ||
                   command == "splice_a_to_b") {
            int index = 0;
            std::cin >> index;
            List& destination = command == "splice_b_to_a" ? a : b;
            List& source = command == "splice_b_to_a" ? b : a;
            if (index < 0 || index > destination.size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                destination.splice(index, source);
            }
        } else if (command.starts_with("pop_front_") ||
                   command.starts_with("pop_back_")) {
            List& list = command.back() == 'a' ? a : b;
            list.pop(command.starts_with("pop_front_"));
        } else if (command == "get_a" || command == "get_b") {
            int index = 0;
            std::cin >> index;
            List& list = command == "get_a" ? a : b;
            if (index < 0 || index >= list.size) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << list.nodeAt(index)->value << '\n';
            }
        } else if (command == "size_a") {
            std::cout << a.size << '\n';
        } else if (command == "size_b") {
            std::cout << b.size << '\n';
        } else if (command == "empty_a") {
            std::cout << (a.size == 0 ? "true" : "false") << '\n';
        } else if (command == "empty_b") {
            std::cout << (b.size == 0 ? "true" : "false") << '\n';
        }
    }
}
