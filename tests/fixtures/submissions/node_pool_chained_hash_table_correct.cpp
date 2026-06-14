#include <iostream>
#include <string>

struct Node {
    int key;
    int next;
    bool active;
};

int normalizedModulo(int key, int modulus) {
    int result = key % modulus;
    return result < 0 ? result + modulus : result;
}

int main() {
    int bucketCount = 0;
    int capacity = 0;
    int operationCount = 0;
    std::cin >> bucketCount >> capacity >> operationCount;
    int* heads = new int[bucketCount];
    Node* nodes = new Node[capacity]{};
    for (int bucket = 0; bucket < bucketCount; ++bucket) heads[bucket] = -1;
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "free_slots") {
            std::cout << capacity - size << '\n';
        } else if (command == "clear") {
            for (int bucket = 0; bucket < bucketCount; ++bucket) heads[bucket] = -1;
            for (int slot = 0; slot < capacity; ++slot) nodes[slot].active = false;
            size = 0;
        } else if (command == "bucket" || command == "bucket_slots" ||
                   command == "bucket_size") {
            int bucket = -1;
            std::cin >> bucket;
            if (bucket < 0 || bucket >= bucketCount) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "bucket_size") {
                int count = 0;
                for (int slot = heads[bucket]; slot >= 0; slot = nodes[slot].next) {
                    ++count;
                }
                std::cout << count << '\n';
            } else {
                bool first = true;
                for (int slot = heads[bucket]; slot >= 0; slot = nodes[slot].next) {
                    if (!first) std::cout << ' ';
                    std::cout << (command == "bucket" ? nodes[slot].key : slot);
                    first = false;
                }
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        } else {
            int key = 0;
            std::cin >> key;
            const int bucket = normalizedModulo(key, bucketCount);
            int previous = -1;
            int current = heads[bucket];
            while (current >= 0 && nodes[current].key < key) {
                previous = current;
                current = nodes[current].next;
            }
            const bool found = current >= 0 && nodes[current].key == key;
            if (command == "contains") {
                std::cout << (found ? "true" : "false") << '\n';
            } else if (command == "slot") {
                if (!found) std::cout << "NOT_FOUND\n";
                else std::cout << current << '\n';
            } else if (command == "erase") {
                if (!found) {
                    std::cout << "NOT_FOUND\n";
                } else {
                    if (previous < 0) heads[bucket] = nodes[current].next;
                    else nodes[previous].next = nodes[current].next;
                    nodes[current].active = false;
                    --size;
                    std::cout << current << '\n';
                }
            } else if (found) {
                std::cout << "DUPLICATE\n";
            } else if (size == capacity) {
                std::cout << "FULL\n";
            } else {
                int slot = 0;
                while (nodes[slot].active) ++slot;
                nodes[slot] = {key, current, true};
                if (previous < 0) heads[bucket] = slot;
                else nodes[previous].next = slot;
                ++size;
                std::cout << slot << '\n';
            }
        }
    }
    delete[] nodes;
    delete[] heads;
}
