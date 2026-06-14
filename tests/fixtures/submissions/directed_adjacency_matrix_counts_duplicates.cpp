#include <iostream>
#include <string>

bool valid(int vertex, int count) {
    return vertex >= 0 && vertex < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    int* matrix = count == 0 ? nullptr : new int[count * count]{};
    int edges = 0;
    auto at = [&](int from, int to) -> int& {
        return matrix[from * count + to];
    };
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "edge_count") std::cout << edges << '\n';
        else if (command == "clear") {
            for (int index = 0; index < count * count; ++index) matrix[index] = 0;
            edges = 0;
        } else if (command == "out_degree" || command == "in_degree" ||
                   command == "out_neighbors" ||
                   command == "in_neighbors" || command == "row") {
            int vertex = -1;
            std::cin >> vertex;
            if (!valid(vertex, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            if (command == "row") {
                for (int neighbor = 0; neighbor < count; ++neighbor) {
                    if (neighbor != 0) std::cout << ' ';
                    std::cout << (at(vertex, neighbor) > 0 ? 1 : 0);
                }
                std::cout << '\n';
                continue;
            }
            const bool incoming =
                command == "in_degree" || command == "in_neighbors";
            const bool degree =
                command == "in_degree" || command == "out_degree";
            int total = 0;
            bool first = true;
            for (int neighbor = 0; neighbor < count; ++neighbor) {
                const int amount = incoming
                    ? at(neighbor, vertex)
                    : at(vertex, neighbor);
                total += amount;
                if (amount > 0 && !degree) {
                    if (!first) std::cout << ' ';
                    std::cout << neighbor;
                    first = false;
                }
            }
            if (degree) std::cout << total;
            else if (first) std::cout << "EMPTY";
            std::cout << '\n';
        } else {
            int from = -1;
            int to = -1;
            std::cin >> from >> to;
            if (!valid(from, count) || !valid(to, count)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if ((command == "add" || command == "remove") && from == to) {
                std::cout << "SELF_LOOP\n";
            } else if (command == "has") {
                std::cout << (at(from, to) > 0 ? "true" : "false") << '\n';
            } else if (command == "add") {
                ++at(from, to);
                ++edges;
            } else if (at(from, to) == 0) {
                std::cout << "NOT_FOUND\n";
            } else {
                --at(from, to);
                --edges;
                std::cout << "REMOVED\n";
            }
        }
    }
    delete[] matrix;
}
