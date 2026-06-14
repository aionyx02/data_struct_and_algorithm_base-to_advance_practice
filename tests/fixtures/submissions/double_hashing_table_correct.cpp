#include <iostream>
#include <string>

int normalizedModulo(int key, int modulus) {
    const int remainder = key % modulus;
    return remainder < 0 ? remainder + modulus : remainder;
}

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    int* keys = new int[capacity]{};
    bool* used = new bool[capacity]{};
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "table") {
            for (int slot = 0; slot < capacity; ++slot) {
                if (slot) std::cout << ' ';
                if (used[slot]) std::cout << keys[slot];
                else std::cout << '.';
            }
            std::cout << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "capacity") {
            std::cout << capacity << '\n';
        } else if (command == "clear") {
            for (int slot = 0; slot < capacity; ++slot) used[slot] = false;
            size = 0;
        } else {
            int key = 0;
            std::cin >> key;
            const int start = normalizedModulo(key, capacity);
            const int step = 1 + normalizedModulo(key, capacity - 1);
            int found = -1;
            int empty = -1;
            int probes = 0;
            for (int index = 0; index < capacity; ++index) {
                const int slot =
                    static_cast<int>((start + 1LL * index * step) % capacity);
                ++probes;
                if (!used[slot]) {
                    empty = slot;
                    break;
                }
                if (keys[slot] == key) {
                    found = slot;
                    break;
                }
            }
            if (command == "contains") {
                std::cout << (found >= 0 ? "true" : "false") << '\n';
            } else if (command == "slot") {
                if (found < 0) std::cout << "NOT_FOUND\n";
                else std::cout << found << '\n';
            } else if (command == "probe_count") {
                std::cout << probes << '\n';
            } else if (found >= 0) {
                std::cout << "DUPLICATE\n";
            } else if (empty < 0) {
                std::cout << "FULL\n";
            } else {
                used[empty] = true;
                keys[empty] = key;
                ++size;
                std::cout << empty << '\n';
            }
        }
    }
    delete[] used;
    delete[] keys;
}
