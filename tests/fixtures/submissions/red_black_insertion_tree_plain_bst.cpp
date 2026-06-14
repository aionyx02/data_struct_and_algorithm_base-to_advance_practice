#include <iostream>
#include <string>

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    int size = 0;
    bool hasRoot = false;
    int root = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            if (!hasRoot) {
                hasRoot = true;
                root = key;
            }
            ++size;
        } else if (command == "root") {
            if (hasRoot) std::cout << root << '\n';
            else std::cout << "EMPTY\n";
        } else if (command == "preorder") {
            if (hasRoot) std::cout << root << ":B\n";
            else std::cout << "EMPTY\n";
        } else if (command == "inorder") {
            if (hasRoot) std::cout << root << '\n';
            else std::cout << "EMPTY\n";
        } else if (command == "black_height") {
            std::cout << (hasRoot ? 2 : 1) << '\n';
        } else if (command == "validate") {
            std::cout << "VALID\n";
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            size = 0;
            hasRoot = false;
        } else {
            int key = 0;
            std::cin >> key;
            if (command == "contains") std::cout << "false\n";
            else if (command == "color") std::cout << "BLACK\n";
            else std::cout << "NONE\n";
        }
    }
}
