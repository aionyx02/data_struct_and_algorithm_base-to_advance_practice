#include <iostream>
#include <string>

struct Node {
    int parent;
    int firstChild;
    int nextSibling;
};

int subtreeSize(const Node* nodes, int id) {
    int result = 1;
    for (int child = nodes[id].firstChild;
         child != -1;
         child = nodes[child].nextSibling) {
        result += subtreeSize(nodes, child);
    }
    return result;
}

int main() {
    int nodeCount = 0;
    int rootCount = 0;
    int operationCount = 0;
    std::cin >> nodeCount >> rootCount >> operationCount;
    int* roots = rootCount == 0 ? nullptr : new int[rootCount];
    for (int index = 0; index < rootCount; ++index) std::cin >> roots[index];
    Node* nodes = nodeCount == 0 ? nullptr : new Node[nodeCount];
    for (int row = 0; row < nodeCount; ++row) {
        int id = 0;
        std::cin >> id >> nodes[id].parent >> nodes[id].firstChild
                 >> nodes[id].nextSibling;
    }

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "roots") {
            if (rootCount == 0) std::cout << "EMPTY\n";
            else {
                for (int index = 0; index < rootCount; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << roots[index];
                }
                std::cout << '\n';
            }
            continue;
        }
        int id = -1;
        std::cin >> id;
        if (id < 0 || id >= nodeCount) {
            std::cout << "OUT_OF_RANGE\n";
        } else if (command == "parent" || command == "first_child" ||
                   command == "next_sibling") {
            const int link = command == "parent"
                ? nodes[id].parent
                : (command == "first_child"
                    ? nodes[id].firstChild
                    : nodes[id].nextSibling);
            if (link == -1) std::cout << "NONE\n";
            else std::cout << link << '\n';
        } else if (command == "root") {
            int root = id;
            while (nodes[root].parent != -1) root = nodes[root].parent;
            std::cout << root << '\n';
        } else if (command == "subtree_size") {
            std::cout << subtreeSize(nodes, id) << '\n';
        } else {
            int child = nodes[id].firstChild;
            if (child == -1) std::cout << "EMPTY";
            bool first = true;
            while (child != -1) {
                if (!first) std::cout << ' ';
                std::cout << child;
                first = false;
                child = nodes[child].nextSibling;
            }
            std::cout << '\n';
        }
    }
    delete[] nodes;
    delete[] roots;
}
