#include <iostream>
#include <string>

// Known-wrong: behaves like a plain connectivity DSU and ignores parity. Any
// two connected nodes are reported as "same", and a relation between connected
// nodes is never a CONFLICT.

int* parent = nullptr;

int find(int value) {
    if (parent[value] == value) return value;
    return parent[value] = find(parent[value]);
}

bool valid(int value, int count) {
    return value >= 0 && value < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    parent = count == 0 ? nullptr : new int[count];
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;
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
        } else if (command == "find") {
            int value = -1;
            std::cin >> value;
            if (!valid(value, count)) std::cout << "OUT_OF_RANGE\n";
            else std::cout << find(value) << '\n';
        } else if (command == "query" || command == "connected") {
            int left = -1;
            int right = -1;
            std::cin >> left >> right;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int leftRoot = find(left);
            const int rightRoot = find(right);
            if (command == "connected") {
                std::cout << (leftRoot == rightRoot ? "true" : "false") << '\n';
            } else if (leftRoot != rightRoot) {
                std::cout << "unknown\n";
            } else {
                std::cout << "same\n";  // bug: parity ignored
            }
        } else {
            int left = -1;
            int right = -1;
            std::string relation;
            std::cin >> left >> right >> relation;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int leftRoot = find(left);
            const int rightRoot = find(right);
            if (leftRoot == rightRoot) {
                std::cout << "CONSISTENT\n";  // bug: never detects CONFLICT
            } else {
                parent[rightRoot] = leftRoot;
                --components;
                std::cout << "MERGED\n";
            }
        }
    }
    delete[] parent;
}
