#include <iostream>
#include <memory>
#include <string>

int main() {
    int d1 = 0;
    int d2 = 0;
    int d3 = 0;
    int operationCount = 0;
    std::cin >> d1 >> d2 >> d3 >> operationCount;
    const int total = d1 * d2 * d3;
    auto values = std::make_unique<int[]>(total);
    auto valid = [&](int i, int j, int k) {
        return i >= 0 && i < d1 && j >= 0 && j < d2 && k >= 0 && k < d3;
    };
    auto correctIndex = [&](int i, int j, int k) {
        return (i * d2 + j) * d3 + k;
    };
    auto wrongIndex = [&](int i, int j, int k) {
        return (i * d3 + j) * d2 + k;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int i = 0;
            int j = 0;
            int k = 0;
            int value = 0;
            std::cin >> i >> j >> k >> value;
            if (!valid(i, j, k)) std::cout << "OUT_OF_RANGE\n";
            else values[correctIndex(i, j, k)] = value;
        } else if (command == "get" || command == "index") {
            int i = 0;
            int j = 0;
            int k = 0;
            std::cin >> i >> j >> k;
            if (!valid(i, j, k)) std::cout << "OUT_OF_RANGE\n";
            else if (command == "get") std::cout << values[correctIndex(i, j, k)] << '\n';
            else std::cout << wrongIndex(i, j, k) << '\n';
        } else if (command == "coord") {
            int linear = 0;
            std::cin >> linear;
            if (linear < 0 || linear >= total) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int i = linear / (d3 * d2);
                const int remainder = linear % (d3 * d2);
                std::cout << i << ' ' << remainder / d2 << ' '
                          << remainder % d2 << '\n';
            }
        } else if (command == "fill") {
            int value = 0;
            std::cin >> value;
            for (int linear = 0; linear < total; ++linear) values[linear] = value;
        } else if (command == "dims") {
            std::cout << d1 << ' ' << d2 << ' ' << d3 << '\n';
        }
    }
}
