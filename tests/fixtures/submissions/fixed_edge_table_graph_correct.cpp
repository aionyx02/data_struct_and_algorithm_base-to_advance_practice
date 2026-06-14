#include <iostream>
#include <string>

struct Edge {
    int id;
    int left;
    int right;
    bool active;
};

bool valid(int value, int count) {
    return value >= 0 && value < count;
}

int findId(const Edge* edges, int capacity, int id) {
    for (int slot = 0; slot < capacity; ++slot) {
        if (edges[slot].active && edges[slot].id == id) return slot;
    }
    return -1;
}

bool samePair(const Edge& edge, int left, int right) {
    return (edge.left == left && edge.right == right) ||
           (edge.left == right && edge.right == left);
}

int main() {
    int vertices = 0;
    int capacity = 0;
    int operationCount = 0;
    std::cin >> vertices >> capacity >> operationCount;
    Edge* edges = capacity == 0 ? nullptr : new Edge[capacity]{};
    int edgeCount = 0;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "edge_count") std::cout << edgeCount << '\n';
        else if (command == "free_slots") std::cout << capacity - edgeCount << '\n';
        else if (command == "clear") {
            for (int slot = 0; slot < capacity; ++slot) edges[slot].active = false;
            edgeCount = 0;
        } else if (command == "add") {
            int id = 0;
            int left = -1;
            int right = -1;
            std::cin >> id >> left >> right;
            if (!valid(left, vertices) || !valid(right, vertices)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (left == right) {
                std::cout << "SELF_LOOP\n";
            } else {
                bool duplicate = findId(edges, capacity, id) >= 0;
                for (int slot = 0; slot < capacity && !duplicate; ++slot) {
                    duplicate = edges[slot].active &&
                                samePair(edges[slot], left, right);
                }
                if (duplicate) std::cout << "DUPLICATE\n";
                else if (edgeCount == capacity) std::cout << "FULL\n";
                else {
                    int slot = 0;
                    while (edges[slot].active) ++slot;
                    edges[slot] = {id, left, right, true};
                    ++edgeCount;
                    std::cout << slot << '\n';
                }
            }
        } else if (command == "erase") {
            int id = 0;
            std::cin >> id;
            const int slot = findId(edges, capacity, id);
            if (slot < 0) std::cout << "NOT_FOUND\n";
            else {
                edges[slot].active = false;
                --edgeCount;
                std::cout << slot << '\n';
            }
        } else if (command == "edge_at") {
            int slot = -1;
            std::cin >> slot;
            if (!valid(slot, capacity)) std::cout << "OUT_OF_RANGE\n";
            else if (!edges[slot].active) std::cout << "EMPTY\n";
            else {
                std::cout << edges[slot].id << ' ' << edges[slot].left
                          << ' ' << edges[slot].right << '\n';
            }
        } else if (command == "slot" || command == "endpoints") {
            int id = 0;
            std::cin >> id;
            const int slot = findId(edges, capacity, id);
            if (slot < 0) std::cout << "NOT_FOUND\n";
            else if (command == "slot") std::cout << slot << '\n';
            else {
                std::cout << edges[slot].left << ' ' << edges[slot].right
                          << '\n';
            }
        } else {
            int vertex = -1;
            std::cin >> vertex;
            if (!valid(vertex, vertices)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "degree") {
                int degree = 0;
                for (int slot = 0; slot < capacity; ++slot) {
                    if (edges[slot].active &&
                        (edges[slot].left == vertex ||
                         edges[slot].right == vertex)) {
                        ++degree;
                    }
                }
                std::cout << degree << '\n';
            } else {
                bool first = true;
                for (int neighbor = 0; neighbor < vertices; ++neighbor) {
                    bool found = false;
                    for (int slot = 0; slot < capacity && !found; ++slot) {
                        found = edges[slot].active &&
                            ((edges[slot].left == vertex &&
                              edges[slot].right == neighbor) ||
                             (edges[slot].right == vertex &&
                              edges[slot].left == neighbor));
                    }
                    if (found) {
                        if (!first) std::cout << ' ';
                        std::cout << neighbor;
                        first = false;
                    }
                }
                if (first) std::cout << "EMPTY";
                std::cout << '\n';
            }
        }
    }
    delete[] edges;
}
