#include <iostream>
#include <string>

// Known-wrong: rank counts keys <= key instead of strictly less than key.
// classic AVL balance information with a subtree size so select / rank /
// range_count run in O(log n). Two-child deletion replaces the node with its
// in-order successor.

struct Node {
    int key;
    int height;
    int size;
    Node* left;
    Node* right;
};

int height(Node* node) { return node ? node->height : 0; }
int subtreeSize(Node* node) { return node ? node->size : 0; }

void update(Node* node) {
    node->height = 1 + (height(node->left) > height(node->right)
                            ? height(node->left)
                            : height(node->right));
    node->size = 1 + subtreeSize(node->left) + subtreeSize(node->right);
}

int balance(Node* node) { return node ? height(node->left) - height(node->right) : 0; }

Node* rotateRight(Node* y) {
    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    update(y);
    update(x);
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    update(x);
    update(y);
    return y;
}

Node* rebalance(Node* node) {
    update(node);
    const int factor = balance(node);
    if (factor > 1) {
        if (balance(node->left) < 0) node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (factor < -1) {
        if (balance(node->right) > 0) node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

Node* insert(Node* node, int key, bool& inserted) {
    if (!node) {
        inserted = true;
        return new Node{key, 1, 1, nullptr, nullptr};
    }
    if (key == node->key) {
        inserted = false;
        return node;
    }
    if (key < node->key) node->left = insert(node->left, key, inserted);
    else node->right = insert(node->right, key, inserted);
    return rebalance(node);
}

Node* minNode(Node* node) {
    while (node->left) node = node->left;
    return node;
}

Node* erase(Node* node, int key, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }
    if (key < node->key) {
        node->left = erase(node->left, key, removed);
    } else if (key > node->key) {
        node->right = erase(node->right, key, removed);
    } else {
        removed = true;
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            delete node;
            return child;
        }
        Node* successor = minNode(node->right);
        node->key = successor->key;
        bool dummy = false;
        node->right = erase(node->right, successor->key, dummy);
    }
    return rebalance(node);
}

Node* selectNode(Node* node, int k) {
    const int leftSize = subtreeSize(node->left);
    if (k <= leftSize) return selectNode(node->left, k);
    if (k == leftSize + 1) return node;
    return selectNode(node->right, k - leftSize - 1);
}

// Number of keys strictly less than key.
int countLess(Node* node, int key) {
    if (!node) return 0;
    if (key <= node->key) return countLess(node->left, key);
    return subtreeSize(node->left) + 1 + countLess(node->right, key);
}

// Number of keys less than or equal to key.
int countAtMost(Node* node, int key) {
    if (!node) return 0;
    if (key < node->key) return countAtMost(node->left, key);
    return subtreeSize(node->left) + 1 + countAtMost(node->right, key);
}

bool contains(Node* node, int key) {
    while (node) {
        if (key == node->key) return true;
        node = key < node->key ? node->left : node->right;
    }
    return false;
}

void inorder(Node* node, bool& first) {
    if (!node) return;
    inorder(node->left, first);
    if (!first) std::cout << ' ';
    std::cout << node->key;
    first = false;
    inorder(node->right, first);
}

void destroy(Node* node) {
    if (!node) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

int main() {
    int operationCount = 0;
    std::cin >> operationCount;
    Node* root = nullptr;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            bool inserted = false;
            root = insert(root, key, inserted);
            if (!inserted) std::cout << "DUPLICATE\n";
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            bool removed = false;
            root = erase(root, key, removed);
            std::cout << (removed ? "REMOVED" : "NOT_FOUND") << '\n';
        } else if (command == "contains") {
            int key = 0;
            std::cin >> key;
            std::cout << (contains(root, key) ? "true" : "false") << '\n';
        } else if (command == "select") {
            int k = 0;
            std::cin >> k;
            if (k < 1 || k > subtreeSize(root)) std::cout << "OUT_OF_RANGE\n";
            else std::cout << selectNode(root, k)->key << '\n';
        } else if (command == "rank") {
            int key = 0;
            std::cin >> key;
            std::cout << countAtMost(root, key) << '\n';  // bug: <= not <
        } else if (command == "range_count") {
            int low = 0;
            int high = 0;
            std::cin >> low >> high;
            if (low > high) std::cout << 0 << '\n';
            else std::cout << (countAtMost(root, high) - countLess(root, low))
                           << '\n';
        } else if (command == "min") {
            if (!root) std::cout << "EMPTY\n";
            else std::cout << minNode(root)->key << '\n';
        } else if (command == "max") {
            if (!root) {
                std::cout << "EMPTY\n";
            } else {
                Node* node = root;
                while (node->right) node = node->right;
                std::cout << node->key << '\n';
            }
        } else if (command == "size") {
            std::cout << subtreeSize(root) << '\n';
        } else if (command == "height") {
            std::cout << height(root) << '\n';
        } else if (command == "inorder") {
            if (!root) {
                std::cout << "EMPTY\n";
            } else {
                bool first = true;
                inorder(root, first);
                std::cout << '\n';
            }
        } else if (command == "clear") {
            destroy(root);
            root = nullptr;
        }
    }
    destroy(root);
}
