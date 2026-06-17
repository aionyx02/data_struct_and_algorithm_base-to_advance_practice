#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

// Known-wrong: range count sums whole boundary words, so bits outside
// [left, right] leak into the answer.
class BitsetSet {
public:
    explicit BitsetSet(int size)
        : size_(size),
          wordCount_((size + kWordBits - 1) / kWordBits),
          words_(std::make_unique<std::uint64_t[]>(
              static_cast<std::size_t>(wordCount_ < 1 ? 1 : wordCount_)
          )) {}

    void setInitial(int key, bool present) {
        if (present) {
            words_[wordIndex(key)] |= bitMask(key);
            ++total_;
        }
    }

    void set(int key) {
        const std::uint64_t mask = bitMask(key);
        if ((words_[wordIndex(key)] & mask) == 0) {
            words_[wordIndex(key)] |= mask;
            ++total_;
        }
    }

    void clear(int key) {
        const std::uint64_t mask = bitMask(key);
        if ((words_[wordIndex(key)] & mask) != 0) {
            words_[wordIndex(key)] &= ~mask;
            --total_;
        }
    }

    void flip(int key) {
        if (contains(key) == 0) {
            set(key);
        } else {
            clear(key);
        }
    }

    [[nodiscard]] int contains(int key) const {
        return (words_[wordIndex(key)] & bitMask(key)) != 0 ? 1 : 0;
    }

    [[nodiscard]] int count(int left, int right) const {
        int result = 0;
        for (int word = left / kWordBits; word <= right / kWordBits; ++word) {
            result += popcount(words_[static_cast<std::size_t>(word)]);
        }
        return result;
    }

    [[nodiscard]] int firstGreaterEqual(int key) const {
        for (int index = key; index < size_; ++index) {
            if (contains(index) != 0) {
                return index;
            }
        }
        return -1;
    }

    [[nodiscard]] int total() const { return total_; }

private:
    static constexpr int kWordBits = 64;

    [[nodiscard]] static int popcount(std::uint64_t value) {
        int count = 0;
        while (value != 0) {
            value &= value - 1;
            ++count;
        }
        return count;
    }

    [[nodiscard]] static std::size_t wordIndex(int key) {
        return static_cast<std::size_t>(key / kWordBits);
    }

    [[nodiscard]] static std::uint64_t bitMask(int key) {
        return std::uint64_t{1} << (key % kWordBits);
    }

    int size_;
    int wordCount_;
    int total_ = 0;
    std::unique_ptr<std::uint64_t[]> words_;
};

int main() {
    int size = 0;
    int q = 0;
    std::cin >> size >> q;
    BitsetSet set(size);
    for (int key = 0; key < size; ++key) {
        int present = 0;
        std::cin >> present;
        set.setInitial(key, present != 0);
    }

    auto valid = [&](int key) {
        return key >= 0 && key < size;
    };

    for (int operation = 0; operation < q; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "set") {
            int key = 0;
            std::cin >> key;
            if (!valid(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                set.set(key);
            }
        } else if (command == "clear") {
            int key = 0;
            std::cin >> key;
            if (!valid(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                set.clear(key);
            }
        } else if (command == "flip") {
            int key = 0;
            std::cin >> key;
            if (!valid(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                set.flip(key);
            }
        } else if (command == "contains") {
            int key = 0;
            std::cin >> key;
            if (!valid(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << set.contains(key) << '\n';
            }
        } else if (command == "count") {
            int left = 0;
            int right = 0;
            std::cin >> left >> right;
            if (!valid(left) || !valid(right) || left > right) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                std::cout << set.count(left, right) << '\n';
            }
        } else if (command == "first_ge") {
            int key = 0;
            std::cin >> key;
            if (!valid(key)) {
                std::cout << "OUT_OF_RANGE\n";
            } else {
                const int answer = set.firstGreaterEqual(key);
                if (answer < 0) {
                    std::cout << "NONE\n";
                } else {
                    std::cout << answer << '\n';
                }
            }
        } else if (command == "total") {
            std::cout << set.total() << '\n';
        } else if (command == "size") {
            std::cout << size << '\n';
        }
    }
}
