#include <iostream>
#include <string>

int main() {
    int nodeCount = 0;
    int operationCount = 0;
    std::cin >> nodeCount >> operationCount;
    int* parent = nodeCount == 0 ? nullptr : new int[nodeCount];
    for (int id = 0; id < nodeCount; ++id) std::cin >> parent[id];
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "roots") {
            bool first = true;
            for (int id = 0; id < nodeCount; ++id) {
                if (parent[id] == -1) {
                    if (!first) std::cout << ' ';
                    std::cout << id;
                    first = false;
                }
            }
            if (first) std::cout << "EMPTY";
            std::cout << '\n';
        } else if (command == "size") {
            std::cout << nodeCount << '\n';
        } else {
            int id = -1;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) std::cout << "OUT_OF_RANGE\n";
            else if (command == "parent") {
                if (parent[id] == -1) std::cout << "NONE\n";
                else std::cout << parent[id] << '\n';
            } else if (command == "root") {
                std::cout << (parent[id] == -1 ? id : parent[id]) << '\n';
            } else if (command == "depth") {
                std::cout << (parent[id] == -1 ? 0 : 1) << '\n';
            } else {
                bool first = true;
                for (int child = 0; child < nodeCount; ++child) {
                    if (parent[child] == id) {
                        if (!first) std::cout << ' ';
                        std::cout << child;
                        first = false;
                    }
                }
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        }
    }
    delete[] parent;
}
