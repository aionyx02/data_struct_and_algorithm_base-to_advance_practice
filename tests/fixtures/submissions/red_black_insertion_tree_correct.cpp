#include <climits>
#include <iostream>
#include <string>

struct Node {
    int key;
    bool red;
    Node* left;
    Node* right;
    Node* parent;
};

Node* find(Node* root, int key) {
    while (root != nullptr && root->key != key) {
        root = key < root->key ? root->left : root->right;
    }
    return root;
}

void rotateLeft(Node*& root, Node* node) {
    Node* pivot = node->right;
    node->right = pivot->left;
    if (pivot->left != nullptr) pivot->left->parent = node;
    pivot->parent = node->parent;
    if (node->parent == nullptr) root = pivot;
    else if (node == node->parent->left) node->parent->left = pivot;
    else node->parent->right = pivot;
    pivot->left = node;
    node->parent = pivot;
}

void rotateRight(Node*& root, Node* node) {
    Node* pivot = node->left;
    node->left = pivot->right;
    if (pivot->right != nullptr) pivot->right->parent = node;
    pivot->parent = node->parent;
    if (node->parent == nullptr) root = pivot;
    else if (node == node->parent->left) node->parent->left = pivot;
    else node->parent->right = pivot;
    pivot->right = node;
    node->parent = pivot;
}

void fixInsert(Node*& root, Node* node) {
    while (node != root && node->parent->red) {
        Node* parent = node->parent;
        Node* grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;
            if (uncle != nullptr && uncle->red) {
                parent->red = false;
                uncle->red = false;
                grandparent->red = true;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    node = parent;
                    rotateLeft(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->red = false;
                grandparent->red = true;
                rotateRight(root, grandparent);
            }
        } else {
            Node* uncle = grandparent->left;
            if (uncle != nullptr && uncle->red) {
                parent->red = false;
                uncle->red = false;
                grandparent->red = true;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    node = parent;
                    rotateRight(root, node);
                    parent = node->parent;
                    grandparent = parent->parent;
                }
                parent->red = false;
                grandparent->red = true;
                rotateLeft(root, grandparent);
            }
        }
    }
    root->red = false;
}

bool insert(Node*& root, int key) {
    Node* parent = nullptr;
    Node* current = root;
    while (current != nullptr) {
        parent = current;
        if (key == current->key) return false;
        current = key < current->key ? current->left : current->right;
    }
    Node* node = new Node{key, true, nullptr, nullptr, parent};
    if (parent == nullptr) root = node;
    else if (key < parent->key) parent->left = node;
    else parent->right = node;
    fixInsert(root, node);
    return true;
}

void printPreorder(Node* root, bool& first) {
    if (root == nullptr) return;
    if (!first) std::cout << ' ';
    std::cout << root->key << ':' << (root->red ? 'R' : 'B');
    first = false;
    printPreorder(root->left, first);
    printPreorder(root->right, first);
}

void printInorder(Node* root, bool& first) {
    if (root == nullptr) return;
    printInorder(root->left, first);
    if (!first) std::cout << ' ';
    std::cout << root->key;
    first = false;
    printInorder(root->right, first);
}

int blackHeight(Node* root) {
    if (root == nullptr) return 1;
    return blackHeight(root->left) + (root->red ? 0 : 1);
}

bool validate(
    Node* node,
    Node* parent,
    long long low,
    long long high,
    int& height
) {
    if (node == nullptr) {
        height = 1;
        return true;
    }
    if (node->parent != parent ||
        node->key <= low || node->key >= high ||
        (node->red &&
         ((node->left != nullptr && node->left->red) ||
          (node->right != nullptr && node->right->red)))) {
        return false;
    }
    int leftHeight = 0;
    int rightHeight = 0;
    if (!validate(node->left, node, low, node->key, leftHeight) ||
        !validate(node->right, node, node->key, high, rightHeight) ||
        leftHeight != rightHeight) {
        return false;
    }
    height = leftHeight + (node->red ? 0 : 1);
    return true;
}

void destroy(Node* root) {
    if (root == nullptr) return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* root = nullptr;
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "root") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else std::cout << root->key << '\n';
        } else if (command == "preorder" || command == "inorder") {
            if (root == nullptr) {
                std::cout << "EMPTY\n";
            } else {
                bool first = true;
                if (command == "preorder") printPreorder(root, first);
                else printInorder(root, first);
                std::cout << '\n';
            }
        } else if (command == "black_height") {
            std::cout << blackHeight(root) << '\n';
        } else if (command == "validate") {
            int height = 0;
            const bool valid =
                (root == nullptr || !root->red) &&
                validate(root, nullptr, LLONG_MIN, LLONG_MAX, height);
            std::cout << (valid ? "VALID" : "INVALID") << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            destroy(root);
            root = nullptr;
            size = 0;
        } else {
            int key = 0;
            std::cin >> key;
            if (command == "insert") {
                if (!insert(root, key)) std::cout << "DUPLICATE\n";
                else ++size;
            } else {
                Node* node = find(root, key);
                if (command == "contains") {
                    std::cout << (node != nullptr ? "true" : "false") << '\n';
                } else if (node == nullptr) {
                    std::cout << "NOT_FOUND\n";
                } else if (command == "color") {
                    std::cout << (node->red ? "RED" : "BLACK") << '\n';
                } else if (node->parent == nullptr) {
                    std::cout << "NONE\n";
                } else {
                    std::cout << node->parent->key << '\n';
                }
            }
        }
    }
    destroy(root);
}
