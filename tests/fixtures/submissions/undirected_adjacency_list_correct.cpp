#include <iostream>
#include <string>

bool valid(int vertex, int count) {
    return vertex >= 0 && vertex < count;
}

bool contains(const int* values, int size, int value) {
    for (int index = 0; index < size; ++index) {
        if (values[index] == value) return true;
    }
    return false;
}

void insertSorted(int* values, int& size, int value) {
    int index = size;
    while (index > 0 && values[index - 1] > value) {
        values[index] = values[index - 1];
        --index;
    }
    values[index] = value;
    ++size;
}

void eraseValue(int* values, int& size, int value) {
    int index = 0;
    while (values[index] != value) ++index;
    for (; index + 1 < size; ++index) values[index] = values[index + 1];
    --size;
}

int main() {
    int count = 0;
    int capacity = 0;
    int operationCount = 0;
    std::cin >> count >> capacity >> operationCount;
    int** adjacency = count == 0 ? nullptr : new int*[count];
    int* degrees = count == 0 ? nullptr : new int[count]{};
    for (int vertex = 0; vertex < count; ++vertex) {
        adjacency[vertex] = new int[count];
    }
    int edges = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "edge_count") std::cout << edges << '\n';
        else if (command == "capacity") std::cout << capacity << '\n';
        else if (command == "clear") {
            for (int vertex = 0; vertex < count; ++vertex) degrees[vertex] = 0;
            edges = 0;
        } else if (command == "degree" || command == "neighbors") {
            int vertex = -1;
            std::cin >> vertex;
            if (!valid(vertex, count)) std::cout << "OUT_OF_RANGE\n";
            else if (command == "degree") std::cout << degrees[vertex] << '\n';
            else if (degrees[vertex] == 0) std::cout << "EMPTY\n";
            else {
                for (int index = 0; index < degrees[vertex]; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << adjacency[vertex][index];
                }
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
            } else {
                const bool exists =
                    contains(adjacency[from], degrees[from], to);
                if (command == "has") {
                    std::cout << (exists ? "true" : "false") << '\n';
                } else if (command == "add") {
                    if (exists) std::cout << "DUPLICATE\n";
                    else if (edges == capacity) std::cout << "FULL\n";
                    else {
                        insertSorted(adjacency[from], degrees[from], to);
                        insertSorted(adjacency[to], degrees[to], from);
                        ++edges;
                    }
                } else if (!exists) {
                    std::cout << "NOT_FOUND\n";
                } else {
                    eraseValue(adjacency[from], degrees[from], to);
                    eraseValue(adjacency[to], degrees[to], from);
                    --edges;
                    std::cout << "REMOVED\n";
                }
            }
        }
    }
    for (int vertex = 0; vertex < count; ++vertex) delete[] adjacency[vertex];
    delete[] degrees;
    delete[] adjacency;
}
