#include <iostream>

int main() {
    int count = 0;
    int root = -1;
    std::cin >> count >> root;
    bool sorted = true;
    for (int index = 0; index < count; ++index) {
        int id = 0;
        int keyCount = 0;
        int keys[3];
        int children[4];
        std::cin >> id >> keyCount;
        for (int& key : keys) std::cin >> key;
        for (int& child : children) std::cin >> child;
        for (int key = 1; key < keyCount; ++key) {
            sorted = sorted && keys[key - 1] < keys[key];
        }
    }
    std::cout << (sorted ? "VALID" : "INVALID_KEYS") << '\n';
}
