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

    Node* nodeAt(int index) const {
        if (index < size / 2) {
            Node* current = head;
            for (int step = 0; step < index; ++step) {
                current = current->next;
            }
            return current;
        }
        Node* current = tail;
        for (int step = size - 1; step > index; --step) {
            current = current->previous;
        }
        return current;
    }

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

    void splice(int index, List& source) {
        if (source.size == 0) {
            return;
        }
        if (size == 0) {
            head = source.head;
            tail = source.tail;
        } else if (index == 0) {
            source.tail->next = head;
            head->previous = source.tail;
            head = source.head;
        } else if (index == size) {
            tail->next = source.head;
            source.head->previous = tail;
            tail = source.tail;
        } else {
            Node* after = nodeAt(index);
            Node* before = after->previous;
            before->next = source.head;
            source.head->previous = before;
            source.tail->next = after;
            after->previous = source.tail;
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
        if (removed->previous == nullptr) {
            head = removed->next;
        } else {
            removed->previous->next = removed->next;
        }
        if (removed->next == nullptr) {
            tail = removed->previous;
        } else {
            removed->next->previous = removed->previous;
        }
        std::cout << removed->value << '\n';
        delete removed;
        --size;
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
    a.clear();
    b.clear();
}
