#include <iostream>
#include <memory>
#include <string>

class PersistentSegmentTree {
public:
    PersistentSegmentTree(const long long* values, int size, int operations)
        : size_(size),
          capacity_(static_cast<std::size_t>(size) * 4 +
                    static_cast<std::size_t>(operations + 1) * 80 + 16),
          nodes_(std::make_unique<Node[]>(capacity_)),
          roots_(std::make_unique<int[]>(
              static_cast<std::size_t>(operations + 2)
          )),
          nodeCount_(0),
          versionCount_(1) {
        roots_[0] = build(values, 0, size_ - 1);
    }

    [[nodiscard]] int versionCount() const {
        return versionCount_;
    }

    [[nodiscard]] long long query(int version, int left, int right) const {
        return rangeQuery(roots_[version], 0, size_ - 1, left, right);
    }

    int set(int baseVersion, int index, long long value) {
        roots_[versionCount_] =
            updateSet(roots_[baseVersion], 0, size_ - 1, index, value);
        return versionCount_++;
    }

    int add(int baseVersion, int index, long long delta) {
        roots_[versionCount_] =
            updateAdd(roots_[baseVersion], 0, size_ - 1, index, delta);
        return versionCount_++;
    }

private:
    struct Node {
        long long sum = 0;
        int left = 0;
        int right = 0;
    };

    int newNode() {
        return ++nodeCount_;
    }

    int cloneNode(int source) {
        const int target = newNode();
        nodes_[static_cast<std::size_t>(target)] =
            nodes_[static_cast<std::size_t>(source)];
        return target;
    }

    int build(const long long* values, int left, int right) {
        const int node = newNode();
        if (left == right) {
            nodes_[static_cast<std::size_t>(node)].sum =
                values[static_cast<std::size_t>(left)];
            return node;
        }
        const int middle = left + (right - left) / 2;
        nodes_[static_cast<std::size_t>(node)].left =
            build(values, left, middle);
        nodes_[static_cast<std::size_t>(node)].right =
            build(values, middle + 1, right);
        pull(node);
        return node;
    }

    int updateSet(
        int source,
        int left,
        int right,
        int index,
        long long value
    ) {
        const int node = cloneNode(source);
        if (left == right) {
            nodes_[static_cast<std::size_t>(node)].sum = value;
            return node;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            nodes_[static_cast<std::size_t>(node)].left = updateSet(
                nodes_[static_cast<std::size_t>(source)].left,
                left,
                middle,
                index,
                value
            );
        } else {
            nodes_[static_cast<std::size_t>(node)].right = updateSet(
                nodes_[static_cast<std::size_t>(source)].right,
                middle + 1,
                right,
                index,
                value
            );
        }
        pull(node);
        return node;
    }

    int updateAdd(
        int source,
        int left,
        int right,
        int index,
        long long delta
    ) {
        const int node = cloneNode(source);
        if (left == right) {
            nodes_[static_cast<std::size_t>(node)].sum += delta;
            return node;
        }
        const int middle = left + (right - left) / 2;
        if (index <= middle) {
            nodes_[static_cast<std::size_t>(node)].left = updateAdd(
                nodes_[static_cast<std::size_t>(source)].left,
                left,
                middle,
                index,
                delta
            );
        } else {
            nodes_[static_cast<std::size_t>(node)].right = updateAdd(
                nodes_[static_cast<std::size_t>(source)].right,
                middle + 1,
                right,
                index,
                delta
            );
        }
        pull(node);
        return node;
    }

    [[nodiscard]] long long rangeQuery(
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
            return nodes_[static_cast<std::size_t>(node)].sum;
        }
        const int middle = left + (right - left) / 2;
        return rangeQuery(
                   nodes_[static_cast<std::size_t>(node)].left,
                   left,
                   middle,
                   queryLeft,
                   queryRight
               ) +
               rangeQuery(
                   nodes_[static_cast<std::size_t>(node)].right,
                   middle + 1,
                   right,
                   queryLeft,
                   queryRight
               );
    }

    void pull(int node) {
        const int leftChild = nodes_[static_cast<std::size_t>(node)].left;
        const int rightChild = nodes_[static_cast<std::size_t>(node)].right;
        nodes_[static_cast<std::size_t>(node)].sum =
            nodes_[static_cast<std::size_t>(leftChild)].sum +
            nodes_[static_cast<std::size_t>(rightChild)].sum;
    }

    int size_;
    std::size_t capacity_;
    std::unique_ptr<Node[]> nodes_;
    std::unique_ptr<int[]> roots_;
    int nodeCount_;
    int versionCount_;
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
    PersistentSegmentTree tree(initial.get(), size, q);

    auto validIndex = [&](int index) {
        return 0 <= index && index < size;
    };
    auto validVersion = [&](int version) {
        return 0 <= version && version < tree.versionCount();
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int version = 0;
            int index = 0;
            long long value = 0;
            std::cin >> version >> index >> value;
            if (!validVersion(version) || !validIndex(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.set(version, index, value) << '\n';
            }
        } else if (command == "add") {
            int version = 0;
            int index = 0;
            long long delta = 0;
            std::cin >> version >> index >> delta;
            if (!validVersion(version) || !validIndex(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.add(version, index, delta) << '\n';
            }
        } else if (command == "get") {
            int version = 0;
            int index = 0;
            std::cin >> version >> index;
            if (!validVersion(version) || !validIndex(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(version, index, index) << '\n';
            }
        } else if (command == "range") {
            int version = 0;
            int left = 0;
            int right = 0;
            std::cin >> version >> left >> right;
            if (!validVersion(version) || !validIndex(left) ||
                !validIndex(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(version, left, right) << '\n';
            }
        } else if (command == "all") {
            int version = 0;
            std::cin >> version;
            if (!validVersion(version)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << tree.query(version, 0, size - 1) << '\n';
            }
        } else if (command == "versions") {
            std::cout << tree.versionCount() << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
