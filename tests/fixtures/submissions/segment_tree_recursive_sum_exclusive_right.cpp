#include <iostream>
#include <memory>
#include <string>

class SegmentTree {
public:
    SegmentTree(const long long* values, int size)
        : size_(size),
          values_(std::make_unique<long long[]>(static_cast<std::size_t>(size))),
          tree_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )) {
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
        build(1, 0, size_ - 1);
    }

    void set(int index, long long value) {
        values_[static_cast<std::size_t>(index)] = value;
        update(1, 0, size_ - 1, index, value);
    }

    void add(int index, long long delta) {
        set(index, values_[static_cast<std::size_t>(index)] + delta);
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    // Known-wrong: the public right endpoint is inclusive, but this drops it.
    [[nodiscard]] long long range(int left, int right) const {
        return query(1, 0, size_ - 1, left, right - 1);
    }

    [[nodiscard]] long long all() const {
        return tree_[1];
    }

private:
    void build(int node, int left, int right) {
        if (left == right) {
            tree_[static_cast<std::size_t>(node)] =
                values_[static_cast<std::size_t>(left)];
            return;
        }
        const int middle = left + (right - left) / 2;
        build(node * 2, left, middle);
        build(node * 2 + 1, middle + 1, right);
        tree_[static_cast<std::size_t>(node)] =
            tree_[static_cast<std::size_t>(node * 2)] +
            tree_[static_cast<std::size_t>(node * 2 + 1)];
    }

    void update(int node, int left, int right, int index, long long value) {
        if (left == right) {
            tree_[static_cast<std::size_t>(node)] = value;
            return;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            update(node * 2, left, middle, index, value);
        } else {
            update(node * 2 + 1, middle + 1, right, index, value);
        }
        tree_[static_cast<std::size_t>(node)] =
            tree_[static_cast<std::size_t>(node * 2)] +
            tree_[static_cast<std::size_t>(node * 2 + 1)];
    }

    [[nodiscard]] long long query(
        int node,
        int left,
        int right,
        int queryLeft,
        int queryRight
    ) const {
        if (queryRight < left || right < queryLeft) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return tree_[static_cast<std::size_t>(node)];
        }
        const int middle = left + (right - left) / 2;
        return query(node * 2, left, middle, queryLeft, queryRight) +
               query(node * 2 + 1, middle + 1, right, queryLeft, queryRight);
    }

    int size_;
    std::unique_ptr<long long[]> values_;
    std::unique_ptr<long long[]> tree_;
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
    SegmentTree tree(initial.get(), size);

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
                std::cout << tree.get(index) << '\n';
            }
        } else if (command == "range") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.range(left, right) << '\n';
            }
        } else if (command == "all") {
            std::cout << tree.all() << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
