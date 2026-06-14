#include <iostream>
#include <string>

int normalizedModulo(int key, int modulus) {
    int result = key % modulus;
    return result < 0 ? result + modulus : result;
}

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    int* keys = new int[capacity]{};
    int* states = new int[capacity]{};
    int size = 0;
    int tombstones = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "table") {
            for (int slot = 0; slot < capacity; ++slot) {
                if (slot) std::cout << ' ';
                if (states[slot] == 0) std::cout << '.';
                else if (states[slot] == 1) std::cout << '#';
                else std::cout << keys[slot];
            }
            std::cout << '\n';
        } else if (command == "size") std::cout << size << '\n';
        else if (command == "tombstones") std::cout << tombstones << '\n';
        else if (command == "clear") {
            for (int slot = 0; slot < capacity; ++slot) states[slot] = 0;
            size = tombstones = 0;
        } else if (command == "state") {
            int slot = -1;
            std::cin >> slot;
            if (slot < 0 || slot >= capacity) std::cout << "OUT_OF_RANGE\n";
            else if (states[slot] == 0) std::cout << "EMPTY\n";
            else if (states[slot] == 1) std::cout << "TOMBSTONE\n";
            else std::cout << "OCCUPIED " << keys[slot] << '\n';
        } else {
            int key = 0;
            std::cin >> key;
            int found = -1;
            int empty = -1;
            for (int index = 0; index < capacity; ++index) {
                const int slot =
                    (normalizedModulo(key, capacity) + index) % capacity;
                if (states[slot] != 2) {
                    empty = slot;
                    break;
                }
                if (keys[slot] == key) {
                    found = slot;
                    break;
                }
            }
            if (command == "contains") std::cout << (found >= 0 ? "true\n" : "false\n");
            else if (command == "slot") {
                if (found < 0) std::cout << "NOT_FOUND\n";
                else std::cout << found << '\n';
            } else if (command == "erase") {
                if (found < 0) std::cout << "NOT_FOUND\n";
                else {
                    states[found] = 1;
                    --size;
                    ++tombstones;
                    std::cout << "REMOVED\n";
                }
            } else if (found >= 0) std::cout << "DUPLICATE\n";
            else if (empty < 0) std::cout << "FULL\n";
            else {
                if (states[empty] == 1) --tombstones;
                states[empty] = 2;
                keys[empty] = key;
                ++size;
                std::cout << empty << '\n';
            }
        }
    }
    delete[] states;
    delete[] keys;
}
