#include <iostream>
#include <limits>
#include <memory>
#include <string>

// Segment Tree Beats (Ji Driver tree) for range chmin with range sum and range
// maximum queries. Each node tracks the maximum, the strict second maximum, the
// count of maximum elements, and the sum.
class BeatsSegmentTree {
public:
    BeatsSegmentTree(const long long* values, int size)
        : size_(size),
          maxValue_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          secondMax_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          maxCount_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          sum_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )) {
        build(values, 1, 0, size_ - 1);
    }

    void chmin(int left, int right, long long bound) {
        applyRange(1, 0, size_ - 1, left, right, bound);
    }

    [[nodiscard]] long long sum(int left, int right) {
        return querySum(1, 0, size_ - 1, left, right);
    }

    [[nodiscard]] long long max(int left, int right) {
        return queryMax(1, 0, size_ - 1, left, right);
    }

private:
    static constexpr long long kNegInf =
        std::numeric_limits<long long>::min();

    void build(const long long* values, int node, int left, int right) {
        if (left == right) {
            const auto index = static_cast<std::size_t>(node);
            maxValue_[index] = values[static_cast<std::size_t>(left)];
            secondMax_[index] = kNegInf;
            maxCount_[index] = 1;
            sum_[index] = values[static_cast<std::size_t>(left)];
            return;
        }
        const int middle = left + (right - left) / 2;
        build(values, node * 2, left, middle);
        build(values, node * 2 + 1, middle + 1, right);
        pull(node);
    }

    void pull(int node) {
        const auto self = static_cast<std::size_t>(node);
        const auto leftChild = static_cast<std::size_t>(node * 2);
        const auto rightChild = static_cast<std::size_t>(node * 2 + 1);
        sum_[self] = sum_[leftChild] + sum_[rightChild];
        if (maxValue_[leftChild] == maxValue_[rightChild]) {
            maxValue_[self] = maxValue_[leftChild];
            maxCount_[self] = maxCount_[leftChild] + maxCount_[rightChild];
            secondMax_[self] =
                std::max(secondMax_[leftChild], secondMax_[rightChild]);
        } else if (maxValue_[leftChild] > maxValue_[rightChild]) {
            maxValue_[self] = maxValue_[leftChild];
            maxCount_[self] = maxCount_[leftChild];
            secondMax_[self] =
                std::max(secondMax_[leftChild], maxValue_[rightChild]);
        } else {
            maxValue_[self] = maxValue_[rightChild];
            maxCount_[self] = maxCount_[rightChild];
            secondMax_[self] =
                std::max(maxValue_[leftChild], secondMax_[rightChild]);
        }
    }

    // Lowers only the maximum elements of the node to bound. Precondition:
    // secondMax < bound < maxValue.
    void applyTag(int node, long long bound) {
        const auto index = static_cast<std::size_t>(node);
        if (bound >= maxValue_[index]) {
            return;
        }
        sum_[index] -= (maxValue_[index] - bound) * maxCount_[index];
        maxValue_[index] = bound;
    }

    void pushDown(int node) {
        const auto index = static_cast<std::size_t>(node);
        applyTag(node * 2, maxValue_[index]);
        applyTag(node * 2 + 1, maxValue_[index]);
    }

    void applyRange(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight,
        long long bound
    ) {
        const auto index = static_cast<std::size_t>(node);
        if (queryRight < left || right < queryLeft ||
            maxValue_[index] <= bound) {
            return;
        }
        if (queryLeft <= left && right <= queryRight &&
            secondMax_[index] < bound) {
            applyTag(node, bound);
            return;
        }
        pushDown(node);
        const int middle = left + (right - left) / 2;
        applyRange(node * 2, left, middle, queryLeft, queryRight, bound);
        applyRange(
            node * 2 + 1,
            middle + 1,
            right,
            queryLeft,
            queryRight,
            bound
        );
        pull(node);
    }

    long long querySum(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight
    ) {
        if (queryRight < left || right < queryLeft) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return sum_[static_cast<std::size_t>(node)];
        }
        pushDown(node);
        const int middle = left + (right - left) / 2;
        return querySum(node * 2, left, middle, queryLeft, queryRight) +
               querySum(
                   node * 2 + 1,
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight
               );
    }

    long long queryMax(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight
    ) {
        if (queryRight < left || right < queryLeft) {
            return kNegInf;
        }
        if (queryLeft <= left && right <= queryRight) {
            return maxValue_[static_cast<std::size_t>(node)];
        }
        pushDown(node);
        const int middle = left + (right - left) / 2;
        return std::max(
            queryMax(node * 2, left, middle, queryLeft, queryRight),
            queryMax(node * 2 + 1, middle + 1, right, queryLeft, queryRight)
        );
    }

    int size_;
    std::unique_ptr<long long[]> maxValue_;
    std::unique_ptr<long long[]> secondMax_;
    std::unique_ptr<long long[]> maxCount_;
    std::unique_ptr<long long[]> sum_;
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
    BeatsSegmentTree tree(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "chmin") {
            int left = 0;
            int right = 0;
            long long bound = 0;
            std::cin >> left >> right >> bound;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.chmin(left, right, bound);
            }
        } else if (command == "sum") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.sum(left, right) << '\n';
            }
        } else if (command == "max") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.max(left, right) << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
