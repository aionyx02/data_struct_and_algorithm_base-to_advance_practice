#include <iostream>
#include <string>

int findRoot(const int* parent, int value) {
    while (parent[value] != value) value = parent[value];
    return value;
}

bool valid(int value, int count) {
    return value >= 0 && value < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    int* parent = count == 0 ? nullptr : new int[count];
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "components") std::cout << components << '\n';
        else if (command == "snapshot") {
            if (count == 0) std::cout << "EMPTY";
            for (int index = 0; index < count; ++index) {
                if (index != 0) std::cout << ' ';
                std::cout << parent[index];
            }
            std::cout << '\n';
        } else if (command == "find" || command == "parent" ||
                   command == "component_size") {
            int value = -1;
            std::cin >> value;
            if (!valid(value, count)) std::cout << "OUT_OF_RANGE\n";
            else if (command == "find") std::cout << findRoot(parent, value) << '\n';
            else if (command == "parent") std::cout << parent[value] << '\n';
            else {
                const int root = findRoot(parent, value);
                int size = 0;
                for (int index = 0; index < count; ++index) {
                    if (findRoot(parent, index) == root) ++size;
                }
                std::cout << size << '\n';
            }
        } else {
            int left = -1;
            int right = -1;
            std::cin >> left >> right;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int leftRoot = findRoot(parent, left);
            const int rightRoot = findRoot(parent, right);
            if (command == "connected") {
                std::cout << (leftRoot == rightRoot ? "true" : "false") << '\n';
            } else if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
            } else {
                parent[right] = left;
                --components;
                std::cout << leftRoot << '\n';
            }
        }
    }
    delete[] parent;
}
