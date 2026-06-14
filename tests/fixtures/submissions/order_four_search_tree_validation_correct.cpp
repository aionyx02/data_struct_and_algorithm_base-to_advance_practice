#include <climits>
#include <iostream>

struct Node {
    int count = 0;
    int keys[3] = {-1, -1, -1};
    int children[4] = {-1, -1, -1, -1};
};

bool walk(
    int id,
    const Node* nodes,
    unsigned char* state,
    long long low,
    long long high,
    bool& cycle,
    bool& occupancy,
    bool& keysValid,
    bool& rangeValid
) {
    if (state[id] == 1) {
        cycle = true;
        return false;
    }
    if (state[id] == 2) {
        cycle = true;
        return false;
    }
    state[id] = 1;
    const Node& node = nodes[id];
    if (node.count < 1 || node.count > 3) occupancy = false;
    for (int index = 0; index < node.count; ++index) {
        if (index > 0 && node.keys[index - 1] >= node.keys[index]) {
            keysValid = false;
        }
        if (node.keys[index] <= low || node.keys[index] >= high) {
            rangeValid = false;
        }
    }
    bool hasChild = false;
    int childCount = 0;
    for (int index = 0; index < 4; ++index) {
        if (node.children[index] >= 0) {
            hasChild = true;
            ++childCount;
        }
    }
    if (hasChild && childCount != node.count + 1) occupancy = false;
    if (hasChild) {
        for (int index = 0; index <= node.count; ++index) {
            if (node.children[index] < 0) {
                occupancy = false;
                continue;
            }
            const long long childLow =
                index == 0 ? low : node.keys[index - 1];
            const long long childHigh =
                index == node.count ? high : node.keys[index];
            walk(
                node.children[index],
                nodes,
                state,
                childLow,
                childHigh,
                cycle,
                occupancy,
                keysValid,
                rangeValid
            );
        }
    }
    state[id] = 2;
    return true;
}

int main() {
    int count = 0;
    int root = -1;
    std::cin >> count >> root;
    Node* nodes = count == 0 ? nullptr : new Node[count];
    bool invalidLink = root < 0 || root >= count;
    for (int index = 0; index < count; ++index) {
        int id = -1;
        Node node;
        std::cin >> id >> node.count;
        for (int& key : node.keys) std::cin >> key;
        for (int& child : node.children) {
            std::cin >> child;
            if (child < -1 || child >= count) invalidLink = true;
        }
        if (id < 0 || id >= count) invalidLink = true;
        else nodes[id] = node;
    }
    if (invalidLink) {
        std::cout << "INVALID_LINK\n";
    } else {
        unsigned char* state = new unsigned char[count]{};
        bool cycle = false;
        bool occupancy = true;
        bool keysValid = true;
        bool rangeValid = true;
        walk(
            root,
            nodes,
            state,
            LLONG_MIN,
            LLONG_MAX,
            cycle,
            occupancy,
            keysValid,
            rangeValid
        );
        bool disconnected = false;
        for (int id = 0; id < count; ++id) disconnected |= state[id] == 0;
        if (cycle) std::cout << "CYCLE\n";
        else if (disconnected) std::cout << "DISCONNECTED\n";
        else if (!occupancy) std::cout << "INVALID_OCCUPANCY\n";
        else if (!keysValid) std::cout << "INVALID_KEYS\n";
        else if (!rangeValid) std::cout << "INVALID_RANGE\n";
        else std::cout << "VALID\n";
        delete[] state;
    }
    delete[] nodes;
}
