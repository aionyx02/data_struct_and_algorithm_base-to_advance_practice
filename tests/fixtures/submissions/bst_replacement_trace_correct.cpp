#include <iostream>
#include <string>

struct Node {
    int id;
    int key;
    Node* parent;
    Node* left;
    Node* right;
};

Node* findKey(Node* root, int key) {
    while (root != nullptr && root->key != key) {
        root = key < root->key ? root->left : root->right;
    }
    return root;
}

Node* findId(Node* node, int id) {
    if (node == nullptr || node->id == id) return node;
    if (Node* found = findId(node->left, id)) return found;
    return findId(node->right, id);
}

Node* minimum(Node* node) {
    while (node->left != nullptr) node = node->left;
    return node;
}

void transplant(Node*& root, Node* oldNode, Node* newNode) {
    if (oldNode->parent == nullptr) root = newNode;
    else if (oldNode->parent->left == oldNode) oldNode->parent->left = newNode;
    else oldNode->parent->right = newNode;
    if (newNode != nullptr) newNode->parent = oldNode->parent;
}

void printInorder(Node* node, bool& first) {
    if (node == nullptr) return;
    printInorder(node->left, first);
    if (!first) std::cout << ' ';
    std::cout << node->id;
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
            int id = 0;
            int key = 0;
            std::cin >> id >> key;
            if (findId(root, id) != nullptr || findKey(root, key) != nullptr) {
                std::cout << "DUPLICATE\n";
                continue;
            }
            Node* parent = nullptr;
            Node* current = root;
            while (current != nullptr) {
                parent = current;
                current = key < current->key
                    ? current->left
                    : current->right;
            }
            Node* node = new Node{id, key, parent, nullptr, nullptr};
            if (parent == nullptr) root = node;
            else if (key < parent->key) parent->left = node;
            else parent->right = node;
            ++size;
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            Node* node = findKey(root, key);
            if (node == nullptr) {
                std::cout << "NOT_FOUND\n";
                continue;
            }
            std::cout << node->id << '\n';
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
        } else if (command == "find") {
            int key = 0;
            std::cin >> key;
            Node* node = findKey(root, key);
            if (node == nullptr) std::cout << "NOT_FOUND\n";
            else std::cout << node->id << '\n';
        } else if (command == "key" || command == "parent" ||
                   command == "left" || command == "right") {
            int id = 0;
            std::cin >> id;
            Node* node = findId(root, id);
            if (node == nullptr) {
                std::cout << "NOT_FOUND\n";
            } else if (command == "key") {
                std::cout << node->key << '\n';
            } else {
                Node* link = command == "parent"
                    ? node->parent
                    : (command == "left" ? node->left : node->right);
                if (link == nullptr) std::cout << "NONE\n";
                else std::cout << link->id << '\n';
            }
        } else if (command == "root") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else std::cout << root->id << '\n';
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
