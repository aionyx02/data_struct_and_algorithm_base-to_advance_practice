#include <iostream>
#include <memory>
#include <string>

// Disjoint Sparse Table for static range sum. At level h the array is split into
// segments of length 2^(h+1); each position stores the aggregate toward its
// segment midpoint (suffix on the left half, prefix on the right half). A query
// combines two disjoint halves, so it works for non-idempotent operations.
class DisjointSparseTable {
public:
    DisjointSparseTable(const long long* values, int size) : size_(size) {
        const int slots = size_ < 1 ? 1 : size_;
        values_ = std::make_unique<long long[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
        levels_ = size_ >= 2 ? highestBit(static_cast<unsigned>(size_ - 1)) + 1 : 1;
        const std::size_t cells =
            static_cast<std::size_t>(levels_) * static_cast<std::size_t>(slots);
        table_ = std::make_unique<long long[]>(cells);
        for (int h = 0; h < levels_; ++h) {
            const int blockSize = 1 << (h + 1);
            for (int start = 0; start < size_; start += blockSize) {
                const int mid = start + (1 << h);
                const int leftEnd = mid < size_ ? mid : size_;  // exclusive
                long long acc = 0;
                for (int i = leftEnd - 1; i >= start; --i) {
                    acc += values_[static_cast<std::size_t>(i)];
                    table_[cell(h, i)] = acc;
                }
                if (mid < size_) {
                    const int blockEnd =
                        start + blockSize < size_ ? start + blockSize : size_;
                    acc = 0;
                    for (int i = mid; i < blockEnd; ++i) {
                        acc += values_[static_cast<std::size_t>(i)];
                        table_[cell(h, i)] = acc;
                    }
                }
            }
        }
    }

    [[nodiscard]] long long rangeSum(int left, int right) const {
        if (left == right) {
            return values_[static_cast<std::size_t>(left)];
        }
        const int h = highestBit(static_cast<unsigned>(left ^ right));
        return table_[cell(h, left)] + table_[cell(h, right)];
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
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
    std::unique_ptr<long long[]> values_;
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
    DisjointSparseTable table(initial.get(), size);

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
