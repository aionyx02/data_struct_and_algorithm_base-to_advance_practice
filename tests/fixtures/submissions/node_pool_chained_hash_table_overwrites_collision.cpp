#include <iostream>
#include <string>

int normalizedModulo(int key, int modulus) {
    int result = key % modulus;
    return result < 0 ? result + modulus : result;
}

int main() {
    int bucketCount = 0;
    int capacity = 0;
    int operationCount = 0;
    std::cin >> bucketCount >> capacity >> operationCount;
    int* keys = new int[bucketCount]{};
    bool* used = new bool[bucketCount]{};
    int size = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "size") std::cout << size << '\n';
        else if (command == "free_slots") std::cout << capacity - size << '\n';
        else if (command == "clear") {
            for (int bucket = 0; bucket < bucketCount; ++bucket) used[bucket] = false;
            size = 0;
        } else if (command == "bucket" || command == "bucket_slots" ||
                   command == "bucket_size") {
            int bucket = -1;
            std::cin >> bucket;
            if (bucket < 0 || bucket >= bucketCount) std::cout << "OUT_OF_RANGE\n";
            else if (command == "bucket_size") std::cout << (used[bucket] ? 1 : 0) << '\n';
            else if (!used[bucket]) std::cout << "EMPTY\n";
            else if (command == "bucket") std::cout << keys[bucket] << '\n';
            else std::cout << bucket << '\n';
        } else {
            int key = 0;
            std::cin >> key;
            const int bucket = normalizedModulo(key, bucketCount);
            const bool found = used[bucket] && keys[bucket] == key;
            if (command == "contains") std::cout << (found ? "true\n" : "false\n");
            else if (command == "slot") {
                if (found) std::cout << bucket << '\n';
                else std::cout << "NOT_FOUND\n";
            } else if (command == "erase") {
                if (!found) std::cout << "NOT_FOUND\n";
                else {
                    used[bucket] = false;
                    --size;
                    std::cout << bucket << '\n';
                }
            } else if (found) std::cout << "DUPLICATE\n";
            else if (size == capacity) std::cout << "FULL\n";
            else {
                if (!used[bucket]) ++size;
                keys[bucket] = key;
                used[bucket] = true;
                std::cout << bucket << '\n';
            }
        }
    }
    delete[] used;
    delete[] keys;
}
