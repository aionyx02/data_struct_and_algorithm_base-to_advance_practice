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

    auto position = [&](int exponent) {
        int index = 0;
        while (index < size && terms[index].exponent > exponent) ++index;
        return index;
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
            const int index = position(exponent);
            const bool found = index < size && terms[index].exponent == exponent;
            if (found && coefficient == 0) {
                for (int next = index + 1; next < size; ++next) {
                    terms[next - 1] = terms[next];
                }
                --size;
            } else if (found) {
                terms[index].coefficient = coefficient;
            } else if (coefficient != 0) {
                if (size == capacity) {
                    std::cout << "FULL\n";
                } else {
                    for (int next = size; next > index; --next) {
                        terms[next] = terms[next - 1];
                    }
                    terms[index] = {exponent, coefficient};
                    ++size;
                }
            }
        } else if (command == "coeff") {
            int exponent = 0;
            std::cin >> exponent;
            if (exponent < 0) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int index = position(exponent);
                std::cout << (index < size && terms[index].exponent == exponent
                                  ? terms[index].coefficient
                                  : 0)
                          << '\n';
            }
        } else if (command == "degree") {
            std::cout << (size == 0 ? -1 : terms[0].exponent) << '\n';
        } else if (command == "leading") {
            if (size == 0) std::cout << "EMPTY\n";
            else std::cout << terms[0].exponent << ' ' << terms[0].coefficient << '\n';
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
