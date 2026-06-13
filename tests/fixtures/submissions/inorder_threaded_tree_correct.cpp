#include <functional>
#include <iostream>
#include <memory>
#include <string>

struct Node {
    int id = 0;
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;
    bool leftThread = false;
    bool rightThread = false;
};

int main() {
    int nodeCount = 0;
    int rootId = -1;
    int operationCount = 0;
    std::cin >> nodeCount >> rootId >> operationCount;
    auto nodes = std::make_unique<Node[]>(nodeCount);
    auto leftIds = std::make_unique<int[]>(nodeCount);
    auto rightIds = std::make_unique<int[]>(nodeCount);
    for (int row = 0; row < nodeCount; ++row) {
        int id = 0;
        std::cin >> id >> nodes[id].value >> leftIds[id] >> rightIds[id];
        nodes[id].id = id;
    }
    for (int id = 0; id < nodeCount; ++id) {
        nodes[id].left = leftIds[id] == -1 ? nullptr : &nodes[leftIds[id]];
        nodes[id].right =
            rightIds[id] == -1 ? nullptr : &nodes[rightIds[id]];
    }
    Node* root = rootId == -1 ? nullptr : &nodes[rootId];
    Node* previous = nullptr;
    std::function<void(Node*)> thread = [&](Node* node) {
        if (node == nullptr) return;
        Node* actualLeft = node->left;
        Node* actualRight = node->right;
        thread(actualLeft);
        if (actualLeft == nullptr) {
            node->left = previous;
            node->leftThread = true;
        }
        if (previous != nullptr && previous->right == nullptr) {
            previous->right = node;
            previous->rightThread = true;
        }
        previous = node;
        thread(actualRight);
    };
    thread(root);
    if (previous != nullptr && previous->right == nullptr) {
        previous->rightThread = true;
    }

    auto valid = [&](int id) { return id >= 0 && id < nodeCount; };
    auto successor = [&](Node* node) {
        if (node->rightThread) return node->right;
        Node* next = node->right;
        while (next != nullptr && !next->leftThread) next = next->left;
        return next;
    };
    auto predecessor = [&](Node* node) {
        if (node->leftThread) return node->left;
        Node* prior = node->left;
        while (prior != nullptr && !prior->rightThread) prior = prior->right;
        return prior;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "inorder") {
            if (root == nullptr) {
                std::cout << "EMPTY\n";
                continue;
            }
            Node* current = root;
            while (!current->leftThread) current = current->left;
            bool first = true;
            while (current != nullptr) {
                if (!first) std::cout << ' ';
                std::cout << current->id;
                first = false;
                current = successor(current);
            }
            std::cout << '\n';
        } else {
            int id = 0;
            std::cin >> id;
            if (!valid(id)) {
                std::cout << "NOT_FOUND\n";
                continue;
            }
            Node* node = &nodes[id];
            if (command == "value") {
                std::cout << node->value << '\n';
            } else if (command == "predecessor" || command == "successor") {
                Node* result =
                    command == "predecessor" ? predecessor(node) : successor(node);
                if (result == nullptr) std::cout << "NONE\n";
                else std::cout << result->id << '\n';
            } else {
                Node* link =
                    command == "left_link" ? node->left : node->right;
                const bool isThread = command == "left_link"
                    ? node->leftThread
                    : node->rightThread;
                if (link == nullptr) std::cout << "NONE\n";
                else std::cout << (isThread ? "THREAD " : "CHILD ")
                               << link->id << '\n';
            }
        }
    }
}
