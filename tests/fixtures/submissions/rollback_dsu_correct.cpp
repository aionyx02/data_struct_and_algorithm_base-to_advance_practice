#include <iostream>
#include <string>

// Rollback disjoint set. Union is by size and deliberately does NOT compress
// paths, because compression would rewrite pointers that the undo log does not
// track. Every union pushes an undo record; checkpoint marks a position in that
// log and rollback restores the structure to the most recent checkpoint.

int* parent = nullptr;
int* sizes = nullptr;

int find(int value) {
    while (parent[value] != value) value = parent[value];
    return value;
}

bool valid(int value, int count) {
    return value >= 0 && value < count;
}

int main() {
    int count = 0;
    int operationCount = 0;
    std::cin >> count >> operationCount;
    parent = count == 0 ? nullptr : new int[count];
    sizes = count == 0 ? nullptr : new int[count];
    for (int index = 0; index < count; ++index) {
        parent[index] = index;
        sizes[index] = 1;
    }
    const int capacity = operationCount > 0 ? operationCount : 1;
    int* undoChild = new int[capacity];
    int* undoGrew = new int[capacity];
    int* undoOldSize = new int[capacity];
    int undoCount = 0;
    int* checkpoints = new int[capacity];
    int checkpointCount = 0;
    int components = count;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "components") {
            std::cout << components << '\n';
        } else if (command == "snapshot") {
            if (count == 0) std::cout << "EMPTY";
            for (int index = 0; index < count; ++index) {
                if (index != 0) std::cout << ' ';
                std::cout << parent[index];
            }
            std::cout << '\n';
        } else if (command == "checkpoint") {
            checkpoints[checkpointCount++] = undoCount;
            std::cout << checkpointCount << '\n';
        } else if (command == "rollback") {
            if (checkpointCount == 0) {
                std::cout << "NONE\n";
            } else {
                const int target = checkpoints[--checkpointCount];
                int undone = 0;
                while (undoCount > target) {
                    --undoCount;
                    parent[undoChild[undoCount]] = undoChild[undoCount];
                    sizes[undoGrew[undoCount]] = undoOldSize[undoCount];
                    ++components;
                    ++undone;
                }
                std::cout << undone << '\n';
            }
        } else if (command == "find" || command == "component_size") {
            int value = -1;
            std::cin >> value;
            if (!valid(value, count)) std::cout << "OUT_OF_RANGE\n";
            else {
                const int root = find(value);
                if (command == "find") std::cout << root << '\n';
                else std::cout << sizes[root] << '\n';
            }
        } else {  // unite or connected
            int left = -1;
            int right = -1;
            std::cin >> left >> right;
            if (!valid(left, count) || !valid(right, count)) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int leftRoot = find(left);
            int rightRoot = find(right);
            if (command == "connected") {
                std::cout << (leftRoot == rightRoot ? "true" : "false") << '\n';
            } else if (leftRoot == rightRoot) {
                std::cout << "ALREADY\n";
            } else {
                if (sizes[leftRoot] < sizes[rightRoot]) {
                    const int temporary = leftRoot;
                    leftRoot = rightRoot;
                    rightRoot = temporary;
                }
                undoChild[undoCount] = rightRoot;
                undoGrew[undoCount] = leftRoot;
                undoOldSize[undoCount] = sizes[leftRoot];
                ++undoCount;
                parent[rightRoot] = leftRoot;
                sizes[leftRoot] += sizes[rightRoot];
                --components;
                std::cout << leftRoot << '\n';
            }
        }
    }
    delete[] checkpoints;
    delete[] undoOldSize;
    delete[] undoGrew;
    delete[] undoChild;
    delete[] sizes;
    delete[] parent;
}
