#include <algorithm>
#include <iostream>
#include <string>

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;
    Node* parent;
};

int height(Node* node) {
    return node == nullptr ? 0 : node->height;
}

void update(Node* node) {
    node->height = 1 + std::max(height(node->left), height(node->right));
}

int balance(Node* node) {
    return node == nullptr ? 0 : height(node->left) - height(node->right);
}

Node* rotateLeft(Node* root) {
    Node* pivot = root->right;
    Node* middle = pivot->left;
    pivot->left = root;
    pivot->parent = root->parent;
    root->parent = pivot;
    root->right = middle;
    if (middle != nullptr) middle->parent = root;
    update(root);
    update(pivot);
    return pivot;
}

Node* rotateRight(Node* root) {
    Node* pivot = root->left;
    Node* middle = pivot->right;
    pivot->right = root;
    pivot->parent = root->parent;
    root->parent = pivot;
    root->left = middle;
    if (middle != nullptr) middle->parent = root;
    update(root);
    update(pivot);
    return pivot;
}

Node* rebalance(Node* root) {
    update(root);
    if (balance(root) > 1) {
        if (balance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            root->left->parent = root;
        }
        return rotateRight(root);
    }
    if (balance(root) < -1) {
        if (balance(root->right) > 0) {
            root->right = rotateRight(root->right);
            root->right->parent = root;
        }
        return rotateLeft(root);
    }
    return root;
}

Node* insert(Node* root, Node* parent, int key, bool& inserted) {
    if (root == nullptr) {
        inserted = true;
        return new Node{key, 1, nullptr, nullptr, parent};
    }
    if (key == root->key) return root;
    if (key < root->key) {
        root->left = insert(root->left, root, key, inserted);
        root->left->parent = root;
    } else {
        root->right = insert(root->right, root, key, inserted);
        root->right->parent = root;
    }
    Node* result = rebalance(root);
    result->parent = parent;
    return result;
}

Node* minimum(Node* root) {
    while (root->left != nullptr) root = root->left;
    return root;
}

Node* erase(Node* root, Node* parent, int key, bool& removed) {
    if (root == nullptr) return nullptr;
    if (key < root->key) {
        root->left = erase(root->left, root, key, removed);
        if (root->left != nullptr) root->left->parent = root;
    } else if (key > root->key) {
        root->right = erase(root->right, root, key, removed);
        if (root->right != nullptr) root->right->parent = root;
    } else {
        removed = true;
        if (root->left == nullptr || root->right == nullptr) {
            Node* child = root->left != nullptr ? root->left : root->right;
            if (child != nullptr) child->parent = parent;
            delete root;
            return child;
        }
        Node* successor = minimum(root->right);
        root->key = successor->key;
        bool ignored = false;
        root->right = erase(root->right, root, successor->key, ignored);
        if (root->right != nullptr) root->right->parent = root;
    }
    Node* result = rebalance(root);
    result->parent = parent;
    return result;
}

Node* find(Node* root, int key) {
    while (root != nullptr && root->key != key) {
        root = key < root->key ? root->left : root->right;
    }
    return root;
}

void printPreorder(Node* root, bool& first) {
    if (root == nullptr) return;
    if (!first) std::cout << ' ';
    std::cout << root->key;
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
                bool inserted = false;
                root = insert(root, nullptr, key, inserted);
                if (root != nullptr) root->parent = nullptr;
                if (!inserted) std::cout << "DUPLICATE\n";
                else ++size;
            } else if (command == "erase") {
                bool removed = false;
                root = erase(root, nullptr, key, removed);
                if (root != nullptr) root->parent = nullptr;
                if (!removed) std::cout << "NOT_FOUND\n";
                else {
                    --size;
                    std::cout << "REMOVED\n";
                }
            } else {
                Node* node = find(root, key);
                if (command == "contains") {
                    std::cout << (node != nullptr ? "true" : "false") << '\n';
                } else if (node == nullptr) {
                    std::cout << "NOT_FOUND\n";
                } else if (command == "parent") {
                    if (node->parent == nullptr) std::cout << "NONE\n";
                    else std::cout << node->parent->key << '\n';
                } else if (command == "height") {
                    std::cout << node->height << '\n';
                } else {
                    std::cout << balance(node) << '\n';
                }
            }
        }
    }
    destroy(root);
}
