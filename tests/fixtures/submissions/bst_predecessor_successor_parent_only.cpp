#include <iostream>
#include <string>

struct Node {
    int key;
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

Node* minimum(Node* node) {
    while (node != nullptr && node->left != nullptr) node = node->left;
    return node;
}

Node* maximum(Node* node) {
    while (node != nullptr && node->right != nullptr) node = node->right;
    return node;
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
            std::cin >> key;
            Node* parent = nullptr;
            Node* current = root;
            while (current != nullptr && current->key != key) {
                parent = current;
                current = key < current->key ? current->left : current->right;
            }
            if (current != nullptr) std::cout << "DUPLICATE\n";
            else {
                Node* node = new Node{key, parent, nullptr, nullptr};
                if (parent == nullptr) root = node;
                else if (key < parent->key) parent->left = node;
                else parent->right = node;
                ++size;
            }
        } else if (command == "predecessor" || command == "successor") {
            int key = 0;
            std::cin >> key;
            Node* node = findNode(root, key);
            if (node == nullptr) std::cout << "NOT_FOUND\n";
            else {
                Node* neighbor = nullptr;
                if (command == "predecessor") {
                    neighbor = node->left != nullptr
                        ? maximum(node->left)
                        : node->parent;
                } else {
                    neighbor = node->right != nullptr
                        ? minimum(node->right)
                        : node->parent;
                }
                if (neighbor == nullptr) std::cout << "NONE\n";
                else std::cout << neighbor->key << '\n';
            }
        } else if (command == "min" || command == "max") {
            Node* node = command == "min" ? minimum(root) : maximum(root);
            if (node == nullptr) std::cout << "EMPTY\n";
            else std::cout << node->key << '\n';
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
