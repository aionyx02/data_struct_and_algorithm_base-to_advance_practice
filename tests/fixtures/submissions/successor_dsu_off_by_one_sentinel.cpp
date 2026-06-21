#include <iostream>
#include <string>

// Known-wrong: treats the last real slot (index count - 1) as if it were the
// sentinel, so it refuses to assign the final free slot and reports NONE one
// step too early.

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
        } else {
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
                if (slot >= count - 1) {  // bug: last slot treated as sentinel
                    std::cout << "NONE\n";
                } else if (command == "next") {
                    std::cout << slot << '\n';
                } else {
                    parent[slot] = slot + 1;
                    --remaining;
                    std::cout << slot << '\n';
                }
            }
        }
    }
    delete[] parent;
}
