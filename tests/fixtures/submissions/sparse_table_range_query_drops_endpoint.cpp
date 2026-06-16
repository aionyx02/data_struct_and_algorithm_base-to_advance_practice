#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

// Known-wrong: the second overlapping block starts at right - 2^k instead of
// right - 2^k + 1, so it covers [right - 2^k, right - 1] and drops the right
// endpoint. Queries whose extreme value sits at the right end are wrong.
class SparseTable {
public:
    SparseTable(const long long* values, int size) : size_(size) {
        log_ = std::make_unique<int[]>(static_cast<std::size_t>(size_ + 1));
        log_[1] = 0;
        for (int length = 2; length <= size_; ++length) {
            log_[length] = log_[length / 2] + 1;
        }
        levels_ = log_[size_ < 1 ? 1 : size_] + 1;
        const std::size_t cells =
            static_cast<std::size_t>(levels_) * static_cast<std::size_t>(size_);
        minTable_ = std::make_unique<long long[]>(cells);
        maxTable_ = std::make_unique<long long[]>(cells);
        for (int index = 0; index < size_; ++index) {
            minTable_[cell(0, index)] = values[static_cast<std::size_t>(index)];
            maxTable_[cell(0, index)] = values[static_cast<std::size_t>(index)];
        }
        for (int k = 1; k < levels_; ++k) {
            const int span = 1 << k;
            const int half = 1 << (k - 1);
            for (int index = 0; index + span <= size_; ++index) {
                minTable_[cell(k, index)] = std::min(
                    minTable_[cell(k - 1, index)],
                    minTable_[cell(k - 1, index + half)]
                );
                maxTable_[cell(k, index)] = std::max(
                    maxTable_[cell(k - 1, index)],
                    maxTable_[cell(k - 1, index + half)]
                );
            }
        }
    }

    [[nodiscard]] long long rangeMin(int left, int right) const {
        const int k = log_[right - left + 1];
        // Bug: drops the right endpoint (missing the `+ 1`).
        return std::min(
            minTable_[cell(k, left)],
            minTable_[cell(k, right - (1 << k))]
        );
    }

    [[nodiscard]] long long rangeMax(int left, int right) const {
        const int k = log_[right - left + 1];
        return std::max(
            maxTable_[cell(k, left)],
            maxTable_[cell(k, right - (1 << k))]
        );
    }

private:
    [[nodiscard]] std::size_t cell(int level, int index) const {
        return static_cast<std::size_t>(level) *
                   static_cast<std::size_t>(size_) +
               static_cast<std::size_t>(index);
    }

    int size_;
    int levels_;
    std::unique_ptr<int[]> log_;
    std::unique_ptr<long long[]> minTable_;
    std::unique_ptr<long long[]> maxTable_;
};

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;
    auto initial =
        std::make_unique<long long[]>(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        std::cin >> initial[static_cast<std::size_t>(index)];
    }
    SparseTable table(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "min") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << table.rangeMin(left, right) << '\n';
            }
        } else if (command == "max") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << table.rangeMax(left, right) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << initial[static_cast<std::size_t>(index)] << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
