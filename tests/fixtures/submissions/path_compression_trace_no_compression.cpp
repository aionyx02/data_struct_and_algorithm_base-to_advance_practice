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
    int* sizes = count == 0 ? nullptr : new int[count]{};
    for (int index = 0; index < count; ++index) std::cin >> parent[index];
    int components = 0;
    for (int index = 0; index < count; ++index) {
        if (parent[index] == index) ++components;
        ++sizes[findRoot(parent, index)];
    }
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
            else if (command == "parent") std::cout << parent[value] << '\n';
            else {
                const int root = findRoot(parent, value);
                if (command == "find") std::cout << root << '\n';
                else std::cout << sizes[root] << '\n';
            }
        } else {
            int left = -1;
            int right = -1;
            std::cin >> left >> right;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int leftRoot = findRoot(parent, left);
            int rightRoot = findRoot(parent, right);
            if (command == "connected") {
                std::cout << (leftRoot == rightRoot ? "true" : "false") << '\n';
            } else if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
            } else {
                if (sizes[leftRoot] < sizes[rightRoot]) {
                    const int temporary = leftRoot;
                    leftRoot = rightRoot;
                    rightRoot = temporary;
                }
                parent[rightRoot] = leftRoot;
                sizes[leftRoot] += sizes[rightRoot];
                --components;
                std::cout << leftRoot << '\n';
            }
        }
    }
    delete[] sizes;
    delete[] parent;
}
