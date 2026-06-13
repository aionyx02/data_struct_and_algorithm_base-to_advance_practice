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
    while (node->left != nullptr) node = node->left;
    return node;
}

void transplant(Node*& root, Node* oldNode, Node* newNode) {
    if (oldNode->parent == nullptr) root = newNode;
    else if (oldNode->parent->left == oldNode) {
        oldNode->parent->left = newNode;
    } else {
        oldNode->parent->right = newNode;
    }
    if (newNode != nullptr) newNode->parent = oldNode->parent;
}

void printInorder(Node* node, bool& first) {
    if (node == nullptr) return;
    printInorder(node->left, first);
    if (!first) std::cout << ' ';
    std::cout << node->key;
    first = false;
    printInorder(node->right, first);
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
                current = key < current->key
                    ? current->left
                    : current->right;
            }
            if (current != nullptr) std::cout << "DUPLICATE\n";
            else {
                Node* node = new Node{key, parent, nullptr, nullptr};
                if (parent == nullptr) root = node;
                else if (key < parent->key) parent->left = node;
                else parent->right = node;
                ++size;
            }
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            Node* node = findNode(root, key);
            if (node == nullptr) {
                std::cout << "NOT_FOUND\n";
                continue;
            }
            std::cout << key << '\n';
            if (node->left == nullptr) transplant(root, node, node->right);
            else if (node->right == nullptr) transplant(root, node, node->left);
            else {
                Node* successor = minimum(node->right);
                if (successor->parent != node) {
                    transplant(root, successor, successor->right);
                    successor->right = node->right;
                    successor->right->parent = successor;
                }
                transplant(root, node, successor);
                successor->left = node->left;
                successor->left->parent = successor;
            }
            delete node;
            --size;
        } else if (command == "contains" || command == "parent") {
            int key = 0;
            std::cin >> key;
            Node* node = findNode(root, key);
            if (command == "contains") {
                std::cout << (node != nullptr ? "true" : "false") << '\n';
            } else if (node == nullptr) {
                std::cout << "NOT_FOUND\n";
            } else if (node->parent == nullptr) {
                std::cout << "NONE\n";
            } else {
                std::cout << node->parent->key << '\n';
            }
        } else if (command == "root") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else std::cout << root->key << '\n';
        } else if (command == "inorder") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else {
                bool first = true;
                printInorder(root, first);
                std::cout << '\n';
            }
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
