#include <iostream>
#include <memory>
#include <string>

// Square Root Decomposition for point update and range sum. The array is split
// into blocks of size about sqrt(n); each block caches its sum so a range query
// reads only the two partial boundary blocks and the cached whole blocks.
class SqrtDecomposition {
public:
    SqrtDecomposition(const long long* values, int size) : size_(size) {
        blockSize_ = 1;
        while ((blockSize_ + 1) * (blockSize_ + 1) <= (size_ < 1 ? 1 : size_)) {
            ++blockSize_;
        }
        blockCount_ = (size_ + blockSize_ - 1) / blockSize_;
        const int valueSlots = size_ < 1 ? 1 : size_;
        const int blockSlots = blockCount_ < 1 ? 1 : blockCount_;
        values_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(valueSlots));
        blockSum_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(blockSlots));
        for (int block = 0; block < blockCount_; ++block) {
            blockSum_[static_cast<std::size_t>(block)] = 0;
        }
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
            blockSum_[static_cast<std::size_t>(index / blockSize_)] +=
                values[static_cast<std::size_t>(index)];
        }
    }

    void set(int index, long long value) {
        const std::size_t cell = static_cast<std::size_t>(index);
        blockSum_[static_cast<std::size_t>(index / blockSize_)] +=
            value - values_[cell];
        values_[cell] = value;
    }

    void add(int index, long long delta) {
        values_[static_cast<std::size_t>(index)] += delta;
        blockSum_[static_cast<std::size_t>(index / blockSize_)] += delta;
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] long long sum(int left, int right) const {
        const int leftBlock = left / blockSize_;
        const int rightBlock = right / blockSize_;
        long long total = 0;
        if (leftBlock == rightBlock) {
            for (int index = left; index <= right; ++index) {
                total += values_[static_cast<std::size_t>(index)];
            }
            return total;
        }
        for (int index = left; index < (leftBlock + 1) * blockSize_; ++index) {
            total += values_[static_cast<std::size_t>(index)];
        }
        for (int block = leftBlock + 1; block < rightBlock; ++block) {
            total += blockSum_[static_cast<std::size_t>(block)];
        }
        for (int index = rightBlock * blockSize_; index <= right; ++index) {
            total += values_[static_cast<std::size_t>(index)];
        }
        return total;
    }

private:
    int size_;
    int blockSize_;
    int blockCount_;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> blockSum_;
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
    SqrtDecomposition table(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                table.set(index, value);
            }
        } else if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                table.add(index, delta);
            }
        } else if (command == "sum") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << table.sum(left, right) << '\n';
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
