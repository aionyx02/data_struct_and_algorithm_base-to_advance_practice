#include <iostream>
#include <string>

struct Node {
    int id;
    int value;
    Node* parent;
    Node* left;
    Node* right;
};

Node* findNode(Node* node, int id) {
    if (node == nullptr || node->id == id) return node;
    if (Node* found = findNode(node->left, id)) return found;
    return findNode(node->right, id);
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
        if (command == "root") {
            int id = 0;
            int value = 0;
            std::cin >> id >> value;
            if (root != nullptr) std::cout << "OCCUPIED\n";
            else {
                root = new Node{id, value, nullptr, nullptr, nullptr};
                size = 1;
            }
        } else if (command == "attach_left" || command == "attach_right") {
            int parentId = 0;
            int childId = 0;
            int value = 0;
            std::cin >> parentId >> childId >> value;
            Node* parent = findNode(root, parentId);
            if (parent == nullptr) std::cout << "NOT_FOUND\n";
            else if (findNode(root, childId) != nullptr) std::cout << "DUPLICATE\n";
            else {
                Node*& link =
                    command == "attach_left" ? parent->left : parent->right;
                if (link != nullptr) std::cout << "OCCUPIED\n";
                else {
                    link = new Node{childId, value, parent, nullptr, nullptr};
                    ++size;
                }
            }
        } else if (command == "value" || command == "parent" ||
                   command == "left" || command == "right") {
            int id = 0;
            std::cin >> id;
            Node* node = findNode(root, id);
            if (node == nullptr) std::cout << "NOT_FOUND\n";
            else if (command == "value") std::cout << node->value << '\n';
            else {
                Node* link = command == "parent"
                    ? nullptr
                    : (command == "left" ? node->left : node->right);
                if (link == nullptr) std::cout << "NONE\n";
                else std::cout << link->id << '\n';
            }
        } else if (command == "erase_leaf") {
            int id = 0;
            std::cin >> id;
            Node* node = findNode(root, id);
            if (node == nullptr) std::cout << "NOT_FOUND\n";
            else if (node->left != nullptr || node->right != nullptr) {
                std::cout << "NOT_LEAF\n";
            } else {
                std::cout << node->value << '\n';
                if (node->parent == nullptr) root = nullptr;
                else if (node->parent->left == node) node->parent->left = nullptr;
                else node->parent->right = nullptr;
                delete node;
                --size;
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
