#include <iostream>
#include <string>

struct Node {
    int key;
    int value;
    Node* parent;
    Node* left;
    Node* right;
};

Node* findNode(Node* root, int key) {
    while (root != nullptr && root->key != key) {
        root = key < root->key ? root->left : root->right;
    }
    return root;
}

void deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* root = nullptr;
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            int value = 0;
            std::cin >> key >> value;
            Node* parent = nullptr;
            Node* current = root;
            while (current != nullptr && current->key != key) {
                parent = current;
                current = key < current->key
                    ? current->left
                    : current->right;
            }
            if (current != nullptr) {
                std::cout << "DUPLICATE\n";
            } else {
                Node* node =
                    new Node{key, value, parent, nullptr, nullptr};
                if (parent == nullptr) root = node;
                else if (key < parent->key) parent->left = node;
                else parent->right = node;
                ++size;
            }
        } else if (command == "contains") {
            int key = 0;
            std::cin >> key;
            std::cout << (findNode(root, key) != nullptr ? "true" : "false")
                      << '\n';
        } else if (command == "value" || command == "parent") {
            int key = 0;
            std::cin >> key;
            Node* node = findNode(root, key);
            if (node == nullptr) std::cout << "NOT_FOUND\n";
            else if (command == "value") std::cout << node->value << '\n';
            else if (node->parent == nullptr) std::cout << "NONE\n";
            else std::cout << node->parent->key << '\n';
        } else if (command == "root") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else std::cout << root->key << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            deleteTree(root);
            root = nullptr;
            size = 0;
        }
    }
    deleteTree(root);
}
