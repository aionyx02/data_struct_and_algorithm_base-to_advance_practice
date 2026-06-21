#include <iostream>
#include <string>

// Persistent disjoint set. Version 0 is all singletons. Every successful unite
// forks a brand new version from the requested one, leaving older versions
// untouched, so any past version can still be queried. Unions are by size with
// no compression to keep each version's structure stable.

int** par = nullptr;
int** sz = nullptr;
int* comp = nullptr;
int versions = 1;
int count = 0;

int findIn(int version, int value) {
    while (par[version][value] != value) value = par[version][value];
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
    const int capacity = operationCount + 1;
    par = new int*[capacity];
    sz = new int*[capacity];
    comp = new int[capacity];
    par[0] = new int[count == 0 ? 1 : count];
    sz[0] = new int[count == 0 ? 1 : count];
    for (int index = 0; index < count; ++index) {
        par[0][index] = index;
        sz[0][index] = 1;
    }
    comp[0] = count;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "components" || command == "snapshot") {
            int version = -1;
            std::cin >> version;
            if (!validVersion(version)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "components") {
                std::cout << comp[version] << '\n';
            } else {
                if (count == 0) std::cout << "EMPTY";
                for (int index = 0; index < count; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << par[version][index];
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
                const int root = findIn(version, value);
                if (command == "find") std::cout << root << '\n';
                else std::cout << sz[version][root] << '\n';
            }
        } else if (command == "connected") {
            int version = -1;
            int left = -1;
            int right = -1;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !valid(left) || !valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << (findIn(version, left) == findIn(version, right)
                                  ? "true"
                                  : "false")
                          << '\n';
            }
        } else {  // unite v a b
            int version = -1;
            int left = -1;
            int right = -1;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !valid(left) || !valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int leftRoot = findIn(version, left);
            int rightRoot = findIn(version, right);
            if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
                continue;
            }
            // Fork a fresh version that copies the requested one.
            par[versions] = new int[count == 0 ? 1 : count];
            sz[versions] = new int[count == 0 ? 1 : count];
            for (int index = 0; index < count; ++index) {
                par[versions][index] = par[version][index];
                sz[versions][index] = sz[version][index];
            }
            comp[versions] = comp[version];
            if (sz[versions][leftRoot] < sz[versions][rightRoot]) {
                const int temporary = leftRoot;
                leftRoot = rightRoot;
                rightRoot = temporary;
            }
            par[versions][rightRoot] = leftRoot;
            sz[versions][leftRoot] += sz[versions][rightRoot];
            --comp[versions];
            std::cout << versions << '\n';
            ++versions;
        }
    }
    for (int version = 0; version < versions; ++version) {
        delete[] par[version];
        delete[] sz[version];
    }
    delete[] comp;
    delete[] sz;
    delete[] par;
}
