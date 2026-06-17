#include <cstdint>
#include <iostream>
#include <memory>
#include <string>

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
        std::uint64_t& word = words_[wordIndex(key)];
        if ((word & mask) == 0) {
            word |= mask;
            ++total_;
        }
    }

    void clear(int key) {
        const std::uint64_t mask = bitMask(key);
        std::uint64_t& word = words_[wordIndex(key)];
        if ((word & mask) != 0) {
            word &= ~mask;
            --total_;
        }
    }

    void flip(int key) {
        const std::uint64_t mask = bitMask(key);
        std::uint64_t& word = words_[wordIndex(key)];
        if ((word & mask) == 0) {
            word |= mask;
            ++total_;
        } else {
            word &= ~mask;
            --total_;
        }
    }

    [[nodiscard]] int contains(int key) const {
        return (words_[wordIndex(key)] & bitMask(key)) != 0 ? 1 : 0;
    }

    [[nodiscard]] int count(int left, int right) const {
        const int leftWord = left / kWordBits;
        const int rightWord = right / kWordBits;
        const int leftBit = left % kWordBits;
        const int rightBit = right % kWordBits;
        int result = 0;
        for (int word = leftWord; word <= rightWord; ++word) {
            std::uint64_t mask = ~std::uint64_t{0};
            if (word == leftWord) {
                mask &= ~std::uint64_t{0} << leftBit;
            }
            if (word == rightWord) {
                mask &= rightBit == kWordBits - 1
                            ? ~std::uint64_t{0}
                            : ((std::uint64_t{1} << (rightBit + 1)) - 1);
            }
            result += popcount(words_[static_cast<std::size_t>(word)] & mask);
        }
        return result;
    }

    [[nodiscard]] int firstGreaterEqual(int key) const {
        int word = key / kWordBits;
        int bit = key % kWordBits;
        while (word < wordCount_) {
            std::uint64_t bits = words_[static_cast<std::size_t>(word)];
            if (bit > 0) {
                bits &= ~std::uint64_t{0} << bit;
            }
            if (word == wordCount_ - 1) {
                bits &= lastWordMask();
            }
            if (bits != 0) {
                const int answer = word * kWordBits + trailingZeros(bits);
                return answer < size_ ? answer : -1;
            }
            ++word;
            bit = 0;
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

    [[nodiscard]] static int trailingZeros(std::uint64_t value) {
        int count = 0;
        while ((value & std::uint64_t{1}) == 0) {
            value >>= 1;
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

    [[nodiscard]] std::uint64_t lastWordMask() const {
        const int usedBits = size_ % kWordBits;
        if (usedBits == 0) {
            return ~std::uint64_t{0};
        }
        return (std::uint64_t{1} << usedBits) - 1;
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
