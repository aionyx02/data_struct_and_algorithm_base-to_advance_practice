#include <iostream>
#include <memory>
#include <string>

int main() {
    int rows = 0;
    int columns = 0;
    int nonzero = 0;
    int operationCount = 0;
    std::cin >> rows >> columns >> nonzero >> operationCount;
    auto rowOffsets = std::make_unique<int[]>(rows + 1);
    auto columnIndices = std::make_unique<int[]>(nonzero);
    auto values = std::make_unique<int[]>(nonzero);

    for (int index = 0; index < nonzero; ++index) {
        int row = 0;
        std::cin >> row >> columnIndices[index] >> values[index];
        ++rowOffsets[row + 1];
    }
    for (int row = 1; row <= rows; ++row) {
        rowOffsets[row] += rowOffsets[row - 1];
    }

    auto valid = [&](int row, int column) {
        return row >= 0 && row < rows && column >= 0 && column < columns;
    };

    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "get") {
            int row = 0;
            int column = 0;
            std::cin >> row >> column;
            if (!valid(row, column)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                int value = 0;
                int left = rowOffsets[row];
                int right = rowOffsets[row + 1] - 1;
                while (left <= right) {
                    const int middle = left + (right - left) / 2;
                    if (columnIndices[middle] == column) {
                        value = values[middle];
                        break;
                    }
                    if (columnIndices[middle] < column) left = middle + 1;
                    else right = middle - 1;
                }
                std::cout << value << '\n';
            }
        } else if (command == "row_nnz" || command == "row_sum") {
            int row = 0;
            std::cin >> row;
            if (row < 0 || row >= rows) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (command == "row_nnz") {
                std::cout << rowOffsets[row + 1] - rowOffsets[row] << '\n';
            } else {
                int sum = 0;
                for (int index = rowOffsets[row]; index < rowOffsets[row + 1]; ++index) {
                    sum += values[index];
                }
                std::cout << sum << '\n';
            }
        } else if (command == "row_start") {
            int row = 0;
            std::cin >> row;
            if (row < 0 || row > rows) std::cout << "OUT_OF_RANGE\n";
            else std::cout << rowOffsets[row] << '\n';
        } else if (command == "term") {
            int index = 0;
            std::cin >> index;
            if (index < 0 || index >= nonzero) std::cout << "OUT_OF_RANGE\n";
            else std::cout << columnIndices[index] << ' ' << values[index] << '\n';
        } else if (command == "nonzero") {
            std::cout << nonzero << '\n';
        }
    }
}
