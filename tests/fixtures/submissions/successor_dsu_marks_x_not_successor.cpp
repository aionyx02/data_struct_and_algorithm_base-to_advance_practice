#include <iostream>
#include <string>

// Known-wrong: assign marks slot x itself used and prints x without first
// walking to the next free slot, so assigning the same start repeatedly hands
// out the same slot and corrupts the chain.

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
            } else if (command == "next") {
                const int slot = find(value);
                std::cout << (slot == count ? "NONE" : std::to_string(slot))
                          << '\n';
            } else {  // assign: bug marks x itself
                parent[value] = value + 1;
                --remaining;
                std::cout << value << '\n';
            }
        }
    }
    delete[] parent;
}
