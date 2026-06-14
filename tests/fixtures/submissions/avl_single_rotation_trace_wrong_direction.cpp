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
        std::cout << child << " | " << child << ' ' << grandchild << ' '
                  << root << " | 2 1 1\n";
    }
}
