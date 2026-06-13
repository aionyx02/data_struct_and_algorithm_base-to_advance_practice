#include <algorithm>
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
    auto queue = std::make_unique<Node*[]>(nodeCount == 0 ? 1 : nodeCount);
    auto depths = std::make_unique<int[]>(nodeCount == 0 ? 1 : nodeCount);

    auto scan = [&](int targetDepth, bool printLevel, bool printAll) {
        if (root == nullptr) {
            if (printLevel || printAll) std::cout << "EMPTY\n";
            return 0;
        }
        int head = 0;
        int tail = 0;
        int peak = 1;
        int count = 0;
        bool first = true;
        queue[tail] = root;
        depths[tail++] = 0;
        while (head < tail) {
            Node* node = queue[head];
            const int depth = depths[head++];
            if (printAll || (printLevel && depth == targetDepth)) {
                if (!first) std::cout << ' ';
                std::cout << node->id;
                first = false;
            }
            if (depth == targetDepth) ++count;
            if (node->left != nullptr) {
                queue[tail] = node->left;
                depths[tail++] = depth + 1;
            }
            if (node->right != nullptr) {
                queue[tail] = node->right;
                depths[tail++] = depth + 1;
            }
            peak = std::max(peak, tail - head);
        }
        if ((printLevel || printAll) && first) std::cout << "EMPTY";
        if (printLevel || printAll) std::cout << '\n';
        return printAll ? peak : count;
    };
    auto depthOf = [&](int id) {
        if (root == nullptr) return -1;
        int head = 0;
        int tail = 0;
        queue[tail] = root;
        depths[tail++] = 0;
        while (head < tail) {
            Node* node = queue[head];
            const int depth = depths[head++];
            if (node->id == id) return depth;
            if (node->left != nullptr) {
                queue[tail] = node->left;
                depths[tail++] = depth + 1;
            }
            if (node->right != nullptr) {
                queue[tail] = node->right;
                depths[tail++] = depth + 1;
            }
        }
        return -1;
    };
    auto queuePeak = [&]() {
        if (root == nullptr) return 0;
        int head = 0;
        int tail = 0;
        int peak = 1;
        queue[tail++] = root;
        while (head < tail) {
            Node* node = queue[head++];
            if (node->left != nullptr) queue[tail++] = node->left;
            if (node->right != nullptr) queue[tail++] = node->right;
            peak = std::max(peak, tail - head);
        }
        return peak;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "levelorder") {
            scan(0, false, true);
        } else if (command == "level") {
            int depth = 0;
            std::cin >> depth;
            if (depth < 0) std::cout << "OUT_OF_RANGE\n";
            else scan(depth, true, false);
        } else if (command == "depth") {
            int id = 0;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) std::cout << "NOT_FOUND\n";
            else std::cout << depthOf(id) << '\n';
        } else if (command == "width") {
            int depth = 0;
            std::cin >> depth;
            if (depth < 0) std::cout << "OUT_OF_RANGE\n";
            else std::cout << scan(depth, false, false) << '\n';
        } else if (command == "levels") {
            int maximum = -1;
            for (int id = 0; id < nodeCount; ++id) {
                maximum = std::max(maximum, depthOf(id));
            }
            std::cout << maximum + 1 << '\n';
        } else if (command == "queue_peak") {
            std::cout << queuePeak() << '\n';
        }
    }
}
