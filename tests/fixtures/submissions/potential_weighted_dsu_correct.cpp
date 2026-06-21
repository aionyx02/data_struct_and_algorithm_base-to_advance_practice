#include <iostream>
#include <string>

// Weighted (potential) disjoint set. weight[x] stores potential(x) minus
// potential(parent(x)). find compresses the path while accumulating weights so
// that after compression weight[x] equals potential(x) minus potential(root).

int* parent = nullptr;
long long* weight = nullptr;

int find(int value) {
    if (parent[value] == value) return value;
    const int root = find(parent[value]);
    weight[value] += weight[parent[value]];
    parent[value] = root;
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
    weight = count == 0 ? nullptr : new long long[count];
    for (int index = 0; index < count; ++index) {
        parent[index] = index;
        weight[index] = 0;
    }
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
        } else if (command == "diff" || command == "connected") {
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
                std::cout << "UNKNOWN\n";
            } else {
                std::cout << (weight[right] - weight[left]) << '\n';
            }
        } else {  // relate a b w
            int left = -1;
            int right = -1;
            long long delta = 0;
            std::cin >> left >> right >> delta;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int leftRoot = find(left);
            const int rightRoot = find(right);
            if (leftRoot == rightRoot) {
                if (weight[right] - weight[left] == delta) {
                    std::cout << "CONSISTENT\n";
                } else {
                    std::cout << "CONFLICT\n";
                }
            } else {
                // Attach the root of b under the root of a so that
                // potential(b) - potential(a) == delta.
                parent[rightRoot] = leftRoot;
                weight[rightRoot] = delta + weight[left] - weight[right];
                --components;
                std::cout << "MERGED\n";
            }
        }
    }
    delete[] weight;
    delete[] parent;
}
