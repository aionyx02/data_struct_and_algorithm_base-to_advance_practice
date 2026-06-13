#include <iostream>
#include <memory>
#include <string>

struct Node {
    int value = 0;
    int left = -1;
    int right = -1;
};

int main() {
    int nodeCount = 0;
    int rootId = -1;
    int operationCount = 0;
    std::cin >> nodeCount >> rootId >> operationCount;
    auto nodes = std::make_unique<Node[]>(nodeCount);
    for (int row = 0; row < nodeCount; ++row) {
        int id = 0;
        std::cin >> id >> nodes[id].value >> nodes[id].left >> nodes[id].right;
    }
    auto valid = [&](int id) { return id >= 0 && id < nodeCount; };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "inorder") {
            if (nodeCount == 0) std::cout << "EMPTY\n";
            else {
                for (int id = 0; id < nodeCount; ++id) {
                    if (id != 0) std::cout << ' ';
                    std::cout << id;
                }
                std::cout << '\n';
            }
        } else {
            int id = 0;
            std::cin >> id;
            if (!valid(id)) {
                std::cout << "NOT_FOUND\n";
            } else if (command == "value") {
                std::cout << nodes[id].value << '\n';
            } else if (command == "predecessor") {
                if (id == 0) std::cout << "NONE\n";
                else std::cout << id - 1 << '\n';
            } else if (command == "successor") {
                if (id + 1 == nodeCount) std::cout << "NONE\n";
                else std::cout << id + 1 << '\n';
            } else {
                const bool left = command == "left_link";
                const int child = left ? nodes[id].left : nodes[id].right;
                if (child != -1) std::cout << "CHILD " << child << '\n';
                else {
                    const int thread = left ? id - 1 : id + 1;
                    if (!valid(thread)) std::cout << "NONE\n";
                    else std::cout << "THREAD " << thread << '\n';
                }
            }
        }
    }
    (void)rootId;
}
