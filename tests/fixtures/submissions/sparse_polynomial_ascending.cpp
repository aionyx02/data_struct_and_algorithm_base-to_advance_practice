#include <iostream>
#include <memory>
#include <string>

struct Term {
    int exponent = 0;
    int coefficient = 0;
};

int main() {
    int capacity = 0;
    int operationCount = 0;
    std::cin >> capacity >> operationCount;
    auto terms = std::make_unique<Term[]>(capacity);
    int size = 0;

    auto find = [&](int exponent) {
        for (int index = 0; index < size; ++index) {
            if (terms[index].exponent == exponent) return index;
        }
        return -1;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int exponent = 0;
            int coefficient = 0;
            std::cin >> exponent >> coefficient;
            if (exponent < 0) {
                std::cout << "OUT_OF_RANGE\n";
                continue;
            }
            int index = find(exponent);
            if (index != -1 && coefficient == 0) {
                for (int next = index + 1; next < size; ++next) {
                    terms[next - 1] = terms[next];
                }
                --size;
            } else if (index != -1) {
                terms[index].coefficient = coefficient;
            } else if (coefficient != 0) {
                if (size == capacity) {
                    std::cout << "FULL\n";
                } else {
                    index = 0;
                    while (index < size && terms[index].exponent < exponent) ++index;
                    for (int next = size; next > index; --next) terms[next] = terms[next - 1];
                    terms[index] = {exponent, coefficient};
                    ++size;
                }
            }
        } else if (command == "coeff") {
            int exponent = 0;
            std::cin >> exponent;
            if (exponent < 0) std::cout << "OUT_OF_RANGE\n";
            else {
                const int index = find(exponent);
                std::cout << (index == -1 ? 0 : terms[index].coefficient) << '\n';
            }
        } else if (command == "degree") {
            std::cout << (size == 0 ? -1 : terms[size - 1].exponent) << '\n';
        } else if (command == "leading") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << terms[size - 1].exponent << ' '
                           << terms[size - 1].coefficient << '\n';
        } else if (command == "term") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= size) std::cout << "OUT_OF_RANGE\n";
            else std::cout << terms[index].exponent << ' '
                           << terms[index].coefficient << '\n';
        } else if (command == "terms") {
            std::cout << size << '\n';
        } else if (command == "clear") {
            size = 0;
        }
    }
}
