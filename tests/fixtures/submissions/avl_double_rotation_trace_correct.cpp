#include <iostream>
#include <string>

int main() {
    int cases = 0;
    std::cin >> cases;
    while (cases-- > 0) {
        std::string type;
        int root = 0;
        int child = 0;
        int grandchild = 0;
        std::cin >> type >> root >> child >> grandchild;
        std::cout << (type == "LR" ? "LEFT_RIGHT" : "RIGHT_LEFT")
                  << " | " << grandchild << " | " << grandchild << ' ';
        if (type == "LR") std::cout << child << ' ' << root;
        else std::cout << root << ' ' << child;
        std::cout << " | 2 1 1\n";
    }
}
