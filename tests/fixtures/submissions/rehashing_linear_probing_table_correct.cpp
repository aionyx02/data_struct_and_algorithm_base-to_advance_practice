#include <iostream>
#include <string>

int normalizedModulo(int key, int modulus) {
    int result = key % modulus;
    return result < 0 ? result + modulus : result;
}

int findKey(const int* keys, const bool* used, int capacity, int key) {
    const int start = normalizedModulo(key, capacity);
    for (int index = 0; index < capacity; ++index) {
        const int slot = (start + index) % capacity;
        if (!used[slot]) return -1;
        if (keys[slot] == key) return slot;
    }
    return -1;
}

int insertRaw(int* keys, bool* used, int capacity, int key) {
    int slot = normalizedModulo(key, capacity);
    while (used[slot]) slot = (slot + 1) % capacity;
    used[slot] = true;
    keys[slot] = key;
    return slot;
}

int main() {
    int initialCapacity = 0;
    int operationCount = 0;
    std::cin >> initialCapacity >> operationCount;
    int capacity = initialCapacity;
    int* keys = new int[capacity]{};
    bool* used = new bool[capacity]{};
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "capacity") std::cout << capacity << '\n';
        else if (command == "size") std::cout << size << '\n';
        else if (command == "load_percent") {
            std::cout << size * 100 / capacity << '\n';
        } else if (command == "table") {
            for (int slot = 0; slot < capacity; ++slot) {
                if (slot) std::cout << ' ';
                if (used[slot]) std::cout << keys[slot];
                else std::cout << '.';
            }
            std::cout << '\n';
        } else if (command == "clear") {
            delete[] used;
            delete[] keys;
            capacity = initialCapacity;
            keys = new int[capacity]{};
            used = new bool[capacity]{};
            size = 0;
        } else {
            int key = 0;
            std::cin >> key;
            int slot = findKey(keys, used, capacity, key);
            if (command == "contains") {
                std::cout << (slot >= 0 ? "true" : "false") << '\n';
            } else if (command == "slot") {
                if (slot < 0) std::cout << "NOT_FOUND\n";
                else std::cout << slot << '\n';
            } else if (slot >= 0) {
                std::cout << "DUPLICATE\n";
            } else {
                if ((size + 1) * 2 > capacity) {
                    const int oldCapacity = capacity;
                    int* oldKeys = keys;
                    bool* oldUsed = used;
                    capacity = capacity * 2 + 1;
                    keys = new int[capacity]{};
                    used = new bool[capacity]{};
                    for (int oldSlot = 0; oldSlot < oldCapacity; ++oldSlot) {
                        if (oldUsed[oldSlot]) {
                            insertRaw(keys, used, capacity, oldKeys[oldSlot]);
                        }
                    }
                    delete[] oldUsed;
                    delete[] oldKeys;
                }
                slot = insertRaw(keys, used, capacity, key);
                ++size;
                std::cout << slot << '\n';
            }
        }
    }
    delete[] used;
    delete[] keys;
}
