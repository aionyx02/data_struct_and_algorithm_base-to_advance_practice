#include <iostream>

struct Node {
    int key;
    int left;
    int right;
    int parent;
    int height;
};

int main() {
    int count = 0;
    int root = -1;
    std::cin >> count >> root;
    Node* nodes = count == 0 ? nullptr : new Node[count];
    for (int index = 0; index < count; ++index) {
        int id = 0;
        std::cin >> id >> nodes[id].key >> nodes[id].left >> nodes[id].right
                 >> nodes[id].parent >> nodes[id].height;
    }
    bool valid = true;
    for (int id = 0; id < count; ++id) {
        if (nodes[id].left >= 0) {
            valid = valid && nodes[nodes[id].left].key < nodes[id].key;
        }
        if (nodes[id].right >= 0) {
            valid = valid && nodes[nodes[id].right].key > nodes[id].key;
        }
    }
    std::cout << (valid ? "VALID" : "INVALID_ORDER") << '\n';
    delete[] nodes;
}
