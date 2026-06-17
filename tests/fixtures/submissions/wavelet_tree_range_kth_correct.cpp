#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

class WaveletTree {
public:
    WaveletTree(const long long* values, int size) : size_(size) {
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

        auto ranks = std::make_unique<int[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            ranks[static_cast<std::size_t>(index)] =
                lowerBoundRank(values_[static_cast<std::size_t>(index)]);
        }
        if (size_ > 0) {
            root_ = build(ranks.get(), size_, 0, uniqueCount_ - 1);
        }
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] long long kth(int left, int right, int kthOrder) const {
        const int rank = kth(root_.get(), left, right, kthOrder);
        return uniqueValues_[static_cast<std::size_t>(rank)];
    }

    [[nodiscard]] int countLessEqual(
        int left,
        int right,
        long long value
    ) const {
        const int rankLimit = upperBoundRank(value) - 1;
        if (rankLimit < 0) {
            return 0;
        }
        if (rankLimit >= uniqueCount_ - 1) {
            return right - left + 1;
        }
        return countLessEqual(root_.get(), left, right, rankLimit);
    }

    [[nodiscard]] int frequency(int left, int right, long long value) const {
        const int rank = lowerBoundRank(value);
        if (rank >= uniqueCount_ ||
            uniqueValues_[static_cast<std::size_t>(rank)] != value) {
            return 0;
        }
        return countEqual(root_.get(), left, right, rank);
    }

private:
    struct Node {
        int low = 0;
        int high = 0;
        int length = 0;
        std::unique_ptr<int[]> prefixLeft;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

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

    std::unique_ptr<Node> build(
        const int* ranks,
        int length,
        int low,
        int high
    ) {
        auto node = std::make_unique<Node>();
        node->low = low;
        node->high = high;
        node->length = length;
        node->prefixLeft =
            std::make_unique<int[]>(static_cast<std::size_t>(length) + 1);
        node->prefixLeft[0] = 0;
        if (low == high || length == 0) {
            return node;
        }

        const int middle = low + (high - low) / 2;
        int leftCount = 0;
        for (int index = 0; index < length; ++index) {
            if (ranks[static_cast<std::size_t>(index)] <= middle) {
                ++leftCount;
            }
        }
        const int rightCount = length - leftCount;
        auto leftRanks = std::make_unique<int[]>(
            static_cast<std::size_t>(leftCount < 1 ? 1 : leftCount)
        );
        auto rightRanks = std::make_unique<int[]>(
            static_cast<std::size_t>(rightCount < 1 ? 1 : rightCount)
        );

        int leftIndex = 0;
        int rightIndex = 0;
        for (int index = 0; index < length; ++index) {
            const int rank = ranks[static_cast<std::size_t>(index)];
            if (rank <= middle) {
                leftRanks[static_cast<std::size_t>(leftIndex)] = rank;
                ++leftIndex;
            } else {
                rightRanks[static_cast<std::size_t>(rightIndex)] = rank;
                ++rightIndex;
            }
            node->prefixLeft[static_cast<std::size_t>(index) + 1] = leftIndex;
        }

        if (leftCount > 0) {
            node->left = build(leftRanks.get(), leftCount, low, middle);
        }
        if (rightCount > 0) {
            node->right = build(rightRanks.get(), rightCount, middle + 1, high);
        }
        return node;
    }

    [[nodiscard]] int kth(
        const Node* node,
        int left,
        int right,
        int kthOrder
    ) const {
        if (node->low == node->high) {
            return node->low;
        }
        const int leftBefore = node->prefixLeft[static_cast<std::size_t>(left)];
        const int leftThroughRight =
            node->prefixLeft[static_cast<std::size_t>(right) + 1];
        const int leftInRange = leftThroughRight - leftBefore;
        if (kthOrder <= leftInRange) {
            return kth(node->left.get(), leftBefore, leftThroughRight - 1, kthOrder);
        }
        const int rightBefore = left - leftBefore;
        const int rightInRange = right - left + 1 - leftInRange;
        return kth(
            node->right.get(),
            rightBefore,
            rightBefore + rightInRange - 1,
            kthOrder - leftInRange
        );
    }

    [[nodiscard]] int countLessEqual(
        const Node* node,
        int left,
        int right,
        int rankLimit
    ) const {
        if (node == nullptr || left > right || rankLimit < node->low) {
            return 0;
        }
        if (node->high <= rankLimit) {
            return right - left + 1;
        }

        const int leftBefore = node->prefixLeft[static_cast<std::size_t>(left)];
        const int leftThroughRight =
            node->prefixLeft[static_cast<std::size_t>(right) + 1];
        const int leftInRange = leftThroughRight - leftBefore;
        const int rightBefore = left - leftBefore;
        const int rightInRange = right - left + 1 - leftInRange;

        int total = 0;
        if (leftInRange > 0) {
            total += countLessEqual(
                node->left.get(),
                leftBefore,
                leftThroughRight - 1,
                rankLimit
            );
        }
        if (rightInRange > 0) {
            total += countLessEqual(
                node->right.get(),
                rightBefore,
                rightBefore + rightInRange - 1,
                rankLimit
            );
        }
        return total;
    }

    [[nodiscard]] int countEqual(
        const Node* node,
        int left,
        int right,
        int rank
    ) const {
        if (node == nullptr || left > right) {
            return 0;
        }
        if (node->low == node->high) {
            return right - left + 1;
        }
        const int middle = node->low + (node->high - node->low) / 2;
        const int leftBefore = node->prefixLeft[static_cast<std::size_t>(left)];
        const int leftThroughRight =
            node->prefixLeft[static_cast<std::size_t>(right) + 1];
        const int leftInRange = leftThroughRight - leftBefore;
        if (rank <= middle) {
            return countEqual(
                node->left.get(),
                leftBefore,
                leftThroughRight - 1,
                rank
            );
        }
        const int rightBefore = left - leftBefore;
        const int rightInRange = right - left + 1 - leftInRange;
        return countEqual(
            node->right.get(),
            rightBefore,
            rightBefore + rightInRange - 1,
            rank
        );
    }

    int size_;
    int uniqueCount_ = 0;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> uniqueValues_;
    std::unique_ptr<Node> root_;
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
    WaveletTree tree(initial.get(), size);

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
                std::cout << tree.kth(left, right, kth) << '\n';
            }
        } else if (command == "count_le") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.countLessEqual(left, right, value) << '\n';
            }
        } else if (command == "freq") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.frequency(left, right, value) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.get(index) << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
