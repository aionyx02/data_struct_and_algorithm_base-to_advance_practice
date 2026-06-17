#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

// Known-wrong: when kth moves into the one-bit partition, it behaves as if the
// zero-count offset were missing and reads from the beginning of the partitioned
// order instead of the one-side offset.
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
        if (kthOrder > length / 2) {
            return sorted[static_cast<std::size_t>(kthOrder - length / 2 - 1)];
        }
        return sorted[static_cast<std::size_t>(kthOrder - 1)];
    }

    [[nodiscard]] int countLess(int left, int right, long long value) const {
        int count = 0;
        for (int index = left; index <= right; ++index) {
            if (values_[static_cast<std::size_t>(index)] < value) {
                ++count;
            }
        }
        return count;
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
    RangeModel matrix(initial.get(), size);

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
                std::cout << matrix.kth(left, right, kth) << '\n';
            }
        } else if (command == "count_lt") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.countLess(left, right, value) << '\n';
            }
        } else if (command == "count_le") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.countLessEqual(left, right, value) << '\n';
            }
        } else if (command == "freq") {
            int left = 0;
            int right = 0;
            long long value = 0;
            std::cin >> left >> right >> value;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.frequency(left, right, value) << '\n';
            }
        } else if (command == "get") {
            int index = 0;
            std::cin >> index;
            if (!valid(index)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << matrix.get(index) << '\n';
            }
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
