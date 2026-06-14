#pragma once

#include <random>
#include <sstream>
#include <string>
#include <vector>

namespace judge::generators {

// Shared helpers used by every domain's case generators. Defined inline so they
// can live in a single header without violating the one-definition rule.

inline int randomInt(std::mt19937_64& random, int minimum, int maximum) {
    return std::uniform_int_distribution<int>(minimum, maximum)(random);
}

inline int randomValue(std::mt19937_64& random) {
    return randomInt(random, -1000, 1000);
}

inline void appendLine(std::ostringstream& output, const std::string& value) {
    output << value << '\n';
}

inline void appendLine(std::ostringstream& output, int value) {
    output << value << '\n';
}

// Prints a space-separated integer list, or EMPTY when the list has no values.
inline void appendIntList(
    std::ostringstream& output,
    const std::vector<int>& values
) {
    if (values.empty()) {
        appendLine(output, "EMPTY");
        return;
    }
    for (std::size_t index = 0; index < values.size(); ++index) {
        if (index != 0) output << ' ';
        output << values[index];
    }
    output << '\n';
}

}  // namespace judge::generators
