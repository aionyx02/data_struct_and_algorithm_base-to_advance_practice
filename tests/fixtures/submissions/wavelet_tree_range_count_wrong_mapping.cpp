#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

// Known-wrong: simulates a right-child range translation bug by losing one
// right-partition value for non-prefix count queries.
class RangeModel {
public:
    RangeModel(const long long* values, int size) : size_(size) {
        const int slots = size_ < 1 ? 1 : size_;
        values_ = std::make_unique<long long[]>(static_cast<std::size_t>(slots));
        for (int index = 0; index < size_; ++index) {
            values_[static_cast<std::size_t>(index)] =
                values[static_cast<std::size_t>(index)];
        }
        minimum_ = values_[0];
        maximum_ = values_[0];
        for (int index = 1; index < size_; ++index) {
            minimum_ = std::min(minimum_, values_[static_cast<std::size_t>(index)]);
            maximum_ = std::max(maximum_, values_[static_cast<std::size_t>(index)]);
        }
        middle_ = minimum_ + (maximum_ - minimum_) / 2;
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
        return sorted[static_cast<std::size_t>(kthOrder - 1)];
    }

    [[nodiscard]] int countLessEqual(int left, int right, long long value) const {
        int count = 0;
        bool lostRightMappedValue = false;
        for (int index = left; index <= right; ++index) {
            const long long current = values_[static_cast<std::size_t>(index)];
            if (current <= value) {
                ++count;
            }
            if (left > 0 && !lostRightMappedValue && current > middle_ &&
                current <= value) {
                lostRightMappedValue = true;
            }
        }
        if (lostRightMappedValue) {
            --count;
        }
        return count;
    }

    [[nodiscard]] int frequency(int left, int right, long long value) const {
        return countLessEqual(left, right, value) -
               countLessEqual(left, right, value - 1);
    }

private:
    int size_;
    long long minimum_ = 0;
    long long maximum_ = 0;
    long long middle_ = 0;
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
