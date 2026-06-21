#include <iostream>
#include <string>

// Successor disjoint set ("next free slot"). parent[i] points toward the next
// free slot at or after i; index count is a sentinel meaning "no free slot".
// find compresses, so repeated successor queries stay fast. Assigning a slot
// links it to slot+1.

int* parent = nullptr;
int count = 0;

int find(int value) {
    if (parent[value] != value) parent[value] = find(parent[value]);
    return parent[value];
}

bool valid(int value) {
    return value >= 0 && value < count;
}

int main() {
    int operationCount = 0;
    std::cin >> count >> operationCount;
    parent = new int[count + 1];
    for (int index = 0; index <= count; ++index) parent[index] = index;
    int remaining = count;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "remaining") {
            std::cout << remaining << '\n';
        } else if (command == "snapshot") {
            if (count == 0) std::cout << "EMPTY";
            for (int index = 0; index < count; ++index) {
                if (index != 0) std::cout << ' ';
                std::cout << parent[index];
            }
            std::cout << '\n';
        } else {  // assign x, next x, used x
            int value = -1;
            std::cin >> value;
            if (!valid(value)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            if (command == "used") {
                std::cout << (find(value) != value ? 1 : 0) << '\n';
            } else {
                const int slot = find(value);
                if (slot == count) {
                    std::cout << "NONE\n";
                } else if (command == "next") {
                    std::cout << slot << '\n';
                } else {  // assign
                    parent[slot] = slot + 1;
                    --remaining;
                    std::cout << slot << '\n';
                }
            }
        }
    }
    delete[] parent;
}
