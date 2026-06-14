#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace judge::generators {
namespace {
int stressModulo(int key, int modulus) {
    const int remainder = key % modulus;
    return remainder < 0 ? remainder + modulus : remainder;
}

GeneratedCase generateModuloHashFunction(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "F52-modulo-hash-function operation_limit is too small"
        );
    }
    const int bucketCount = 11;
    std::ostringstream input;
    std::ostringstream output;
    input << bucketCount << ' ' << operationCount
          << "\nhash -1\nsame_bucket -12 10\nsequence -3 4\nbucket_count\n";
    output << "10\ntrue\n8 9 10 0\n11\n";
    for (int operation = 4; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 3);
        if (kind == 0) {
            const int key = randomInt(random, -1000, 1000);
            input << "hash " << key << '\n';
            appendLine(output, stressModulo(key, bucketCount));
        } else if (kind == 1) {
            const int left = randomInt(random, -1000, 1000);
            const int right = randomInt(random, -1000, 1000);
            input << "same_bucket " << left << ' ' << right << '\n';
            appendLine(
                output,
                stressModulo(left, bucketCount) ==
                        stressModulo(right, bucketCount)
                    ? "true"
                    : "false"
            );
        } else if (kind == 2) {
            const int start = randomInt(random, -1000, 1000);
            const int count = randomInt(random, 0, 8);
            input << "sequence " << start << ' ' << count << '\n';
            std::vector<int> result;
            for (int index = 0; index < count; ++index) {
                result.push_back(stressModulo(start + index, bucketCount));
            }
            appendIntList(output, result);
        } else {
            input << "bucket_count\n";
            appendLine(output, bucketCount);
        }
    }
    return {input.str(), output.str()};
}

enum class StressProbeKind {
    linear,
    quadratic,
    doubleHash
};

struct StressProbeTable {
    int capacity;
    StressProbeKind kind;
    std::vector<int> keys;
    std::vector<unsigned char> used;
    int size = 0;

    StressProbeTable(int tableCapacity, StressProbeKind probeKind)
        : capacity(tableCapacity),
          kind(probeKind),
          keys(static_cast<std::size_t>(tableCapacity)),
          used(static_cast<std::size_t>(tableCapacity), 0) {}

    int position(int key, int probe) const {
        const int start = stressModulo(key, capacity);
        if (kind == StressProbeKind::linear) {
            return (start + probe) % capacity;
        }
        if (kind == StressProbeKind::quadratic) {
            return static_cast<int>(
                (start + 1LL * probe * probe) % capacity
            );
        }
        const int step = 1 + stressModulo(key, capacity - 1);
        return static_cast<int>(
            (start + 1LL * probe * step) % capacity
        );
    }

    void locate(int key, int& found, int& empty, int& probes) const {
        found = -1;
        empty = -1;
        probes = 0;
        for (int probe = 0; probe < capacity; ++probe) {
            const int slot = position(key, probe);
            ++probes;
            if (!used[static_cast<std::size_t>(slot)]) {
                empty = slot;
                return;
            }
            if (keys[static_cast<std::size_t>(slot)] == key) {
                found = slot;
                return;
            }
        }
    }

    void clear() {
        std::fill(used.begin(), used.end(), 0);
        size = 0;
    }
};

void appendProbeTable(
    std::ostringstream& output,
    const StressProbeTable& table
) {
    for (int slot = 0; slot < table.capacity; ++slot) {
        if (slot != 0) output << ' ';
        if (table.used[static_cast<std::size_t>(slot)]) {
            output << table.keys[static_cast<std::size_t>(slot)];
        } else {
            output << '.';
        }
    }
    output << '\n';
}

void applyProbeInsert(
    std::ostringstream& output,
    StressProbeTable& table,
    int key
) {
    int found = -1;
    int empty = -1;
    int probes = 0;
    table.locate(key, found, empty, probes);
    if (found >= 0) {
        appendLine(output, "DUPLICATE");
    } else if (empty < 0) {
        appendLine(output, "FULL");
    } else {
        table.used[static_cast<std::size_t>(empty)] = 1;
        table.keys[static_cast<std::size_t>(empty)] = key;
        ++table.size;
        appendLine(output, empty);
    }
}

GeneratedCase generateProbingHashTable(
    std::mt19937_64& random,
    int operationCount,
    StressProbeKind kind
) {
    if (operationCount < 7) {
        throw std::runtime_error(
            "probing hash table operation_limit is too small"
        );
    }
    StressProbeTable table(7, kind);
    std::ostringstream input;
    std::ostringstream output;
    if (kind == StressProbeKind::linear) {
        input << "7 " << operationCount
              << "\ninsert 6\ninsert 13\ninsert 20\ntable\nslot 20\n"
              << "probe_count 27\nsize\n";
    } else {
        input << "7 " << operationCount
              << "\ninsert 1\ninsert 8\ninsert 15\ntable\nslot 15\n"
              << "probe_count 22\nsize\n";
    }
    if (kind == StressProbeKind::linear) {
        applyProbeInsert(output, table, 6);
        applyProbeInsert(output, table, 13);
        applyProbeInsert(output, table, 20);
        appendProbeTable(output, table);
        appendLine(output, 1);
        int found = 0;
        int empty = 0;
        int probes = 0;
        table.locate(27, found, empty, probes);
        appendLine(output, probes);
    } else {
        applyProbeInsert(output, table, 1);
        applyProbeInsert(output, table, 8);
        applyProbeInsert(output, table, 15);
        appendProbeTable(output, table);
        int found = 0;
        int empty = 0;
        int probes = 0;
        table.locate(15, found, empty, probes);
        appendLine(output, found);
        table.locate(22, found, empty, probes);
        appendLine(output, probes);
    }
    appendLine(output, table.size);

    for (int operation = 7; operation < operationCount; ++operation) {
        const int command = randomInt(random, 0, 7);
        if (command <= 3) {
            const int key = randomInt(random, -30, 30);
            const char* name = command == 0
                ? "insert"
                : (command == 1
                    ? "contains"
                    : (command == 2 ? "slot" : "probe_count"));
            input << name << ' ' << key << '\n';
            int found = -1;
            int empty = -1;
            int probes = 0;
            table.locate(key, found, empty, probes);
            if (command == 0) {
                applyProbeInsert(output, table, key);
            } else if (command == 1) {
                appendLine(output, found >= 0 ? "true" : "false");
            } else if (command == 2) {
                if (found < 0) appendLine(output, "NOT_FOUND");
                else appendLine(output, found);
            } else {
                appendLine(output, probes);
            }
        } else if (command == 4) {
            input << "table\n";
            appendProbeTable(output, table);
        } else if (command == 5) {
            input << "size\n";
            appendLine(output, table.size);
        } else if (command == 6) {
            input << "capacity\n";
            appendLine(output, table.capacity);
        } else {
            input << "clear\n";
            table.clear();
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateLinearProbingHashTable(
    std::mt19937_64& random,
    int operationCount
) {
    return generateProbingHashTable(
        random,
        operationCount,
        StressProbeKind::linear
    );
}

GeneratedCase generateQuadraticProbingHashTable(
    std::mt19937_64& random,
    int operationCount
) {
    return generateProbingHashTable(
        random,
        operationCount,
        StressProbeKind::quadratic
    );
}

GeneratedCase generateDoubleHashingTable(
    std::mt19937_64& random,
    int operationCount
) {
    return generateProbingHashTable(
        random,
        operationCount,
        StressProbeKind::doubleHash
    );
}

struct StressTombstoneTable {
    int capacity;
    std::vector<int> keys;
    std::vector<unsigned char> states;
    int size = 0;
    int tombstones = 0;

    explicit StressTombstoneTable(int tableCapacity)
        : capacity(tableCapacity),
          keys(static_cast<std::size_t>(tableCapacity)),
          states(static_cast<std::size_t>(tableCapacity), 0) {}

    void locate(
        int key,
        int& found,
        int& firstTombstone,
        int& empty
    ) const {
        found = -1;
        firstTombstone = -1;
        empty = -1;
        const int start = stressModulo(key, capacity);
        for (int probe = 0; probe < capacity; ++probe) {
            const int slot = (start + probe) % capacity;
            const unsigned char state = states[static_cast<std::size_t>(slot)];
            if (state == 0) {
                empty = slot;
                return;
            }
            if (state == 1) {
                if (firstTombstone < 0) firstTombstone = slot;
            } else if (keys[static_cast<std::size_t>(slot)] == key) {
                found = slot;
                return;
            }
        }
    }

    void clear() {
        std::fill(states.begin(), states.end(), 0);
        size = 0;
        tombstones = 0;
    }
};

void appendTombstoneTable(
    std::ostringstream& output,
    const StressTombstoneTable& table
) {
    for (int slot = 0; slot < table.capacity; ++slot) {
        if (slot != 0) output << ' ';
        const unsigned char state = table.states[static_cast<std::size_t>(slot)];
        if (state == 0) output << '.';
        else if (state == 1) output << '#';
        else output << table.keys[static_cast<std::size_t>(slot)];
    }
    output << '\n';
}

GeneratedCase generateTombstoneHashTable(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F56-tombstone-hash-table operation_limit is too small"
        );
    }
    StressTombstoneTable table(7);
    std::ostringstream input;
    std::ostringstream output;
    input << "7 " << operationCount
          << "\ninsert 1\ninsert 8\ninsert 15\nerase 8\ncontains 15\n"
          << "slot 15\ninsert 22\ntable\n";
    auto insert = [&](int key) {
        int found = -1;
        int firstTombstone = -1;
        int empty = -1;
        table.locate(key, found, firstTombstone, empty);
        if (found >= 0) {
            appendLine(output, "DUPLICATE");
        } else {
            const int target =
                firstTombstone >= 0 ? firstTombstone : empty;
            if (target < 0) {
                appendLine(output, "FULL");
            } else {
                if (table.states[static_cast<std::size_t>(target)] == 1) {
                    --table.tombstones;
                }
                table.states[static_cast<std::size_t>(target)] = 2;
                table.keys[static_cast<std::size_t>(target)] = key;
                ++table.size;
                appendLine(output, target);
            }
        }
    };
    insert(1);
    insert(8);
    insert(15);
    table.states[2] = 1;
    --table.size;
    ++table.tombstones;
    output << "REMOVED\ntrue\n3\n";
    insert(22);
    appendTombstoneTable(output, table);

    for (int operation = 8; operation < operationCount; ++operation) {
        const int command = randomInt(random, 0, 8);
        if (command <= 3) {
            const int key = randomInt(random, -25, 25);
            const char* name = command == 0
                ? "insert"
                : (command == 1
                    ? "erase"
                    : (command == 2 ? "contains" : "slot"));
            input << name << ' ' << key << '\n';
            int found = -1;
            int firstTombstone = -1;
            int empty = -1;
            table.locate(key, found, firstTombstone, empty);
            if (command == 0) {
                insert(key);
            } else if (command == 1) {
                if (found < 0) {
                    appendLine(output, "NOT_FOUND");
                } else {
                    table.states[static_cast<std::size_t>(found)] = 1;
                    --table.size;
                    ++table.tombstones;
                    appendLine(output, "REMOVED");
                }
            } else if (command == 2) {
                appendLine(output, found >= 0 ? "true" : "false");
            } else if (found < 0) {
                appendLine(output, "NOT_FOUND");
            } else {
                appendLine(output, found);
            }
        } else if (command == 4) {
            const int slot = randomInt(random, -1, table.capacity);
            input << "state " << slot << '\n';
            if (slot < 0 || slot >= table.capacity) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                const unsigned char state =
                    table.states[static_cast<std::size_t>(slot)];
                if (state == 0) appendLine(output, "EMPTY");
                else if (state == 1) appendLine(output, "TOMBSTONE");
                else {
                    output << "OCCUPIED "
                           << table.keys[static_cast<std::size_t>(slot)]
                           << '\n';
                }
            }
        } else if (command == 5) {
            input << "table\n";
            appendTombstoneTable(output, table);
        } else if (command == 6) {
            input << "size\n";
            appendLine(output, table.size);
        } else if (command == 7) {
            input << "tombstones\n";
            appendLine(output, table.tombstones);
        } else {
            input << "clear\n";
            table.clear();
        }
    }
    return {input.str(), output.str()};
}

struct StressPoolHashNode {
    int key = 0;
    bool active = false;
};

GeneratedCase generateNodePoolChainedHashTable(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F57-node-pool-chained-hash-table operation_limit is too small"
        );
    }
    const int bucketCount = 5;
    const int capacity = 8;
    std::vector<StressPoolHashNode> nodes(
        static_cast<std::size_t>(capacity)
    );
    int size = 0;
    auto findKey = [&](int key) {
        for (int slot = 0; slot < capacity; ++slot) {
            if (nodes[static_cast<std::size_t>(slot)].active &&
                nodes[static_cast<std::size_t>(slot)].key == key) {
                return slot;
            }
        }
        return -1;
    };
    auto insertKey = [&](int key) {
        int slot = 0;
        while (nodes[static_cast<std::size_t>(slot)].active) ++slot;
        nodes[static_cast<std::size_t>(slot)] = {key, true};
        ++size;
        return slot;
    };
    auto bucketEntries = [&](int bucket) {
        std::vector<std::pair<int, int>> entries;
        for (int slot = 0; slot < capacity; ++slot) {
            const StressPoolHashNode& node =
                nodes[static_cast<std::size_t>(slot)];
            if (node.active &&
                stressModulo(node.key, bucketCount) == bucket) {
                entries.emplace_back(node.key, slot);
            }
        }
        std::sort(entries.begin(), entries.end());
        return entries;
    };
    std::ostringstream input;
    std::ostringstream output;
    input << "5 8 " << operationCount
          << "\ninsert 1\ninsert 6\ninsert 11\ninsert -4\nbucket 1\n"
          << "bucket_slots 1\nerase 6\ninsert 16\n";
    appendLine(output, insertKey(1));
    appendLine(output, insertKey(6));
    appendLine(output, insertKey(11));
    appendLine(output, insertKey(-4));
    output << "-4 1 6 11\n3 0 1 2\n";
    nodes[1].active = false;
    --size;
    appendLine(output, 1);
    appendLine(output, insertKey(16));

    for (int operation = 8; operation < operationCount; ++operation) {
        const int command = randomInt(random, 0, 8);
        if (command <= 3) {
            const int key = randomInt(random, -25, 25);
            const char* name = command == 0
                ? "insert"
                : (command == 1
                    ? "erase"
                    : (command == 2 ? "contains" : "slot"));
            input << name << ' ' << key << '\n';
            const int slot = findKey(key);
            if (command == 0) {
                if (slot >= 0) appendLine(output, "DUPLICATE");
                else if (size == capacity) appendLine(output, "FULL");
                else appendLine(output, insertKey(key));
            } else if (command == 1) {
                if (slot < 0) appendLine(output, "NOT_FOUND");
                else {
                    nodes[static_cast<std::size_t>(slot)].active = false;
                    --size;
                    appendLine(output, slot);
                }
            } else if (command == 2) {
                appendLine(output, slot >= 0 ? "true" : "false");
            } else if (slot < 0) {
                appendLine(output, "NOT_FOUND");
            } else {
                appendLine(output, slot);
            }
        } else if (command <= 6) {
            const int bucket = randomInt(random, -1, bucketCount);
            const char* name = command == 4
                ? "bucket"
                : (command == 5 ? "bucket_slots" : "bucket_size");
            input << name << ' ' << bucket << '\n';
            if (bucket < 0 || bucket >= bucketCount) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                const auto entries = bucketEntries(bucket);
                if (command == 6) {
                    appendLine(output, static_cast<int>(entries.size()));
                } else {
                    std::vector<int> values;
                    for (const auto& entry : entries) {
                        values.push_back(command == 4 ? entry.first : entry.second);
                    }
                    appendIntList(output, values);
                }
            }
        } else if (command == 7) {
            if (randomInt(random, 0, 1) == 0) {
                input << "size\n";
                appendLine(output, size);
            } else {
                input << "free_slots\n";
                appendLine(output, capacity - size);
            }
        } else {
            input << "clear\n";
            for (StressPoolHashNode& node : nodes) node.active = false;
            size = 0;
        }
    }
    return {input.str(), output.str()};
}

struct StressRehashTable {
    int initialCapacity;
    int capacity;
    std::vector<int> keys;
    std::vector<unsigned char> used;
    int size = 0;

    explicit StressRehashTable(int tableCapacity)
        : initialCapacity(tableCapacity),
          capacity(tableCapacity),
          keys(static_cast<std::size_t>(tableCapacity)),
          used(static_cast<std::size_t>(tableCapacity), 0) {}

    int find(int key) const {
        int slot = stressModulo(key, capacity);
        for (int probe = 0; probe < capacity; ++probe) {
            if (!used[static_cast<std::size_t>(slot)]) return -1;
            if (keys[static_cast<std::size_t>(slot)] == key) return slot;
            slot = (slot + 1) % capacity;
        }
        return -1;
    }

    int insertRaw(int key) {
        int slot = stressModulo(key, capacity);
        while (used[static_cast<std::size_t>(slot)]) {
            slot = (slot + 1) % capacity;
        }
        used[static_cast<std::size_t>(slot)] = 1;
        keys[static_cast<std::size_t>(slot)] = key;
        return slot;
    }

    int insert(int key) {
        if ((size + 1) * 2 > capacity) {
            const std::vector<int> oldKeys = keys;
            const std::vector<unsigned char> oldUsed = used;
            const int oldCapacity = capacity;
            capacity = capacity * 2 + 1;
            keys.assign(static_cast<std::size_t>(capacity), 0);
            used.assign(static_cast<std::size_t>(capacity), 0);
            for (int slot = 0; slot < oldCapacity; ++slot) {
                if (oldUsed[static_cast<std::size_t>(slot)]) {
                    insertRaw(oldKeys[static_cast<std::size_t>(slot)]);
                }
            }
        }
        const int slot = insertRaw(key);
        ++size;
        return slot;
    }

    void clear() {
        capacity = initialCapacity;
        keys.assign(static_cast<std::size_t>(capacity), 0);
        used.assign(static_cast<std::size_t>(capacity), 0);
        size = 0;
    }
};

void appendRehashTable(
    std::ostringstream& output,
    const StressRehashTable& table
) {
    for (int slot = 0; slot < table.capacity; ++slot) {
        if (slot != 0) output << ' ';
        if (table.used[static_cast<std::size_t>(slot)]) {
            output << table.keys[static_cast<std::size_t>(slot)];
        } else {
            output << '.';
        }
    }
    output << '\n';
}

GeneratedCase generateRehashingLinearProbingTable(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 8) {
        throw std::runtime_error(
            "F58-rehashing-linear-probing-table operation_limit is too small"
        );
    }
    StressRehashTable table(3);
    std::ostringstream input;
    std::ostringstream output;
    input << "3 " << operationCount
          << "\ninsert -1\ninsert 2\ncapacity\nslot -1\nslot 2\n"
          << "table\nload_percent\nsize\n";
    appendLine(output, table.insert(-1));
    appendLine(output, table.insert(2));
    appendLine(output, table.capacity);
    appendLine(output, table.find(-1));
    appendLine(output, table.find(2));
    appendRehashTable(output, table);
    appendLine(output, table.size * 100 / table.capacity);
    appendLine(output, table.size);

    for (int operation = 8; operation < operationCount; ++operation) {
        const int command = randomInt(random, 0, 7);
        if (command <= 2) {
            const int key = randomInt(random, -40, 40);
            const char* name =
                command == 0 ? "insert" : (command == 1 ? "contains" : "slot");
            input << name << ' ' << key << '\n';
            const int slot = table.find(key);
            if (command == 0) {
                if (slot >= 0) appendLine(output, "DUPLICATE");
                else appendLine(output, table.insert(key));
            } else if (command == 1) {
                appendLine(output, slot >= 0 ? "true" : "false");
            } else if (slot < 0) {
                appendLine(output, "NOT_FOUND");
            } else {
                appendLine(output, slot);
            }
        } else if (command == 3) {
            input << "capacity\n";
            appendLine(output, table.capacity);
        } else if (command == 4) {
            input << "size\n";
            appendLine(output, table.size);
        } else if (command == 5) {
            input << "load_percent\n";
            appendLine(output, table.size * 100 / table.capacity);
        } else if (command == 6) {
            input << "table\n";
            appendRehashTable(output, table);
        } else {
            input << "clear\n";
            table.clear();
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerHashingGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F52-modulo-hash-function", generateModuloHashFunction);
    registry.emplace("F53-linear-probing-hash-table", generateLinearProbingHashTable);
    registry.emplace("F54-quadratic-probing-hash-table", generateQuadraticProbingHashTable);
    registry.emplace("F55-double-hashing-table", generateDoubleHashingTable);
    registry.emplace("F56-tombstone-hash-table", generateTombstoneHashTable);
    registry.emplace("F57-node-pool-chained-hash-table", generateNodePoolChainedHashTable);
    registry.emplace("F58-rehashing-linear-probing-table", generateRehashingLinearProbingTable);
}

}  // namespace judge::generators
