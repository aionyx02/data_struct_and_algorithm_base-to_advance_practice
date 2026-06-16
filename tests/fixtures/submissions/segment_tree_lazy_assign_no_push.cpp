#include <iostream>
#include <memory>
#include <string>

// Known-wrong: range assignments set the covered node, but partial queries and
// later updates never push the pending assignment to children.
class LazyAssignTree {
public:
    LazyAssignTree(const long long* values, int size)
        : size_(size),
          tree_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          assignValue_(std::make_unique<long long[]>(
              static_cast<std::size_t>(size) * 4
          )),
          hasAssign_(std::make_unique<bool[]>(
              static_cast<std::size_t>(size) * 4
          )) {
        build(values, 1, 0, size_ - 1);
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

    void apply(int node, int left, int right, long long value) {
        tree_[static_cast<std::size_t>(node)] =
            value * static_cast<long long>(right - left + 1);
        assignValue_[static_cast<std::size_t>(node)] = value;
        hasAssign_[static_cast<std::size_t>(node)] = true;
    }

    void pull(int node) {
        tree_[static_cast<std::size_t>(node)] =
            tree_[static_cast<std::size_t>(node * 2)] +
            tree_[static_cast<std::size_t>(node * 2 + 1)];
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
            apply(node, left, right, value);
            return;
        }
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
    LazyAssignTree tree(initial.get(), size);

    auto valid = [&](int index) {
        return index >= 0 && index < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "range_assign") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.assign(left, right, value);
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
