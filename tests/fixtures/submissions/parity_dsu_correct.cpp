#include <iostream>
#include <string>

// Parity (two-coloring) disjoint set. weight[x] holds the parity of x relative
// to its parent: 0 means same color, 1 means different. find compresses while
// xor-ing the parity so weight[x] becomes the parity of x relative to its root.

int* parent = nullptr;
int* weight = nullptr;

int find(int value) {
    if (parent[value] == value) return value;
    const int root = find(parent[value]);
    weight[value] ^= weight[parent[value]];
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
    weight = count == 0 ? nullptr : new int[count];
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
                std::cout << ((weight[left] ^ weight[right]) == 0 ? "same"
                                                                  : "diff")
                          << '\n';
            }
        } else {  // relate a b same|diff
            int left = -1;
            int right = -1;
            std::string relation;
            std::cin >> left >> right >> relation;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int wanted = relation == "diff" ? 1 : 0;
            const int leftRoot = find(left);
            const int rightRoot = find(right);
            if (leftRoot == rightRoot) {
                if ((weight[left] ^ weight[right]) == wanted) {
                    std::cout << "CONSISTENT\n";
                } else {
                    std::cout << "CONFLICT\n";
                }
            } else {
                parent[rightRoot] = leftRoot;
                weight[rightRoot] = weight[left] ^ weight[right] ^ wanted;
                --components;
                std::cout << "MERGED\n";
            }
        }
    }
    delete[] weight;
    delete[] parent;
}
