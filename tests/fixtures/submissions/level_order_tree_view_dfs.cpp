#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

struct Node {
    int id = 0;
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
        int value = 0;
        int id = 0;
        std::cin >> id >> value >> leftIds[id] >> rightIds[id];
        nodes[id].id = id;
    }
    for (int id = 0; id < nodeCount; ++id) {
        nodes[id].left = leftIds[id] == -1 ? nullptr : &nodes[leftIds[id]];
        nodes[id].right = rightIds[id] == -1 ? nullptr : &nodes[rightIds[id]];
    }
    Node* root = rootId == -1 ? nullptr : &nodes[rootId];
    std::function<int(Node*)> height = [&](Node* node) {
        if (node == nullptr) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    };
    std::function<int(Node*, int, int)> depthOf =
        [&](Node* node, int id, int depth) {
            if (node == nullptr) return -1;
            if (node->id == id) return depth;
            const int left = depthOf(node->left, id, depth + 1);
            return left != -1 ? left : depthOf(node->right, id, depth + 1);
        };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "levelorder") {
            bool first = true;
            std::function<void(Node*)> preorder = [&](Node* node) {
                if (node == nullptr) return;
                if (!first) std::cout << ' ';
                std::cout << node->id;
                first = false;
                preorder(node->left);
                preorder(node->right);
            };
            preorder(root);
            if (first) std::cout << "EMPTY";
            std::cout << '\n';
        } else if (command == "level" || command == "width") {
            int target = 0;
            std::cin >> target;
            if (target < 0) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int count = 0;
            bool first = true;
            std::function<void(Node*, int)> visit = [&](Node* node, int depth) {
                if (node == nullptr) return;
                if (depth == target) {
                    ++count;
                    if (command == "level") {
                        if (!first) std::cout << ' ';
                        std::cout << node->id;
                        first = false;
                    }
                }
                visit(node->left, depth + 1);
                visit(node->right, depth + 1);
            };
            visit(root, 0);
            if (command == "width") std::cout << count << '\n';
            else {
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        } else if (command == "depth") {
            int id = 0;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) std::cout << "NOT_FOUND\n";
            else std::cout << depthOf(root, id, 0) << '\n';
        } else if (command == "levels") {
            std::cout << height(root) << '\n';
        } else if (command == "queue_peak") {
            std::cout << (root == nullptr ? 0 : 1) << '\n';
        }
    }
}
