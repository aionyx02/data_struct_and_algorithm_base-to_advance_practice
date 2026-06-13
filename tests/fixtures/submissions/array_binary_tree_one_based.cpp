#include <iostream>
#include <memory>
#include <string>

struct Slot {
    int value = 0;
    bool occupied = false;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto slots = std::make_unique<Slot[]>(capacity);
    int count = 0;
    auto valid = [&](int index) { return index >= 0 && index < capacity; };
    auto relative = [&](int index, int target) {
        if (!valid(index)) std::cout << "OUT_OF_RANGE\n";
        else if (!slots[index].occupied) std::cout << "EMPTY\n";
        else if (!valid(target) || !slots[target].occupied) std::cout << "NONE\n";
        else std::cout << target << '\n';
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int index = 0;
            int value = 0;
            std::cin >> index >> value;
            if (!valid(index)) std::cout << "OUT_OF_RANGE\n";
            else if (!slots[index].occupied && index != 0 &&
                     !slots[index / 2].occupied) std::cout << "NO_PARENT\n";
            else {
                if (!slots[index].occupied) ++count;
                slots[index] = {value, true};
            }
        } else if (command == "erase") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) std::cout << "OUT_OF_RANGE\n";
            else if (!slots[index].occupied) std::cout << "EMPTY\n";
            else {
                const int left = 2 * index;
                const int right = left + 1;
                if ((left != index && valid(left) && slots[left].occupied) ||
                    (valid(right) && slots[right].occupied)) {
                    std::cout << "HAS_CHILDREN\n";
                } else {
                    std::cout << slots[index].value << '\n';
                    slots[index].occupied = false;
                    --count;
                }
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) std::cout << "OUT_OF_RANGE\n";
            else if (!slots[index].occupied) std::cout << "EMPTY\n";
            else std::cout << slots[index].value << '\n';
        } else if (command == "parent_index") {
            int index = 0;
            std::cin >> index;
            relative(index, index == 0 ? -1 : index / 2);
        } else if (command == "left_index") {
            int index = 0;
            std::cin >> index;
            relative(index, 2 * index);
        } else if (command == "right_index") {
            int index = 0;
            std::cin >> index;
            relative(index, 2 * index + 1);
        } else if (command == "occupied") {
            std::cout << count << '\n';
        } else if (command == "clear") {
            for (int index = 0; index < capacity; ++index) slots[index].occupied = false;
            count = 0;
        }
    }
}
