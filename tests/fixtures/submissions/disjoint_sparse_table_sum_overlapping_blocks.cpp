#include <iostream>
#include <memory>
#include <string>

// Known-wrong: uses an ordinary overlapping Sparse Table (built for idempotent
// queries) but combines the two blocks with addition. Because sum is not
// idempotent, the overlap of the two length-2^k blocks is counted twice, so any
// query whose length is not a power of two is too large.
class OverlappingSparseTable {
public:
    OverlappingSparseTable(const long long* values, int size) : size_(size) {
        levels_ = highestBit(static_cast<unsigned>(size_ < 1 ? 1 : size_)) + 1;
        const int slots = size_ < 1 ? 1 : size_;
        const std::size_t cells =
            static_cast<std::size_t>(levels_) * static_cast<std::size_t>(slots);
        table_ = std::make_unique<long long[]>(cells);
        for (int index = 0; index < size_; ++index) {
            table_[cell(0, index)] = values[static_cast<std::size_t>(index)];
        }
        for (int k = 1; k < levels_; ++k) {
            const int span = 1 << k;
            const int half = 1 << (k - 1);
            for (int index = 0; index + span <= size_; ++index) {
                table_[cell(k, index)] =
                    table_[cell(k - 1, index)] + table_[cell(k - 1, index + half)];
            }
        }
    }

    [[nodiscard]] long long rangeSum(int left, int right) const {
        const int k = highestBit(static_cast<unsigned>(right - left + 1));
        return table_[cell(k, left)] + table_[cell(k, right - (1 << k) + 1)];
    }

    [[nodiscard]] long long get(int index) const {
        return table_[cell(0, index)];
    }

private:
    static int highestBit(unsigned value) {
        int bit = 0;
        while (value >>= 1U) {
            ++bit;
        }
        return bit;
    }

    [[nodiscard]] std::size_t cell(int level, int index) const {
        return static_cast<std::size_t>(level) *
                   static_cast<std::size_t>(size_) +
               static_cast<std::size_t>(index);
    }

    int size_;
    int levels_;
    std::unique_ptr<long long[]> table_;
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
    OverlappingSparseTable table(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "sum") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << table.rangeSum(left, right) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << table.get(index) << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
