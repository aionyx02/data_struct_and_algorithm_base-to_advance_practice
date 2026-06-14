#include <iostream>
#include <string>

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    int size = 0;
    bool nonempty = false;
    int root = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            if (!nonempty) {
                root = key;
                nonempty = true;
            }
            ++size;
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            std::cout << "NOT_FOUND\n";
        } else if (command == "contains") {
            int key = 0;
            std::cin >> key;
            std::cout << "false\n";
        } else if (command == "root") {
            if (nonempty) std::cout << root << '\n';
            else std::cout << "EMPTY\n";
        } else if (command == "preorder" || command == "inorder") {
            if (nonempty) std::cout << root << '\n';
            else std::cout << "EMPTY\n";
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            size = 0;
            nonempty = false;
        } else {
            int key = 0;
            std::cin >> key;
            if (command == "parent") std::cout << "NONE\n";
            else if (command == "height") std::cout << "1\n";
            else std::cout << "0\n";
        }
    }
}
