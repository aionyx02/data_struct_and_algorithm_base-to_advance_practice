#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

class WaveletMatrix {
public:
    WaveletMatrix(const long long* values, int size) : size_(size) {
        const int slots = size_ < 1 ? 1 : size_;
        values_ = std::make_unique<long long[]>(static_cast<std::size_t>(slots));
        uniqueValues_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
            uniqueValues_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
        std::sort(uniqueValues_.get(), uniqueValues_.get() + size_);
        for (int index = 0; index < size_; ++index) {
            if (index == 0 ||
                uniqueValues_[static_cast<std::size_t>(index)] !=
                    uniqueValues_[static_cast<std::size_t>(index - 1)]) {
                uniqueValues_[static_cast<std::size_t>(uniqueCount_)] =
                    uniqueValues_[static_cast<std::size_t>(index)];
                ++uniqueCount_;
            }
        }

        bitLength_ = 1;
        while ((1 << bitLength_) < uniqueCount_) {
            ++bitLength_;
        }
        zeroCount_ =
            std::make_unique<int[]>(static_cast<std::size_t>(bitLength_));
        prefixOnes_ =
            std::make_unique<std::unique_ptr<int[]>[]>(
                static_cast<std::size_t>(bitLength_)
            );
        for (int level = 0; level < bitLength_; ++level) {
            prefixOnes_[static_cast<std::size_t>(level)] =
                std::make_unique<int[]>(static_cast<std::size_t>(size_) + 1);
        }

        auto current = std::make_unique<int[]>(static_cast<std::size_t>(slots));
        auto next = std::make_unique<int[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            current[static_cast<std::size_t>(index)] =
                lowerBoundRank(values_[static_cast<std::size_t>(index)]);
        }

        for (int level = 0; level < bitLength_; ++level) {
            const int bit = bitLength_ - level - 1;
            int zeros = 0;
            prefixOnes_[static_cast<std::size_t>(level)][0] = 0;
            for (int index = 0; index < size_; ++index) {
                const int rank = current[static_cast<std::size_t>(index)];
                const bool one = ((rank >> bit) & 1) != 0;
                if (!one) {
                    ++zeros;
                }
                prefixOnes_[static_cast<std::size_t>(level)]
                           [static_cast<std::size_t>(index) + 1] =
                    prefixOnes_[static_cast<std::size_t>(level)]
                               [static_cast<std::size_t>(index)] +
                    (one ? 1 : 0);
            }
            zeroCount_[static_cast<std::size_t>(level)] = zeros;

            int zeroIndex = 0;
            int oneIndex = zeros;
            for (int index = 0; index < size_; ++index) {
                const int rank = current[static_cast<std::size_t>(index)];
                if (((rank >> bit) & 1) == 0) {
                    next[static_cast<std::size_t>(zeroIndex)] = rank;
                    ++zeroIndex;
                } else {
                    next[static_cast<std::size_t>(oneIndex)] = rank;
                    ++oneIndex;
                }
            }
            current.swap(next);
        }
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] long long kth(int left, int right, int kthOrder) const {
        int leftPos = left;
        int rightPos = right + 1;
        int rank = 0;
        for (int level = 0; level < bitLength_; ++level) {
            const int bit = bitLength_ - level - 1;
            const int leftOnes = ones(level, leftPos);
            const int rightOnes = ones(level, rightPos);
            const int onesInRange = rightOnes - leftOnes;
            const int zerosInRange = rightPos - leftPos - onesInRange;
            if (kthOrder <= zerosInRange) {
                leftPos -= leftOnes;
                rightPos -= rightOnes;
            } else {
                kthOrder -= zerosInRange;
                rank |= 1 << bit;
                leftPos = zeroCount_[static_cast<std::size_t>(level)] + leftOnes;
                rightPos =
                    zeroCount_[static_cast<std::size_t>(level)] + rightOnes;
            }
        }
        return uniqueValues_[static_cast<std::size_t>(rank)];
    }

    [[nodiscard]] int countLess(int left, int right, long long value) const {
        return countLessRank(left, right + 1, lowerBoundRank(value));
    }

    [[nodiscard]] int countLessEqual(
        int left,
        int right,
        long long value
    ) const {
        return countLessRank(left, right + 1, upperBoundRank(value));
    }

    [[nodiscard]] int frequency(int left, int right, long long value) const {
        return countLessEqual(left, right, value) -
               countLess(left, right, value);
    }

private:
    [[nodiscard]] int lowerBoundRank(long long value) const {
        return static_cast<int>(
            std::lower_bound(
                uniqueValues_.get(),
                uniqueValues_.get() + uniqueCount_,
                value
            ) -
            uniqueValues_.get()
        );
    }

    [[nodiscard]] int upperBoundRank(long long value) const {
        return static_cast<int>(
            std::upper_bound(
                uniqueValues_.get(),
                uniqueValues_.get() + uniqueCount_,
                value
            ) -
            uniqueValues_.get()
        );
    }

    [[nodiscard]] int ones(int level, int position) const {
        return prefixOnes_[static_cast<std::size_t>(level)]
                          [static_cast<std::size_t>(position)];
    }

    [[nodiscard]] int countLessRank(
        int leftPos,
        int rightPos,
        int rankExclusive
    ) const {
        if (rankExclusive <= 0) {
            return 0;
        }
        if (rankExclusive >= uniqueCount_) {
            return rightPos - leftPos;
        }

        int count = 0;
        for (int level = 0; level < bitLength_; ++level) {
            const int bit = bitLength_ - level - 1;
            const int leftOnes = ones(level, leftPos);
            const int rightOnes = ones(level, rightPos);
            const int leftZeros = leftPos - leftOnes;
            const int rightZeros = rightPos - rightOnes;
            if (((rankExclusive >> bit) & 1) != 0) {
                count += rightZeros - leftZeros;
                leftPos = zeroCount_[static_cast<std::size_t>(level)] + leftOnes;
                rightPos =
                    zeroCount_[static_cast<std::size_t>(level)] + rightOnes;
            } else {
                leftPos = leftZeros;
                rightPos = rightZeros;
            }
        }
        return count;
    }

    int size_;
    int uniqueCount_ = 0;
    int bitLength_ = 0;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> uniqueValues_;
    std::unique_ptr<int[]> zeroCount_;
    std::unique_ptr<std::unique_ptr<int[]>[]> prefixOnes_;
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
    WaveletMatrix matrix(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "kth") {
            int left = 0;
            int right = 0;
            int kth = 0;
            std::cin >> left >> right >> kth;
            if (!valid(left) || !valid(right) || left > right || kth < 1 ||
                kth > right - left + 1) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.kth(left, right, kth) << '\n';
            }
        } else if (command == "count_lt") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.countLess(left, right, value) << '\n';
            }
        } else if (command == "count_le") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.countLessEqual(left, right, value) << '\n';
            }
        } else if (command == "freq") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.frequency(left, right, value) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.get(index) << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
