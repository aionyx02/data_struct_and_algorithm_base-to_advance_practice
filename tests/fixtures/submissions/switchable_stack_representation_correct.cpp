#include <iostream>
#include <memory>
#include <string>

struct Node {
    int value;
    Node* next;
};

void deleteNodes(Node*& top) {
    while (top != nullptr) {
        Node* removed = top;
        top = top->next;
        delete removed;
    }
}

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto array = std::make_unique<int[]>(capacity);
    Node* top = nullptr;
    int size = 0;
    bool linked = false;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "push") {
            int value = 0;
            std::cin >> value;
            if (size == capacity) {
                std::cout << "FULL\n";
            } else if (linked) {
                top = new Node{value, top};
                ++size;
            } else {
                array[size++] = value;
            }
        } else if (command == "pop") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else if (linked) {
                Node* removed = top;
                std::cout << removed->value << '\n';
                top = top->next;
                delete removed;
                --size;
            } else {
                std::cout << array[--size] << '\n';
            }
        } else if (command == "top") {
            if (size == 0) std::cout << "EMPTY\n";
            else if (linked) std::cout << top->value << '\n';
            else std::cout << array[size - 1] << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "representation") {
            std::cout << (linked ? "linked" : "array") << '\n';
        } else if (command == "switch") {
            std::string target;
            std::cin >> target;
            const bool targetLinked = target == "linked";
            if (targetLinked == linked) {
                std::cout << "UNCHANGED\n";
            } else if (targetLinked) {
                for (int index = 0; index < size; ++index) {
                    top = new Node{array[index], top};
                }
                linked = true;
                std::cout << size << '\n';
            } else {
                Node* cursor = top;
                for (int index = size - 1; index >= 0; --index) {
                    array[index] = cursor->value;
                    cursor = cursor->next;
                }
                deleteNodes(top);
                linked = false;
                std::cout << size << '\n';
            }
        } else if (command == "values") {
            if (size == 0) {
                std::cout << "EMPTY\n";
            } else if (linked) {
                auto ordered = std::make_unique<int[]>(size);
                Node* cursor = top;
                for (int index = size - 1; index >= 0; --index) {
                    ordered[index] = cursor->value;
                    cursor = cursor->next;
                }
                for (int index = 0; index < size; ++index) {
                    if (index > 0) std::cout << ' ';
                    std::cout << ordered[index];
                }
                std::cout << '\n';
            } else {
                for (int index = 0; index < size; ++index) {
                    if (index > 0) std::cout << ' ';
                    std::cout << array[index];
                }
                std::cout << '\n';
            }
        } else if (command == "clear") {
            if (linked) deleteNodes(top);
            size = 0;
        }
    }

    deleteNodes(top);
}
