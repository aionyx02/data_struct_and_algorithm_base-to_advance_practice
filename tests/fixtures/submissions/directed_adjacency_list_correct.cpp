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
    int** outgoing = count == 0 ? nullptr : new int*[count];
    int* outDegree = count == 0 ? nullptr : new int[count]{};
    int* inDegree = count == 0 ? nullptr : new int[count]{};
    for (int vertex = 0; vertex < count; ++vertex) outgoing[vertex] = new int[count];
    int edges = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "edge_count") std::cout << edges << '\n';
        else if (command == "capacity") std::cout << capacity << '\n';
        else if (command == "clear") {
            for (int vertex = 0; vertex < count; ++vertex) {
                outDegree[vertex] = 0;
                inDegree[vertex] = 0;
            }
            edges = 0;
        } else if (command == "out_degree" || command == "in_degree" ||
                   command == "out_neighbors" || command == "in_neighbors") {
            int vertex = -1;
            std::cin >> vertex;
            if (!valid(vertex, count)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "out_degree") {
                std::cout << outDegree[vertex] << '\n';
            } else if (command == "in_degree") {
                std::cout << inDegree[vertex] << '\n';
            } else if (command == "out_neighbors") {
                if (outDegree[vertex] == 0) std::cout << "EMPTY";
                for (int index = 0; index < outDegree[vertex]; ++index) {
                    if (index != 0) std::cout << ' ';
                    std::cout << outgoing[vertex][index];
                }
                std::cout << '\n';
            } else {
                bool first = true;
                for (int source = 0; source < count; ++source) {
                    if (contains(outgoing[source], outDegree[source], vertex)) {
                        if (!first) std::cout << ' ';
                        std::cout << source;
                        first = false;
                    }
                }
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        } else {
            int from = -1;
            int to = -1;
            std::cin >> from >> to;
            if (!valid(from, count) || !valid(to, count)) std::cout << "OUT_OF_RANGE\n";
            else if ((command == "add" || command == "remove") && from == to) {
                std::cout << "SELF_LOOP\n";
            } else {
                const bool exists = contains(outgoing[from], outDegree[from], to);
                if (command == "has") std::cout << (exists ? "true" : "false") << '\n';
                else if (command == "add") {
                    if (exists) std::cout << "DUPLICATE\n";
                    else if (edges == capacity) std::cout << "FULL\n";
                    else {
                        insertSorted(outgoing[from], outDegree[from], to);
                        ++inDegree[to];
                        ++edges;
                    }
                } else if (!exists) std::cout << "NOT_FOUND\n";
                else {
                    eraseValue(outgoing[from], outDegree[from], to);
                    --inDegree[to];
                    --edges;
                    std::cout << "REMOVED\n";
                }
            }
        }
    }
    for (int vertex = 0; vertex < count; ++vertex) delete[] outgoing[vertex];
    delete[] inDegree;
    delete[] outDegree;
    delete[] outgoing;
}
