#include <iostream>
#include <memory>
#include <string>

// Known-wrong: every internal update step allocates both children instead of
// only the child that the update descends into.
class DynamicSegmentTree {
public:
    explicit DynamicSegmentTree(long long universe)
        : universe_(universe), root_(std::make_unique<Node>()), nodeCount_(1) {}

    void add(long long coordinate, long long delta) {
        if (delta == 0) {
            return;
        }
        add(*root_, 0, universe_ - 1, coordinate, delta);
    }

    void set(long long coordinate, long long value) {
        const long long current = query(coordinate, coordinate);
        add(coordinate, value - current);
    }

    [[nodiscard]] long long query(long long left, long long right) const {
        return query(root_.get(), 0, universe_ - 1, left, right);
    }

    [[nodiscard]] long long all() const {
        return root_->sum;
    }

    [[nodiscard]] long long nodes() const {
        return nodeCount_;
    }

private:
    struct Node {
        long long sum = 0;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    void ensure(std::unique_ptr<Node>& child) {
        if (!child) {
            child = std::make_unique<Node>();
            ++nodeCount_;
        }
    }

    void add(
        Node& node,
        long long left,
        long long right,
        long long coordinate,
        long long delta
    ) {
        node.sum += delta;
        if (left == right) {
            return;
        }
        ensure(node.left);
        ensure(node.right);
        const long long middle = left + (right - left) / 2;
        if (coordinate <= middle) {
            add(*node.left, left, middle, coordinate, delta);
        } else {
            add(*node.right, middle + 1, right, coordinate, delta);
        }
    }

    [[nodiscard]] long long query(
        const Node* node,
        long long left,
        long long right,
        long long queryLeft,
        long long queryRight
    ) const {
        if (node == nullptr || queryRight < left || right < queryLeft) {
            return 0;
        }
        if (queryLeft <= left && right <= queryRight) {
            return node->sum;
        }
        const long long middle = left + (right - left) / 2;
        return query(node->left.get(), left, middle, queryLeft, queryRight) +
               query(
                   node->right.get(),
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight
               );
    }

    long long universe_;
    std::unique_ptr<Node> root_;
    long long nodeCount_;
};

int main() {
    long long universe = 0;
    int q = 0;
    std::cin >> universe >> q;
    DynamicSegmentTree tree(universe);

    auto valid = [&](long long coordinate) {
        return 0 <= coordinate && coordinate < universe;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "add") {
            long long coordinate = 0;
            long long delta = 0;
            std::cin >> coordinate >> delta;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.add(coordinate, delta);
            }
        } else if (command == "set") {
            long long coordinate = 0;
            long long value = 0;
            std::cin >> coordinate >> value;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                tree.set(coordinate, value);
            }
        } else if (command == "get") {
            long long coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(coordinate, coordinate) << '\n';
            }
        } else if (command == "prefix") {
            long long coordinate = 0;
            std::cin >> coordinate;
            if (!valid(coordinate)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(0, coordinate) << '\n';
            }
        } else if (command == "range") {
            long long left = 0;
            long long right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(left, right) << '\n';
            }
        } else if (command == "all") {
            std::cout << tree.all() << '\n';
        } else if (command == "nodes") {
            std::cout << tree.nodes() << '\n';
        } else if (command == "universe") {
            std::cout << universe << '\n';
        }
    }
}
