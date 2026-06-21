#include <iostream>
#include <string>

// Order-statistic red-black tree with unique integer keys. Nodes carry a color
// and a subtree size; insertion and deletion restore the red-black invariants
// with rotations and recoloring, and the subtree sizes make select / rank /
// range_count run in O(log n).

enum Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    int size;
    Node* left;
    Node* right;
    Node* parent;
};

Node* nil = nullptr;
Node* root = nullptr;

void updateSize(Node* node) {
    if (node != nil) node->size = 1 + node->left->size + node->right->size;
}

void leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
    updateSize(x);
    updateSize(y);
}

void rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nil) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nil) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
    updateSize(x);
    updateSize(y);
}

void insertFixup(Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

bool insertKey(int key) {
    Node* y = nil;
    Node* x = root;
    while (x != nil) {
        y = x;
        if (key == x->key) return false;
        x = key < x->key ? x->left : x->right;
    }
    Node* z = new Node{key, RED, 1, nil, nil, y};
    if (y == nil) root = z;
    else if (key < y->key) y->left = z;
    else y->right = z;
    for (Node* p = y; p != nil; p = p->parent) updateSize(p);
    insertFixup(z);
    return true;
}

void transplant(Node* u, Node* v) {
    if (u->parent == nil) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

Node* treeMinimum(Node* node) {
    while (node->left != nil) node = node->left;
    return node;
}

void deleteFixup(Node* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

Node* findNode(int key) {
    Node* x = root;
    while (x != nil) {
        if (key == x->key) return x;
        x = key < x->key ? x->left : x->right;
    }
    return nil;
}

bool eraseKey(int key) {
    Node* z = findNode(key);
    if (z == nil) return false;
    Node* y = z;
    Color yOriginalColor = y->color;
    Node* x = nil;
    Node* fixParent = nil;
    if (z->left == nil) {
        x = z->right;
        fixParent = z->parent;
        transplant(z, z->right);
    } else if (z->right == nil) {
        x = z->left;
        fixParent = z->parent;
        transplant(z, z->left);
    } else {
        y = treeMinimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
            fixParent = y;
        } else {
            fixParent = y->parent;
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    for (Node* p = fixParent; p != nil; p = p->parent) updateSize(p);
    if (yOriginalColor == BLACK) deleteFixup(x);
    nil->parent = nil;
    return true;
}

Node* selectNode(int k) {
    Node* node = root;
    while (node != nil) {
        const int leftSize = node->left->size;
        if (k == leftSize + 1) return node;
        if (k <= leftSize) {
            node = node->left;
        } else {
            k -= leftSize + 1;
            node = node->right;
        }
    }
    return nil;
}

int countLess(int key) {
    int count = 0;
    Node* node = root;
    while (node != nil) {
        if (key <= node->key) {
            node = node->left;
        } else {
            count += node->left->size + 1;
            node = node->right;
        }
    }
    return count;
}

int countAtMost(int key) {
    int count = 0;
    Node* node = root;
    while (node != nil) {
        if (key < node->key) {
            node = node->left;
        } else {
            count += node->left->size + 1;
            node = node->right;
        }
    }
    return count;
}

void inorder(Node* node, bool& first) {
    if (node == nil) return;
    inorder(node->left, first);
    if (!first) std::cout << ' ';
    std::cout << node->key;
    first = false;
    inorder(node->right, first);
}

int main() {
    nil = new Node{0, BLACK, 0, nullptr, nullptr, nullptr};
    nil->left = nil;
    nil->right = nil;
    nil->parent = nil;
    root = nil;

    int operationCount = 0;
    std::cin >> operationCount;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            if (!insertKey(key)) std::cout << "DUPLICATE\n";
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            std::cout << (eraseKey(key) ? "REMOVED" : "NOT_FOUND") << '\n';
        } else if (command == "contains") {
            int key = 0;
            std::cin >> key;
            std::cout << (findNode(key) != nil ? "true" : "false") << '\n';
        } else if (command == "select") {
            int k = 0;
            std::cin >> k;
            if (k < 1 || k > root->size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << selectNode(k)->key << '\n';
        } else if (command == "rank") {
            int key = 0;
            std::cin >> key;
            std::cout << countLess(key) << '\n';
        } else if (command == "range_count") {
            int low = 0;
            int high = 0;
            std::cin >> low >> high;
            if (low > high) std::cout << 0 << '\n';
            else std::cout << (countAtMost(high) - countLess(low)) << '\n';
        } else if (command == "predecessor") {
            int key = 0;
            std::cin >> key;
            const int count = countLess(key);
            if (count == 0) std::cout << "NONE\n";
            else std::cout << selectNode(count)->key << '\n';
        } else if (command == "successor") {
            int key = 0;
            std::cin >> key;
            const int count = countAtMost(key);
            if (count >= root->size) std::cout << "NONE\n";
            else std::cout << selectNode(count + 1)->key << '\n';
        } else if (command == "min") {
            if (root == nil) std::cout << "EMPTY\n";
            else std::cout << treeMinimum(root)->key << '\n';
        } else if (command == "max") {
            if (root == nil) {
                std::cout << "EMPTY\n";
            } else {
                Node* node = root;
                while (node->right != nil) node = node->right;
                std::cout << node->key << '\n';
            }
        } else if (command == "size") {
            std::cout << root->size << '\n';
        } else if (command == "inorder") {
            if (root == nil) {
                std::cout << "EMPTY\n";
            } else {
                bool first = true;
                inorder(root, first);
                std::cout << '\n';
            }
        } else if (command == "clear") {
            // Tear down without recursion sensitivity: repeatedly delete min.
            while (root != nil) eraseKey(root->key);
        }
    }
    return 0;
}
