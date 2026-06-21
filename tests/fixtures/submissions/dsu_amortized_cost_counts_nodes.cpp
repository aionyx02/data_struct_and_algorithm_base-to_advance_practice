#include <iostream>
#include <string>

// Known-wrong: counts the nodes on the path including the root (an off-by-one),
// so every find reports one more than the true edge count.

int* parent = nullptr;
int* rankOf = nullptr;

int findRoot(int value, int& nodes) {
    nodes = 1;  // counts the starting node, then the root, plus interior nodes
    int root = value;
    while (parent[root] != root) {
        root = parent[root];
        ++nodes;
    }
    int current = value;
    while (parent[current] != root) {
        const int next = parent[current];
        parent[current] = root;
        current = next;
    }
    return root;
}

bool valid(int value, int count) {
    return value >= 0 && value < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    parent = count == 0 ? nullptr : new int[count];
    rankOf = count == 0 ? nullptr : new int[count];
    for (int index = 0; index < count; ++index) {
        parent[index] = index;
        rankOf[index] = 0;
    }
    int components = count;
    int ignored = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "components") {
            std::cout << components << '\n';
        } else if (command == "snapshot") {
            if (count == 0) std::cout << "EMPTY";
            for (int index = 0; index < count; ++index) {
                if (index != 0) std::cout << ' ';
                std::cout << parent[index];
            }
            std::cout << '\n';
        } else if (command == "find" || command == "parent") {
            int value = -1;
            std::cin >> value;
            if (!valid(value, count)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "parent") {
                std::cout << parent[value] << '\n';
            } else {
                int nodes = 0;
                findRoot(value, nodes);
                std::cout << nodes << '\n';
            }
        } else {
            int left = -1;
            int right = -1;
            std::cin >> left >> right;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int leftRoot = findRoot(left, ignored);
            int rightRoot = findRoot(right, ignored);
            if (command == "connected") {
                std::cout << (leftRoot == rightRoot ? "true" : "false") << '\n';
            } else if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
            } else {
                if (rankOf[leftRoot] < rankOf[rightRoot]) {
                    const int temporary = leftRoot;
                    leftRoot = rightRoot;
                    rightRoot = temporary;
                }
                parent[rightRoot] = leftRoot;
                if (rankOf[leftRoot] == rankOf[rightRoot]) ++rankOf[leftRoot];
                --components;
                std::cout << leftRoot << '\n';
            }
        }
    }
    delete[] rankOf;
    delete[] parent;
}
