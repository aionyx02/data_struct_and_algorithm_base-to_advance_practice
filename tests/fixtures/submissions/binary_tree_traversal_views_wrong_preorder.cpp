#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

struct Node {
    int id = 0;
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;
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
        nodes[id].right = rightIds[id] == -1 ? nullptr : &nodes[rightIds[id]];
    }
    Node* root = rootId == -1 ? nullptr : &nodes[rootId];
    auto printTraversal = [&](const std::string& order) {
        bool first = true;
        std::function<void(Node*)> visit = [&](Node* node) {
            if (node == nullptr) return;
            visit(node->left);
            if (order == "preorder" || order == "inorder") {
                if (!first) std::cout << ' ';
                std::cout << node->id;
                first = false;
            }
            visit(node->right);
            if (order == "postorder") {
                if (!first) std::cout << ' ';
                std::cout << node->id;
                first = false;
            }
        };
        visit(root);
        if (first) std::cout << "EMPTY";
        std::cout << '\n';
    };
    std::function<int(Node*)> height = [&](Node* node) {
        if (node == nullptr) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "preorder" || command == "inorder" ||
            command == "postorder") printTraversal(command);
        else if (command == "height") std::cout << height(root) << '\n';
        else if (command == "size") std::cout << nodeCount << '\n';
        else {
            int id = 0;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) std::cout << "NOT_FOUND\n";
            else std::cout << nodes[id].value << '\n';
        }
    }
}
