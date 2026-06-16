#include <iostream>
#include <memory>
#include <string>

// Known-wrong: `less x` counts elements <= x instead of strictly less than x,
// so any rank query whose bound is itself a stored key is off by one.
class OrderStatisticTree {
public:
    explicit OrderStatisticTree(int universe)
        : universe_(universe),
          tree_(std::make_unique<long long[]>(
              static_cast<std::size_t>(universe > 0 ? universe : 1) * 4
          )) {}

    void insert(int key) {
        update(1, 0, universe_ - 1, key, 1);
    }

    void erase(int key) {
        update(1, 0, universe_ - 1, key, -1);
    }

    [[nodiscard]] long long count(int key) const {
        return pointCount(1, 0, universe_ - 1, key);
    }

    [[nodiscard]] long long less(int bound) const {
        // Bug: inclusive bound; should stop at bound - 1.
        return rangeCount(1, 0, universe_ - 1, 0, bound);
    }

    [[nodiscard]] long long size() const {
        return tree_[1];
    }

    [[nodiscard]] int kth(long long order) const {
        if (order < 1 || order > tree_[1]) {
            return -1;
        }
        return descend(1, 0, universe_ - 1, order);
    }

private:
    void update(int node, int left, int right, int key, long long delta) {
        tree_[static_cast<std::size_t>(node)] += delta;
        if (left == right) {
            return;
        }
        const int middle = left + (right - left) / 2;
        if (key <= middle) {
            update(node * 2, left, middle, key, delta);
        } else {
            update(node * 2 + 1, middle + 1, right, key, delta);
        }
    }

    [[nodiscard]] long long pointCount(
        int node,
        int left,
        int right,
        int key
    ) const {
        if (left == right) {
            return tree_[static_cast<std::size_t>(node)];
        }
        const int middle = left + (right - left) / 2;
        if (key <= middle) {
            return pointCount(node * 2, left, middle, key);
        }
        return pointCount(node * 2 + 1, middle + 1, right, key);
    }

    [[nodiscard]] long long rangeCount(
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
        return rangeCount(node * 2, left, middle, queryLeft, queryRight) +
               rangeCount(node * 2 + 1, middle + 1, right, queryLeft, queryRight);
    }

    [[nodiscard]] int descend(
        int node,
        int left,
        int right,
        long long order
    ) const {
        if (left == right) {
            return left;
        }
        const int middle = left + (right - left) / 2;
        const long long leftCount = tree_[static_cast<std::size_t>(node * 2)];
        if (leftCount >= order) {
            return descend(node * 2, left, middle, order);
        }
        return descend(node * 2 + 1, middle + 1, right, order - leftCount);
    }

    int universe_;
    std::unique_ptr<long long[]> tree_;
};

int main() {
    int universe = 0;
    int q = 0;
    std::cin >> universe >> q;
    OrderStatisticTree tree(universe);

    auto validKey = [&](int key) {
        return key >= 0 && key < universe;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key = 0;
            std::cin >> key;
            if (!validKey(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.insert(key);
            }
        } else if (command == "erase") {
            int key = 0;
            std::cin >> key;
            if (!validKey(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else if (tree.count(key) == 0) {
                std::cout << "NOT_FOUND\n";
            } else {
                tree.erase(key);
            }
        } else if (command == "count") {
            int key = 0;
            std::cin >> key;
            if (!validKey(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.count(key) << '\n';
            }
        } else if (command == "less") {
            int bound = 0;
            std::cin >> bound;
            if (bound < 0 || bound > universe) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.less(bound) << '\n';
            }
        } else if (command == "kth") {
            long long order = 0;
            std::cin >> order;
            const int key = tree.kth(order);
            if (key < 0) {
                std::cout << "NONE\n";
            } else {
                std::cout << key << '\n';
            }
        } else if (command == "size") {
            std::cout << tree.size() << '\n';
        }
    }
}
