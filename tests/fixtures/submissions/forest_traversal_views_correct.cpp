#include <iostream>
#include <string>

void printValue(int value, bool& first) {
    if (!first) std::cout << ' ';
    std::cout << value;
    first = false;
}

void preorder(const int* parent, int count, int node, bool& first) {
    printValue(node, first);
    for (int child = 0; child < count; ++child) {
        if (parent[child] == node) preorder(parent, count, child, first);
    }
}

void postorder(const int* parent, int count, int node, bool& first) {
    for (int child = 0; child < count; ++child) {
        if (parent[child] == node) postorder(parent, count, child, first);
    }
    printValue(node, first);
}

int height(const int* parent, int count, int node) {
    int best = 0;
    for (int child = 0; child < count; ++child) {
        if (parent[child] == node) {
            const int childHeight = height(parent, count, child);
            if (childHeight > best) best = childHeight;
        }
    }
    return best + 1;
}

int main() {
    int nodeCount = 0;
    int operationCount = 0;
    std::cin >> nodeCount >> operationCount;
    int* parent = nodeCount == 0 ? nullptr : new int[nodeCount];
    for (int id = 0; id < nodeCount; ++id) std::cin >> parent[id];
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "preorder" || command == "postorder") {
            bool first = true;
            for (int root = 0; root < nodeCount; ++root) {
                if (parent[root] == -1) {
                    if (command == "preorder") {
                        preorder(parent, nodeCount, root, first);
                    } else {
                        postorder(parent, nodeCount, root, first);
                    }
                }
            }
            if (first) std::cout << "EMPTY";
            std::cout << '\n';
        } else if (command == "levelorder") {
            if (nodeCount == 0) {
                std::cout << "EMPTY\n";
                continue;
            }
            int* queue = new int[nodeCount];
            int front = 0;
            int back = 0;
            for (int root = 0; root < nodeCount; ++root) {
                if (parent[root] == -1) queue[back++] = root;
            }
            bool first = true;
            while (front < back) {
                const int node = queue[front++];
                printValue(node, first);
                for (int child = 0; child < nodeCount; ++child) {
                    if (parent[child] == node) queue[back++] = child;
                }
            }
            if (first) std::cout << "EMPTY";
            std::cout << '\n';
            delete[] queue;
        } else if (command == "roots") {
            bool first = true;
            for (int root = 0; root < nodeCount; ++root) {
                if (parent[root] == -1) printValue(root, first);
            }
            if (first) std::cout << "EMPTY";
            std::cout << '\n';
        } else {
            int id = -1;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "tree_preorder" ||
                       command == "tree_postorder") {
                bool first = true;
                if (command == "tree_preorder") {
                    preorder(parent, nodeCount, id, first);
                } else {
                    postorder(parent, nodeCount, id, first);
                }
                std::cout << '\n';
            } else if (command == "depth") {
                int depth = 0;
                while (parent[id] != -1) {
                    id = parent[id];
                    ++depth;
                }
                std::cout << depth << '\n';
            } else {
                std::cout << height(parent, nodeCount, id) << '\n';
            }
        }
    }
    delete[] parent;
}
