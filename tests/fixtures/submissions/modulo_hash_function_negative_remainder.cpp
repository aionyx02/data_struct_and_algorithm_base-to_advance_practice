#include <iostream>
#include <string>

int main() {
    int buckets = 0;
    int operations = 0;
    std::cin >> buckets >> operations;
    for (int operation = 0; operation < operations; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "hash") {
            int key = 0;
            std::cin >> key;
            std::cout << key % buckets << '\n';
        } else if (command == "same_bucket") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            std::cout << (left % buckets == right % buckets ? "true" : "false")
                      << '\n';
        } else if (command == "sequence") {
            int start = 0;
            int count = 0;
            std::cin >> start >> count;
            if (count == 0) std::cout << "EMPTY";
            for (int index = 0; index < count; ++index) {
                if (index != 0) std::cout << ' ';
                std::cout << (start + index) % buckets;
            }
            std::cout << '\n';
        } else {
            std::cout << buckets << '\n';
        }
    }
}
