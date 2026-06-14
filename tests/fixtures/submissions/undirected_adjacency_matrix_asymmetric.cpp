#include <iostream>
#include <string>

bool valid(int vertex, int count) {
    return vertex >= 0 && vertex < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    bool* matrix = count == 0 ? nullptr : new bool[count * count]{};
    int edges = 0;
    auto at = [&](int from, int to) -> bool& {
        return matrix[from * count + to];
    };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "edge_count") std::cout << edges << '\n';
        else if (command == "clear") {
            for (int index = 0; index < count * count; ++index) matrix[index] = false;
            edges = 0;
        } else if (command == "degree" || command == "neighbors" ||
                   command == "row") {
            int vertex = -1;
            std::cin >> vertex;
            if (!valid(vertex, count)) std::cout << "OUT_OF_RANGE\n";
            else if (command == "degree") {
                int degree = 0;
                for (int neighbor = 0; neighbor < count; ++neighbor) {
                    if (at(vertex, neighbor)) ++degree;
                }
                std::cout << degree << '\n';
            } else if (command == "neighbors") {
                bool first = true;
                for (int neighbor = 0; neighbor < count; ++neighbor) {
                    if (at(vertex, neighbor)) {
                        if (!first) std::cout << ' ';
                        std::cout << neighbor;
                        first = false;
                    }
                }
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            } else {
                for (int neighbor = 0; neighbor < count; ++neighbor) {
                    if (neighbor != 0) std::cout << ' ';
                    std::cout << (at(vertex, neighbor) ? 1 : 0);
                }
                if (count == 0) std::cout << "EMPTY";
                std::cout << '\n';
            }
        } else {
            int from = -1;
            int to = -1;
            std::cin >> from >> to;
            if (!valid(from, count) || !valid(to, count)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if ((command == "add" || command == "remove") && from == to) {
                std::cout << "SELF_LOOP\n";
            } else if (command == "has") {
                std::cout << (at(from, to) ? "true" : "false") << '\n';
            } else if (command == "add") {
                if (at(from, to)) std::cout << "DUPLICATE\n";
                else {
                    at(from, to) = true;
                    ++edges;
                }
            } else if (!at(from, to)) {
                std::cout << "NOT_FOUND\n";
            } else {
                at(from, to) = false;
                --edges;
                std::cout << "REMOVED\n";
            }
        }
    }
    delete[] matrix;
}
