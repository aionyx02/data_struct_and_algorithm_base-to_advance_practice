#include <iostream>
#include <memory>
#include <string>

// Block decomposition with lazy tags for range add and range sum. Each block
// stores a cached sum and a lazy add tag applied to all its elements but not yet
// written into the individual cells. Whole interior blocks take only the lazy
// tag; partial boundary blocks are updated cell by cell.
class BlockDecomposition {
public:
    BlockDecomposition(const long long* values, int size) : size_(size) {
        blockSize_ = 1;
        while ((blockSize_ + 1) * (blockSize_ + 1) <= (size_ < 1 ? 1 : size_)) {
            ++blockSize_;
        }
        blockCount_ = (size_ + blockSize_ - 1) / blockSize_;
        const int valueSlots = size_ < 1 ? 1 : size_;
        const int blockSlots = blockCount_ < 1 ? 1 : blockCount_;
        values_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(valueSlots));
        lazy_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(blockSlots));
        blockSum_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(blockSlots));
        for (int block = 0; block < blockCount_; ++block) {
            lazy_[static_cast<std::size_t>(block)] = 0;
            blockSum_[static_cast<std::size_t>(block)] = 0;
        }
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
            blockSum_[static_cast<std::size_t>(index / blockSize_)] +=
                values[static_cast<std::size_t>(index)];
        }
    }

    void add(int left, int right, long long delta) {
        const int leftBlock = left / blockSize_;
        const int rightBlock = right / blockSize_;
        if (leftBlock == rightBlock) {
            addPartial(leftBlock, left, right, delta);
            return;
        }
        addPartial(leftBlock, left, blockEnd(leftBlock), delta);
        for (int block = leftBlock + 1; block < rightBlock; ++block) {
            lazy_[static_cast<std::size_t>(block)] += delta;
            blockSum_[static_cast<std::size_t>(block)] +=
                delta * blockLength(block);
        }
        addPartial(rightBlock, blockStart(rightBlock), right, delta);
    }

    [[nodiscard]] long long sum(int left, int right) const {
        const int leftBlock = left / blockSize_;
        const int rightBlock = right / blockSize_;
        if (leftBlock == rightBlock) {
            return partialSum(leftBlock, left, right);
        }
        long long total = partialSum(leftBlock, left, blockEnd(leftBlock));
        for (int block = leftBlock + 1; block < rightBlock; ++block) {
            total += blockSum_[static_cast<std::size_t>(block)];
        }
        total += partialSum(rightBlock, blockStart(rightBlock), right);
        return total;
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)] +
               lazy_[static_cast<std::size_t>(index / blockSize_)];
    }

private:
    [[nodiscard]] int blockStart(int block) const { return block * blockSize_; }

    [[nodiscard]] int blockEnd(int block) const {
        const int end = (block + 1) * blockSize_;
        return (end < size_ ? end : size_) - 1;
    }

    [[nodiscard]] long long blockLength(int block) const {
        return blockEnd(block) - blockStart(block) + 1;
    }

    void addPartial(int block, int left, int right, long long delta) {
        for (int index = left; index <= right; ++index) {
            values_[static_cast<std::size_t>(index)] += delta;
        }
        blockSum_[static_cast<std::size_t>(block)] +=
            delta * (right - left + 1);
    }

    [[nodiscard]] long long partialSum(int block, int left, int right) const {
        long long total = 0;
        for (int index = left; index <= right; ++index) {
            total += values_[static_cast<std::size_t>(index)];
        }
        total += lazy_[static_cast<std::size_t>(block)] * (right - left + 1);
        return total;
    }

    int size_;
    int blockSize_;
    int blockCount_;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> lazy_;
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
    BlockDecomposition table(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            int left = 0;
            int right = 0;
            long long delta = 0;
            std::cin >> left >> right >> delta;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                table.add(left, right, delta);
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
