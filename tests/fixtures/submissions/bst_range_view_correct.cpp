#include <iostream>
#include <string>

struct Node {
    int key;
    Node* left;
    Node* right;
};

void deleteTree(Node* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void printRange(Node* node, int low, int high, bool& first) {
    if (node == nullptr) return;
    if (low < node->key) printRange(node->left, low, high, first);
    if (low <= node->key && node->key <= high) {
        if (!first) std::cout << ' ';
        std::cout << node->key;
        first = false;
    }
    if (node->key < high) printRange(node->right, low, high, first);
}

int countRange(Node* node, int low, int high) {
    if (node == nullptr) return 0;
    if (node->key < low) return countRange(node->right, low, high);
    if (node->key > high) return countRange(node->left, low, high);
    return 1 + countRange(node->left, low, high) +
           countRange(node->right, low, high);
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
            Node** link = &root;
            while (*link != nullptr && (*link)->key != key) {
                link = key < (*link)->key
                    ? &(*link)->left
                    : &(*link)->right;
            }
            if (*link != nullptr) std::cout << "DUPLICATE\n";
            else {
                *link = new Node{key, nullptr, nullptr};
                ++size;
            }
        } else if (command == "range" || command == "count") {
            int low = 0;
            int high = 0;
            std::cin >> low >> high;
            if (command == "count") {
                std::cout << (low > high ? 0 : countRange(root, low, high))
                          << '\n';
            } else {
                bool first = true;
                if (low <= high) printRange(root, low, high, first);
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        } else if (command == "lower" || command == "upper") {
            int key = 0;
            std::cin >> key;
            Node* current = root;
            Node* answer = nullptr;
            while (current != nullptr) {
                const bool candidate = command == "lower"
                    ? current->key >= key
                    : current->key > key;
                if (candidate) {
                    answer = current;
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            if (answer == nullptr) std::cout << "NONE\n";
            else std::cout << answer->key << '\n';
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
