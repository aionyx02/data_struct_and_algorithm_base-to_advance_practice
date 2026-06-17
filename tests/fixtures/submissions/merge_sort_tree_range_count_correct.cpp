#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

class MergeSortTree {
public:
    MergeSortTree(const long long* values, int size) : size_(size) {
        const int valueSlots = size_ < 1 ? 1 : size_;
        values_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(valueSlots));
        uniqueValues_ =
            std::make_unique<long long[]>(static_cast<std::size_t>(valueSlots));
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
            uniqueValues_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
        std::sort(uniqueValues_.get(), uniqueValues_.get() + size_);
        uniqueCount_ = 0;
        for (int index = 0; index < size_; ++index) {
            if (index == 0 ||
                uniqueValues_[static_cast<std::size_t>(index)] !=
                    uniqueValues_[static_cast<std::size_t>(index - 1)]) {
                uniqueValues_[static_cast<std::size_t>(uniqueCount_)] =
                    uniqueValues_[static_cast<std::size_t>(index)];
                ++uniqueCount_;
            }
        }

        const int nodeSlots = size_ < 1 ? 1 : 4 * size_ + 4;
        nodes_ =
            std::make_unique<Node[]>(static_cast<std::size_t>(nodeSlots));
        if (size_ > 0) {
            build(1, 0, size_ - 1);
        }
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] int countLess(int left, int right, long long value) const {
        return countLess(1, 0, size_ - 1, left, right, value);
    }

    [[nodiscard]] int countLessEqual(
        int left,
        int right,
        long long value
    ) const {
        return countLessEqual(1, 0, size_ - 1, left, right, value);
    }

    [[nodiscard]] long long kth(int left, int right, int kth) const {
        int low = 0;
        int high = uniqueCount_ - 1;
        long long answer = uniqueValues_[static_cast<std::size_t>(high)];
        while (low <= high) {
            const int middle = low + (high - low) / 2;
            const long long candidate =
                uniqueValues_[static_cast<std::size_t>(middle)];
            if (countLessEqual(left, right, candidate) >= kth) {
                answer = candidate;
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        }
        return answer;
    }

private:
    struct Node {
        std::unique_ptr<long long[]> values;
        int length = 0;
    };

    void build(int node, int left, int right) {
        if (left == right) {
            nodes_[static_cast<std::size_t>(node)].length = 1;
            nodes_[static_cast<std::size_t>(node)].values =
                std::make_unique<long long[]>(1);
            nodes_[static_cast<std::size_t>(node)].values[0] =
                values_[static_cast<std::size_t>(left)];
            return;
        }

        const int middle = left + (right - left) / 2;
        build(node * 2, left, middle);
        build(node * 2 + 1, middle + 1, right);

        Node& current = nodes_[static_cast<std::size_t>(node)];
        const Node& leftNode = nodes_[static_cast<std::size_t>(node * 2)];
        const Node& rightNode = nodes_[static_cast<std::size_t>(node * 2 + 1)];
        current.length = leftNode.length + rightNode.length;
        current.values =
            std::make_unique<long long[]>(static_cast<std::size_t>(
                current.length
            ));
        std::merge(
            leftNode.values.get(),
            leftNode.values.get() + leftNode.length,
            rightNode.values.get(),
            rightNode.values.get() + rightNode.length,
            current.values.get()
        );
    }

    [[nodiscard]] int countLess(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight,
        long long value
    ) const {
        if (queryRight < left || right < queryLeft) {
            return 0;
        }
        const Node& current = nodes_[static_cast<std::size_t>(node)];
        if (queryLeft <= left && right <= queryRight) {
            return static_cast<int>(
                std::lower_bound(
                    current.values.get(),
                    current.values.get() + current.length,
                    value
                ) -
                current.values.get()
            );
        }
        const int middle = left + (right - left) / 2;
        return countLess(node * 2, left, middle, queryLeft, queryRight, value) +
               countLess(
                   node * 2 + 1,
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight,
                   value
               );
    }

    [[nodiscard]] int countLessEqual(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight,
        long long value
    ) const {
        if (queryRight < left || right < queryLeft) {
            return 0;
        }
        const Node& current = nodes_[static_cast<std::size_t>(node)];
        if (queryLeft <= left && right <= queryRight) {
            return static_cast<int>(
                std::upper_bound(
                    current.values.get(),
                    current.values.get() + current.length,
                    value
                ) -
                current.values.get()
            );
        }
        const int middle = left + (right - left) / 2;
        return countLessEqual(
                   node * 2,
                   left,
                   middle,
                   queryLeft,
                   queryRight,
                   value
               ) +
               countLessEqual(
                   node * 2 + 1,
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight,
                   value
               );
    }

    int size_;
    int uniqueCount_ = 0;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> uniqueValues_;
    std::unique_ptr<Node[]> nodes_;
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
    MergeSortTree tree(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "count_lt") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.countLess(left, right, value) << '\n';
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
        } else if (command == "kth") {
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
