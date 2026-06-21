#include <iostream>
#include <string>

// Known-wrong: forks from the most recently created version instead of the
// requested version v. Connectivity questions answered correctly for the latest
// lineage but wrong whenever a unite branches from an older version.

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
        } else {
            int version = -1;
            int left = -1;
            int right = -1;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !valid(left) || !valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            const int base = versions - 1;  // bug: ignores requested version v
            int leftRoot = findIn(base, left);
            int rightRoot = findIn(base, right);
            if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
                continue;
            }
            par[versions] = new int[count == 0 ? 1 : count];
            sz[versions] = new int[count == 0 ? 1 : count];
            for (int index = 0; index < count; ++index) {
                par[versions][index] = par[base][index];
                sz[versions][index] = sz[base][index];
            }
            comp[versions] = comp[base];
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
