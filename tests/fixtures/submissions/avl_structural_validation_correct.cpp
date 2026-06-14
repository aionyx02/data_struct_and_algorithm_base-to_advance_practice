#include <algorithm>
#include <climits>
#include <iostream>

struct Node {
    int key = 0;
    int left = -1;
    int right = -1;
    int parent = -1;
    int height = 0;
};

bool visit(
    int id,
    const Node* nodes,
    unsigned char* state,
    bool& cycle
) {
    if (id < 0) return true;
    if (state[id] == 1) {
        cycle = true;
        return false;
    }
    if (state[id] == 2) return true;
    state[id] = 1;
    visit(nodes[id].left, nodes, state, cycle);
    visit(nodes[id].right, nodes, state, cycle);
    state[id] = 2;
    return !cycle;
}

bool validOrder(const Node* nodes, int id, long long low, long long high) {
    if (id < 0) return true;
    if (nodes[id].key <= low || nodes[id].key >= high) return false;
    return validOrder(nodes, nodes[id].left, low, nodes[id].key) &&
           validOrder(nodes, nodes[id].right, nodes[id].key, high);
}

int computedHeight(const Node* nodes, int id) {
    if (id < 0) return 0;
    return 1 + std::max(
        computedHeight(nodes, nodes[id].left),
        computedHeight(nodes, nodes[id].right)
    );
}

bool validHeights(const Node* nodes, int id) {
    if (id < 0) return true;
    return nodes[id].height == computedHeight(nodes, id) &&
           validHeights(nodes, nodes[id].left) &&
           validHeights(nodes, nodes[id].right);
}

bool balanced(const Node* nodes, int id) {
    if (id < 0) return true;
    const int difference =
        computedHeight(nodes, nodes[id].left) -
        computedHeight(nodes, nodes[id].right);
    return difference >= -1 && difference <= 1 &&
           balanced(nodes, nodes[id].left) &&
           balanced(nodes, nodes[id].right);
}

int main() {
    int count = 0;
    int root = -1;
    std::cin >> count >> root;
    Node* nodes = count == 0 ? nullptr : new Node[count];
    bool invalidLink = root < -1 || root >= count ||
                       (count == 0 ? root != -1 : root < 0);
    for (int index = 0; index < count; ++index) {
        int id = -1;
        std::cin >> id;
        Node value;
        std::cin >> value.key >> value.left >> value.right
                 >> value.parent >> value.height;
        if (id < 0 || id >= count) invalidLink = true;
        else nodes[id] = value;
        const int links[3] = {value.left, value.right, value.parent};
        for (int link : links) {
            if (link < -1 || link >= count) invalidLink = true;
        }
    }
    if (!invalidLink && count > 0 && nodes[root].parent != -1) {
        invalidLink = true;
    }
    if (invalidLink) {
        std::cout << "INVALID_LINK\n";
    } else if (count == 0) {
        std::cout << "VALID\n";
    } else {
        unsigned char* state = new unsigned char[count]{};
        bool cycle = false;
        visit(root, nodes, state, cycle);
        if (cycle) {
            std::cout << "CYCLE\n";
        } else {
            bool disconnected = false;
            for (int id = 0; id < count; ++id) {
                disconnected = disconnected || state[id] == 0;
            }
            if (disconnected) {
                std::cout << "DISCONNECTED\n";
            } else {
                bool parentOk = true;
                for (int id = 0; id < count; ++id) {
                    if (nodes[id].left >= 0) {
                        parentOk = parentOk &&
                            nodes[nodes[id].left].parent == id;
                    }
                    if (nodes[id].right >= 0) {
                        parentOk = parentOk &&
                            nodes[nodes[id].right].parent == id;
                    }
                }
                if (!parentOk) std::cout << "INVALID_PARENT\n";
                else if (!validOrder(nodes, root, LLONG_MIN, LLONG_MAX)) {
                    std::cout << "INVALID_ORDER\n";
                } else if (!validHeights(nodes, root)) {
                    std::cout << "INVALID_HEIGHT\n";
                } else if (!balanced(nodes, root)) {
                    std::cout << "UNBALANCED\n";
                } else {
                    std::cout << "VALID\n";
                }
            }
        }
        delete[] state;
    }
    delete[] nodes;
}
