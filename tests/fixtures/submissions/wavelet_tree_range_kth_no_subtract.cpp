#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

// Known-wrong: models the Wavelet Tree kth descend but forgets to subtract the
// left-partition count when the answer moves into the right partition.
class RangeModel {
public:
    RangeModel(const long long* values, int size) : size_(size) {
        const int slots = size_ < 1 ? 1 : size_;
        values_ = std::make_unique<long long[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
    }

    [[nodiscard]] long long get(int index) const {
        return values_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] long long kth(int left, int right, int kthOrder) const {
        const int length = right - left + 1;
        auto sorted =
            std::make_unique<long long[]>(static_cast<std::size_t>(length));
        for (int offset = 0; offset < length; ++offset) {
            sorted[static_cast<std::size_t>(offset)] =
                values_[static_cast<std::size_t>(left + offset)];
        }
        std::sort(sorted.get(), sorted.get() + length);
        const int leftPartitionCount = length / 2;
        int index = kthOrder - 1;
        if (kthOrder > leftPartitionCount) {
            index += leftPartitionCount;
        }
        if (index >= length) {
            index = length - 1;
        }
        return sorted[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] int countLessEqual(int left, int right, long long value) const {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values_[static_cast<std::size_t>(index)] <= value) {
                ++count;
            }
        }
        return count;
    }

    [[nodiscard]] int frequency(int left, int right, long long value) const {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values_[static_cast<std::size_t>(index)] == value) {
                ++count;
            }
        }
        return count;
    }

private:
    int size_;
    std::unique_ptr<long long[]> values_;
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
    RangeModel tree(initial.get(), size);

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
