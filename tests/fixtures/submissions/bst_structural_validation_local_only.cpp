#include <iostream>
#include <string>

struct Node {
    int id;
    int key;
    int parentId;
    int leftId;
    int rightId;
    Node* parent;
    Node* left;
    Node* right;
};

bool validateNode(Node* node, Node* expectedParent, int* state, int& visited) {
    if (node == nullptr) return true;
    if (state[node->id] != 0 || node->parent != expectedParent) return false;
    if (node->left != nullptr && node->left->key >= node->key) return false;
    if (node->right != nullptr && node->right->key <= node->key) return false;
    state[node->id] = 1;
    ++visited;
    if (!validateNode(node->left, node, state, visited) ||
        !validateNode(node->right, node, state, visited)) {
        return false;
    }
    state[node->id] = 2;
    return true;
}

void printInorder(Node* node, bool& first) {
    if (node == nullptr) return;
    printInorder(node->left, first);
    if (!first) std::cout << ' ';
    std::cout << node->id;
    first = false;
    printInorder(node->right, first);
}

int main() {
    int nodeCount = 0;
    int rootId = -1;
    int operationCount = 0;
    std::cin >> nodeCount >> rootId >> operationCount;
    Node* nodes = nodeCount == 0 ? nullptr : new Node[nodeCount];
    for (int row = 0; row < nodeCount; ++row) {
        int id = 0;
        std::cin >> id;
        nodes[id].id = id;
        std::cin >> nodes[id].key >> nodes[id].parentId
                 >> nodes[id].leftId >> nodes[id].rightId;
    }
    for (int id = 0; id < nodeCount; ++id) {
        nodes[id].parent = nodes[id].parentId < 0
            ? nullptr
            : &nodes[nodes[id].parentId];
        nodes[id].left = nodes[id].leftId < 0
            ? nullptr
            : &nodes[nodes[id].leftId];
        nodes[id].right = nodes[id].rightId < 0
            ? nullptr
            : &nodes[nodes[id].rightId];
    }
    Node* root = rootId < 0 ? nullptr : &nodes[rootId];
    int* state = nodeCount == 0 ? nullptr : new int[nodeCount]{};
    int visited = 0;
    bool valid = nodeCount == 0
        ? rootId == -1
        : root != nullptr && root->parent == nullptr &&
          validateNode(root, nullptr, state, visited) && visited == nodeCount;

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "valid") {
            std::cout << (valid ? "true" : "false") << '\n';
        } else if (command == "root") {
            if (root == nullptr) std::cout << "EMPTY\n";
            else std::cout << root->id << '\n';
        } else if (command == "inorder") {
            if (!valid) std::cout << "INVALID\n";
            else if (root == nullptr) std::cout << "EMPTY\n";
            else {
                bool first = true;
                printInorder(root, first);
                std::cout << '\n';
            }
        } else if (command == "size") {
            std::cout << nodeCount << '\n';
        } else {
            int id = -1;
            std::cin >> id;
            if (id < 0 || id >= nodeCount) {
                std::cout << "NOT_FOUND\n";
                continue;
            }
            Node& node = nodes[id];
            if (command == "key") std::cout << node.key << '\n';
            else {
                Node* link = command == "parent"
                    ? node.parent
                    : (command == "left" ? node.left : node.right);
                if (link == nullptr) std::cout << "NONE\n";
                else std::cout << link->id << '\n';
            }
        }
    }
    delete[] state;
    delete[] nodes;
}
