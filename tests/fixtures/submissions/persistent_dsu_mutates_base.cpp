#include <iostream>
#include <string>

// Known-wrong: keeps a single shared structure and mutates it in place on every
// unite, only handing out new version numbers. Queries against old versions see
// the latest state instead of the historical one.

int* par = nullptr;
int* sz = nullptr;
int components = 0;
int versions = 1;
int count = 0;

int findRoot(int value) {
    while (par[value] != value) value = par[value];
    return value;
}

bool valid(int value) {
    return value >= 0 && value < count;
}

bool validVersion(int version) {
    return version >= 0 && version < versions;
}

int main() {
    int operationCount = 0;
    std::cin >> count >> operationCount;
    par = new int[count == 0 ? 1 : count];
    sz = new int[count == 0 ? 1 : count];
    for (int index = 0; index < count; ++index) {
        par[index] = index;
        sz[index] = 1;
    }
    components = count;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "components" || command == "snapshot") {
            int version = -1;
            std::cin >> version;
            if (!validVersion(version)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "components") {
                std::cout << components << '\n';
            } else {
                if (count == 0) std::cout << "EMPTY";
                for (int index = 0; index < count; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << par[index];
                }
                std::cout << '\n';
            }
        } else if (command == "find" || command == "component_size") {
            int version = -1;
            int value = -1;
            std::cin >> version >> value;
            if (!validVersion(version) || !valid(value)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int root = findRoot(value);
                if (command == "find") std::cout << root << '\n';
                else std::cout << sz[root] << '\n';
            }
        } else if (command == "connected") {
            int version = -1;
            int left = -1;
            int right = -1;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !valid(left) || !valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << (findRoot(left) == findRoot(right) ? "true"
                                                               : "false")
                          << '\n';
            }
        } else {
            int version = -1;
            int left = -1;
            int right = -1;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !valid(left) || !valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int leftRoot = findRoot(left);
            int rightRoot = findRoot(right);
            if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
                continue;
            }
            if (sz[leftRoot] < sz[rightRoot]) {
                const int temporary = leftRoot;
                leftRoot = rightRoot;
                rightRoot = temporary;
            }
            par[rightRoot] = leftRoot;  // bug: mutates the single shared state
            sz[leftRoot] += sz[rightRoot];
            --components;
            std::cout << versions << '\n';
            ++versions;
        }
    }
    delete[] sz;
    delete[] par;
}
