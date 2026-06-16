#include <iostream>
#include <limits>
#include <memory>
#include <string>

// Known-wrong: searches use the Segment Tree maximum, but they ignore the
// requested left/right boundary and search the whole array.
class MaxSegmentTree {
public:
    MaxSegmentTree(const long long* values, int size)
        : size_(size),
          tree_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )) {
        build(values, 1, 0, size_ - 1);
    }

    void set(int index, long long value) {
        pointSet(1, 0, size_ - 1, index, value);
    }

    void add(int index, long long delta) {
        const long long current = query(index, index);
        set(index, current + delta);
    }

    [[nodiscard]] long long query(int left, int right) const {
        return rangeMax(1, 0, size_ - 1, left, right);
    }

    [[nodiscard]] long long allMax() const {
        return tree_[1];
    }

    [[nodiscard]] int firstAtLeast(int left, long long threshold) const {
        (void)left;
        if (tree_[1] < threshold) {
            return -1;
        }
        return findFirst(1, 0, size_ - 1, threshold);
    }

    [[nodiscard]] int lastAtLeast(int right, long long threshold) const {
        (void)right;
        if (tree_[1] < threshold) {
            return -1;
        }
        return findLast(1, 0, size_ - 1, threshold);
    }

private:
    void build(const long long* values, int node, int left, int right) {
        if (left == right) {
            tree_[static_cast<std::size_t>(node)] =
                values[static_cast<std::size_t>(left)];
            return;
        }
        const int middle = left + (right - left) / 2;
        build(values, node * 2, left, middle);
        build(values, node * 2 + 1, middle + 1, right);
        pull(node);
    }

    void pointSet(
        int node,
        int left,
        int right,
        int index,
        long long value
    ) {
        if (left == right) {
            tree_[static_cast<std::size_t>(node)] = value;
            return;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            pointSet(node * 2, left, middle, index, value);
        } else {
            pointSet(node * 2 + 1, middle + 1, right, index, value);
        }
        pull(node);
    }

    [[nodiscard]] long long rangeMax(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight
    ) const {
        if (queryRight < left || right < queryLeft) {
            return std::numeric_limits<long long>::lowest();
        }
        if (queryLeft <= left && right <= queryRight) {
            return tree_[static_cast<std::size_t>(node)];
        }
        const int middle = left + (right - left) / 2;
        const long long leftMax =
            rangeMax(node * 2, left, middle, queryLeft, queryRight);
        const long long rightMax =
            rangeMax(node * 2 + 1, middle + 1, right, queryLeft, queryRight);
        return leftMax > rightMax ? leftMax : rightMax;
    }

    [[nodiscard]] int findFirst(
        int node,
        int left,
        int right,
        long long threshold
    ) const {
        if (tree_[static_cast<std::size_t>(node)] < threshold) {
            return -1;
        }
        if (left == right) {
            return left;
        }
        const int middle = left + (right - left) / 2;
        const int leftAnswer =
            findFirst(node * 2, left, middle, threshold);
        if (leftAnswer != -1) {
            return leftAnswer;
        }
        return findFirst(node * 2 + 1, middle + 1, right, threshold);
    }

    [[nodiscard]] int findLast(
        int node,
        int left,
        int right,
        long long threshold
    ) const {
        if (tree_[static_cast<std::size_t>(node)] < threshold) {
            return -1;
        }
        if (left == right) {
            return left;
        }
        const int middle = left + (right - left) / 2;
        const int rightAnswer =
            findLast(node * 2 + 1, middle + 1, right, threshold);
        if (rightAnswer != -1) {
            return rightAnswer;
        }
        return findLast(node * 2, left, middle, threshold);
    }

    void pull(int node) {
        const long long leftMax = tree_[static_cast<std::size_t>(node * 2)];
        const long long rightMax =
            tree_[static_cast<std::size_t>(node * 2 + 1)];
        tree_[static_cast<std::size_t>(node)] =
            leftMax > rightMax ? leftMax : rightMax;
    }

    int size_;
    std::unique_ptr<long long[]> tree_;
};

void printPosition(int position) {
    if (position == -1) {
        std::cout << "NONE\n";
    } else {
        std::cout << position << '\n';
    }
}

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;
    auto initial =
        std::make_unique<long long[]>(static_cast<std::size_t>(size));
    for (int index = 0; index < size; ++index) {
        std::cin >> initial[static_cast<std::size_t>(index)];
    }
    MaxSegmentTree tree(initial.get(), size);

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
                tree.set(index, value);
            }
        } else if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.add(index, delta);
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(index, index) << '\n';
            }
        } else if (command == "range_max") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(left, right) << '\n';
            }
        } else if (command == "first_at_least") {
            int left = 0;
            long long threshold = 0;
            std::cin >> left >> threshold;
            if (!valid(left)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                printPosition(tree.firstAtLeast(left, threshold));
            }
        } else if (command == "last_at_least") {
            int right = 0;
            long long threshold = 0;
            std::cin >> right >> threshold;
            if (!valid(right)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                printPosition(tree.lastAtLeast(right, threshold));
            }
        } else if (command == "all_max") {
            std::cout << tree.allMax() << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
