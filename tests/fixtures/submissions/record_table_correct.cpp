#include <iostream>
#include <memory>
#include <string>

struct Record {
    int id = 0;
    int value = 0;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto records = std::make_unique<Record[]>(capacity);
    int size = 0;

    auto find = [&](int id) {
        for (int index = 0; index < size; ++index) {
            if (records[index].id == id) return index;
        }
        return -1;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int id = 0;
            int value = 0;
            std::cin >> id >> value;
            if (find(id) != -1) {
                std::cout << "DUPLICATE\n";
            } else if (size == capacity) {
                std::cout << "FULL\n";
            } else {
                records[size++] = {id, value};
            }
        } else if (command == "update") {
            int id = 0;
            int value = 0;
            std::cin >> id >> value;
            const int index = find(id);
            if (index == -1) std::cout << "NOT_FOUND\n";
            else records[index].value = value;
        } else if (command == "erase") {
            int id = 0;
            std::cin >> id;
            const int index = find(id);
            if (index == -1) {
                std::cout << "NOT_FOUND\n";
            } else {
                std::cout << records[index].value << '\n';
                for (int next = index + 1; next < size; ++next) {
                    records[next - 1] = records[next];
                }
                --size;
            }
        } else if (command == "get") {
            int id = 0;
            std::cin >> id;
            const int index = find(id);
            if (index == -1) std::cout << "NOT_FOUND\n";
            else std::cout << records[index].value << '\n';
        } else if (command == "contains") {
            int id = 0;
            std::cin >> id;
            std::cout << (find(id) == -1 ? "false" : "true") << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            size = 0;
        }
    }
}
