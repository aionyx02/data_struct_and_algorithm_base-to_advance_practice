#include <iostream>
#include <string>

int normalizedModulo(int key, int modulus) {
    int result = key % modulus;
    return result < 0 ? result + modulus : result;
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
        else if (command == "load_percent") std::cout << size * 100 / capacity << '\n';
        else if (command == "table") {
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
            int found = -1;
            int slot = normalizedModulo(key, capacity);
            for (int index = 0; index < capacity; ++index) {
                if (!used[slot]) break;
                if (keys[slot] == key) {
                    found = slot;
                    break;
                }
                slot = (slot + 1) % capacity;
            }
            if (command == "contains") std::cout << (found >= 0 ? "true\n" : "false\n");
            else if (command == "slot") {
                if (found < 0) std::cout << "NOT_FOUND\n";
                else std::cout << found << '\n';
            } else if (found >= 0) std::cout << "DUPLICATE\n";
            else {
                if ((size + 1) * 2 > capacity) {
                    const int oldCapacity = capacity;
                    int* oldKeys = keys;
                    bool* oldUsed = used;
                    capacity = capacity * 2 + 1;
                    keys = new int[capacity]{};
                    used = new bool[capacity]{};
                    for (int oldSlot = 0; oldSlot < oldCapacity; ++oldSlot) {
                        if (oldUsed[oldSlot]) {
                            keys[oldSlot] = oldKeys[oldSlot];
                            used[oldSlot] = true;
                        }
                    }
                    delete[] oldUsed;
                    delete[] oldKeys;
                    slot = normalizedModulo(key, capacity);
                    while (used[slot]) slot = (slot + 1) % capacity;
                }
                keys[slot] = key;
                used[slot] = true;
                ++size;
                std::cout << slot << '\n';
            }
        }
    }
    delete[] used;
    delete[] keys;
}
