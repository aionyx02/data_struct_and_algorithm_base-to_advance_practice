#include <iostream>
#include <memory>
#include <string>

// Known-wrong: a range add after a pending assignment is stored as a separate
// add tag, then pushed before the assignment, so the add is overwritten.
class MixedLazyTree {
public:
    MixedLazyTree(const long long* values, int size)
        : size_(size),
          tree_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          assignValue_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          addValue_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          hasAssign_(std::make_unique<bool[]>(
              static_cast<std::size_t>(size) * 4
          )) {
        build(values, 1, 0, size_ - 1);
    }

    void add(int left, int right, long long delta) {
        rangeAdd(1, 0, size_ - 1, left, right, delta);
    }

    void assign(int left, int right, long long value) {
        rangeAssign(1, 0, size_ - 1, left, right, value);
    }

    [[nodiscard]] long long query(int left, int right) {
        return rangeQuery(1, 0, size_ - 1, left, right);
    }

    [[nodiscard]] long long all() const {
        return tree_[1];
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

    void applyAssign(int node, int left, int right, long long value) {
        tree_[static_cast<std::size_t>(node)] =
            value * static_cast<long long>(right - left + 1);
        assignValue_[static_cast<std::size_t>(node)] = value;
        addValue_[static_cast<std::size_t>(node)] = 0;
        hasAssign_[static_cast<std::size_t>(node)] = true;
    }

    void applyAdd(int node, int left, int right, long long delta) {
        tree_[static_cast<std::size_t>(node)] +=
            delta * static_cast<long long>(right - left + 1);
        addValue_[static_cast<std::size_t>(node)] += delta;
    }

    void push(int node, int left, int right) {
        if (left == right) {
            return;
        }
        const int middle = left + (right - left) / 2;
        const long long delta = addValue_[static_cast<std::size_t>(node)];
        if (delta != 0) {
            applyAdd(node * 2, left, middle, delta);
            applyAdd(node * 2 + 1, middle + 1, right, delta);
            addValue_[static_cast<std::size_t>(node)] = 0;
        }
        if (hasAssign_[static_cast<std::size_t>(node)]) {
            const long long value = assignValue_[static_cast<std::size_t>(node)];
            applyAssign(node * 2, left, middle, value);
            applyAssign(node * 2 + 1, middle + 1, right, value);
            hasAssign_[static_cast<std::size_t>(node)] = false;
        }
    }

    void pull(int node) {
        tree_[static_cast<std::size_t>(node)] =
            tree_[static_cast<std::size_t>(node * 2)] +
            tree_[static_cast<std::size_t>(node * 2 + 1)];
    }

    void rangeAdd(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight,
        long long delta
    ) {
        if (queryRight < left || right < queryLeft) {
            return;
        }
        if (queryLeft <= left && right <= queryRight) {
            applyAdd(node, left, right, delta);
            return;
        }
        push(node, left, right);
        const int middle = left + (right - left) / 2;
        rangeAdd(node * 2, left, middle, queryLeft, queryRight, delta);
        rangeAdd(node * 2 + 1, middle + 1, right, queryLeft, queryRight, delta);
        pull(node);
    }

    void rangeAssign(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight,
        long long value
    ) {
        if (queryRight < left || right < queryLeft) {
            return;
        }
        if (queryLeft <= left && right <= queryRight) {
            applyAssign(node, left, right, value);
            return;
        }
        push(node, left, right);
        const int middle = left + (right - left) / 2;
        rangeAssign(node * 2, left, middle, queryLeft, queryRight, value);
        rangeAssign(
            node * 2 + 1, middle + 1, right, queryLeft, queryRight, value
        );
        pull(node);
    }

    [[nodiscard]] long long rangeQuery(
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
            return tree_[static_cast<std::size_t>(node)];
        }
        push(node, left, right);
        const int middle = left + (right - left) / 2;
        return rangeQuery(node * 2, left, middle, queryLeft, queryRight) +
               rangeQuery(
                   node * 2 + 1,
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight
               );
    }

    int size_;
    std::unique_ptr<long long[]> tree_;
    std::unique_ptr<long long[]> assignValue_;
    std::unique_ptr<long long[]> addValue_;
    std::unique_ptr<bool[]> hasAssign_;
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
    MixedLazyTree tree(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "range_add") {
            int left = 0;
            int right = 0;
            long long delta = 0;
            std::cin >> left >> right >> delta;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.add(left, right, delta);
            }
        } else if (command == "range_assign") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.assign(left, right, value);
            }
        } else if (command == "add") {
            int index = 0;
            long long delta = 0;
            std::cin >> index >> delta;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.add(index, index, delta);
            }
        } else if (command == "set") {
            int index = 0;
            long long value = 0;
            std::cin >> index >> value;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.assign(index, index, value);
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(index, index) << '\n';
            }
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(left, right) << '\n';
            }
        } else if (command == "all") {
            std::cout << tree.all() << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
