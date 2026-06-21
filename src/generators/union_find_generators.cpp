#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

// Picks a slot that is usually valid but occasionally out of range so the
// generated stream exercises OUT_OF_RANGE handling.
int randomIndex(std::mt19937_64& random, int count) {
    const int roll = randomInt(random, 0, 11);
    if (roll == 0) return -1;
    if (roll == 1) return count;
    return randomInt(random, 0, count - 1);
}

void appendParentSnapshot(
    std::ostringstream& output,
    const std::vector<int>& parent,
    int count
) {
    if (count == 0) {
        appendLine(output, "EMPTY");
        return;
    }
    for (int index = 0; index < count; ++index) {
        if (index != 0) output << ' ';
        output << parent[index];
    }
    output << '\n';
}

bool inRange(int value, int count) {
    return value >= 0 && value < count;
}

// ----- A29: union by rank with path compression ---------------------------

int findRankCompress(std::vector<int>& parent, int value) {
    if (parent[value] != value) {
        parent[value] = findRankCompress(parent, parent[value]);
    }
    return parent[value];
}

GeneratedCase generateUnionByRankPathCompression(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 16) {
        throw std::runtime_error(
            "A29-union-by-rank-path-compression operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count);
    std::vector<int> rankOf(count, 0);
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto unite = [&](int left, int right) {
        input << "unite " << left << ' ' << right << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        int leftRoot = findRankCompress(parent, left);
        int rightRoot = findRankCompress(parent, right);
        if (leftRoot == rightRoot) {
            appendLine(output, "ALREADY");
            return;
        }
        if (rankOf[leftRoot] < rankOf[rightRoot]) std::swap(leftRoot, rightRoot);
        parent[rightRoot] = leftRoot;
        if (rankOf[leftRoot] == rankOf[rightRoot]) ++rankOf[leftRoot];
        --components;
        appendLine(output, leftRoot);
    };

    // Pinned prefix: equal-rank tie-breaks build rank-2 trees, then a buried
    // node is found and inspected so compression bugs and size/rank confusion
    // both diverge.
    const int prefix[4][2] = {{0, 1}, {2, 3}, {4, 5}, {6, 7}};
    for (const auto& edge : prefix) unite(edge[0], edge[1]);
    unite(0, 2);
    unite(4, 6);
    input << "rank 0\n";
    appendLine(output, rankOf[findRankCompress(parent, 0)]);
    unite(0, 4);
    input << "snapshot\n";
    appendParentSnapshot(output, parent, count);
    input << "find 7\n";
    appendLine(output, findRankCompress(parent, 7));
    input << "parent 7\n";
    appendLine(output, parent[7]);
    input << "snapshot\n";
    appendParentSnapshot(output, parent, count);

    int emitted = 12;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 7);
        if (kind == 0) {
            unite(randomIndex(random, count), randomIndex(random, count));
        } else if (kind == 1) {
            const int value = randomIndex(random, count);
            input << "find " << value << '\n';
            appendLine(
                output,
                inRange(value, count)
                    ? std::to_string(findRankCompress(parent, value))
                    : "OUT_OF_RANGE"
            );
        } else if (kind == 2) {
            const int value = randomIndex(random, count);
            input << "parent " << value << '\n';
            appendLine(
                output,
                inRange(value, count) ? std::to_string(parent[value])
                                      : "OUT_OF_RANGE"
            );
        } else if (kind == 3) {
            const int value = randomIndex(random, count);
            input << "rank " << value << '\n';
            appendLine(
                output,
                inRange(value, count)
                    ? std::to_string(rankOf[findRankCompress(parent, value)])
                    : "OUT_OF_RANGE"
            );
        } else if (kind == 4) {
            const int left = randomIndex(random, count);
            const int right = randomIndex(random, count);
            input << "connected " << left << ' ' << right << '\n';
            if (!inRange(left, count) || !inRange(right, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(
                    output,
                    findRankCompress(parent, left) ==
                            findRankCompress(parent, right)
                        ? "true"
                        : "false"
                );
            }
        } else if (kind == 5) {
            input << "components\n";
            appendLine(output, components);
        } else {
            input << "snapshot\n";
            appendParentSnapshot(output, parent, count);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A30: disjoint set amortized cost -----------------------------------

int findCostCompress(std::vector<int>& parent, int value, int& edges) {
    edges = 0;
    int root = value;
    while (parent[root] != root) {
        root = parent[root];
        ++edges;
    }
    int current = value;
    while (parent[current] != root) {
        const int next = parent[current];
        parent[current] = root;
        current = next;
    }
    return root;
}

GeneratedCase generateDsuAmortizedCost(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 14) {
        throw std::runtime_error(
            "A30-dsu-amortized-cost operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count);
    std::vector<int> rankOf(count, 0);
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;
    int ignored = 0;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto unite = [&](int left, int right) {
        input << "unite " << left << ' ' << right << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        int leftRoot = findCostCompress(parent, left, ignored);
        int rightRoot = findCostCompress(parent, right, ignored);
        if (leftRoot == rightRoot) {
            appendLine(output, "ALREADY");
            return;
        }
        if (rankOf[leftRoot] < rankOf[rightRoot]) std::swap(leftRoot, rightRoot);
        parent[rightRoot] = leftRoot;
        if (rankOf[leftRoot] == rankOf[rightRoot]) ++rankOf[leftRoot];
        --components;
        appendLine(output, leftRoot);
    };
    auto find = [&](int value) {
        input << "find " << value << '\n';
        if (!inRange(value, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        int edges = 0;
        findCostCompress(parent, value, edges);
        appendLine(output, edges);
    };

    // Pinned prefix: build a depth-2 node, then find it twice so the cost falls
    // and a no-compression bug diverges.
    unite(0, 1);
    unite(2, 3);
    unite(0, 2);
    find(3);
    find(3);
    input << "parent 3\n";
    appendLine(output, parent[3]);
    unite(4, 5);
    unite(6, 7);
    unite(4, 6);
    find(7);

    int emitted = 10;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 6);
        if (kind <= 1) {
            unite(randomIndex(random, count), randomIndex(random, count));
        } else if (kind <= 3) {
            find(randomIndex(random, count));
        } else if (kind == 4) {
            const int value = randomIndex(random, count);
            input << "parent " << value << '\n';
            appendLine(
                output,
                inRange(value, count) ? std::to_string(parent[value])
                                      : "OUT_OF_RANGE"
            );
        } else if (kind == 5) {
            const int left = randomIndex(random, count);
            const int right = randomIndex(random, count);
            input << "connected " << left << ' ' << right << '\n';
            if (!inRange(left, count) || !inRange(right, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(
                    output,
                    findCostCompress(parent, left, ignored) ==
                            findCostCompress(parent, right, ignored)
                        ? "true"
                        : "false"
                );
            }
        } else {
            input << "components\n";
            appendLine(output, components);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A31: potential weighted disjoint set -------------------------------

int findWeighted(
    std::vector<int>& parent,
    std::vector<long long>& weight,
    int value
) {
    if (parent[value] == value) return value;
    const int root = findWeighted(parent, weight, parent[value]);
    weight[value] += weight[parent[value]];
    parent[value] = root;
    return root;
}

GeneratedCase generatePotentialWeightedDsu(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 14) {
        throw std::runtime_error(
            "A31-potential-weighted-dsu operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count);
    std::vector<long long> weight(count, 0);
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto relate = [&](int left, int right, long long delta) {
        input << "relate " << left << ' ' << right << ' ' << delta << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        const int leftRoot = findWeighted(parent, weight, left);
        const int rightRoot = findWeighted(parent, weight, right);
        if (leftRoot == rightRoot) {
            appendLine(
                output,
                weight[right] - weight[left] == delta ? "CONSISTENT"
                                                      : "CONFLICT"
            );
            return;
        }
        parent[rightRoot] = leftRoot;
        weight[rightRoot] = delta + weight[left] - weight[right];
        --components;
        appendLine(output, "MERGED");
    };
    auto diff = [&](int left, int right) {
        input << "diff " << left << ' ' << right << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        if (findWeighted(parent, weight, left) !=
            findWeighted(parent, weight, right)) {
            appendLine(output, "UNKNOWN");
        } else {
            appendLine(output, std::to_string(weight[right] - weight[left]));
        }
    };

    // Pinned prefix: bury a populated root so weighted compression and the
    // merge offset both get exercised.
    relate(0, 1, 2);
    relate(0, 2, 5);
    relate(3, 0, 4);
    diff(3, 1);
    diff(3, 2);
    relate(3, 1, 6);
    relate(3, 1, 7);

    int emitted = 7;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 6);
        if (kind <= 2) {
            relate(
                randomIndex(random, count),
                randomIndex(random, count),
                randomInt(random, -20, 20)
            );
        } else if (kind <= 4) {
            diff(randomIndex(random, count), randomIndex(random, count));
        } else if (kind == 5) {
            const int left = randomIndex(random, count);
            const int right = randomIndex(random, count);
            input << "connected " << left << ' ' << right << '\n';
            if (!inRange(left, count) || !inRange(right, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(
                    output,
                    findWeighted(parent, weight, left) ==
                            findWeighted(parent, weight, right)
                        ? "true"
                        : "false"
                );
            }
        } else {
            input << "components\n";
            appendLine(output, components);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A32: parity disjoint set -------------------------------------------

int findParity(std::vector<int>& parent, std::vector<int>& weight, int value) {
    if (parent[value] == value) return value;
    const int root = findParity(parent, weight, parent[value]);
    weight[value] ^= weight[parent[value]];
    parent[value] = root;
    return root;
}

GeneratedCase generateParityDsu(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 14) {
        throw std::runtime_error(
            "A32-parity-dsu operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count);
    std::vector<int> weight(count, 0);
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto relate = [&](int left, int right, int wanted) {
        input << "relate " << left << ' ' << right << ' '
              << (wanted == 1 ? "diff" : "same") << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        const int leftRoot = findParity(parent, weight, left);
        const int rightRoot = findParity(parent, weight, right);
        if (leftRoot == rightRoot) {
            appendLine(
                output,
                (weight[left] ^ weight[right]) == wanted ? "CONSISTENT"
                                                         : "CONFLICT"
            );
            return;
        }
        parent[rightRoot] = leftRoot;
        weight[rightRoot] = weight[left] ^ weight[right] ^ wanted;
        --components;
        appendLine(output, "MERGED");
    };
    auto query = [&](int left, int right) {
        input << "query " << left << ' ' << right << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        if (findParity(parent, weight, left) !=
            findParity(parent, weight, right)) {
            appendLine(output, "unknown");
        } else {
            appendLine(
                output,
                (weight[left] ^ weight[right]) == 0 ? "same" : "diff"
            );
        }
    };

    // Pinned prefix: bury a root attached with parity 1 so the xor-on-compress
    // bug diverges.
    relate(0, 1, 1);
    relate(0, 2, 1);
    relate(3, 0, 1);
    query(3, 1);
    query(3, 2);
    relate(3, 1, 1);
    relate(3, 1, 0);

    int emitted = 7;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 6);
        if (kind <= 2) {
            relate(
                randomIndex(random, count),
                randomIndex(random, count),
                randomInt(random, 0, 1)
            );
        } else if (kind <= 4) {
            query(randomIndex(random, count), randomIndex(random, count));
        } else if (kind == 5) {
            const int left = randomIndex(random, count);
            const int right = randomIndex(random, count);
            input << "connected " << left << ' ' << right << '\n';
            if (!inRange(left, count) || !inRange(right, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(
                    output,
                    findParity(parent, weight, left) ==
                            findParity(parent, weight, right)
                        ? "true"
                        : "false"
                );
            }
        } else {
            input << "components\n";
            appendLine(output, components);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A33: rollback disjoint set -----------------------------------------

int findPlainRb(const std::vector<int>& parent, int value) {
    while (parent[value] != value) value = parent[value];
    return value;
}

GeneratedCase generateRollbackDsu(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 16) {
        throw std::runtime_error(
            "A33-rollback-dsu operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count);
    std::vector<int> sizes(count, 1);
    for (int index = 0; index < count; ++index) parent[index] = index;
    int components = count;
    std::vector<int> undoChild;
    std::vector<int> undoGrew;
    std::vector<int> undoOldSize;
    std::vector<int> checkpoints;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto unite = [&](int left, int right) {
        input << "unite " << left << ' ' << right << '\n';
        if (!inRange(left, count) || !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        int leftRoot = findPlainRb(parent, left);
        int rightRoot = findPlainRb(parent, right);
        if (leftRoot == rightRoot) {
            appendLine(output, "ALREADY");
            return;
        }
        if (sizes[leftRoot] < sizes[rightRoot]) std::swap(leftRoot, rightRoot);
        undoChild.push_back(rightRoot);
        undoGrew.push_back(leftRoot);
        undoOldSize.push_back(sizes[leftRoot]);
        parent[rightRoot] = leftRoot;
        sizes[leftRoot] += sizes[rightRoot];
        --components;
        appendLine(output, leftRoot);
    };
    auto checkpoint = [&]() {
        input << "checkpoint\n";
        checkpoints.push_back(static_cast<int>(undoChild.size()));
        appendLine(output, static_cast<int>(checkpoints.size()));
    };
    auto rollback = [&]() {
        input << "rollback\n";
        if (checkpoints.empty()) {
            appendLine(output, "NONE");
            return;
        }
        const int target = checkpoints.back();
        checkpoints.pop_back();
        int undone = 0;
        while (static_cast<int>(undoChild.size()) > target) {
            const int child = undoChild.back();
            const int grew = undoGrew.back();
            const int oldSize = undoOldSize.back();
            undoChild.pop_back();
            undoGrew.pop_back();
            undoOldSize.pop_back();
            parent[child] = child;
            sizes[grew] = oldSize;
            ++components;
            ++undone;
        }
        appendLine(output, undone);
    };

    // Pinned prefix: multi-union rollback plus a connected query that would
    // compress (catching the compression bug).
    unite(0, 1);
    unite(1, 2);
    unite(3, 4);
    checkpoint();
    unite(0, 3);
    input << "connected 2 4\n";
    appendLine(
        output,
        findPlainRb(parent, 2) == findPlainRb(parent, 4) ? "true" : "false"
    );
    unite(0, 5);
    input << "snapshot\n";
    appendParentSnapshot(output, parent, count);
    rollback();
    input << "snapshot\n";
    appendParentSnapshot(output, parent, count);

    int emitted = 10;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 9);
        if (kind <= 4) {
            unite(randomIndex(random, count), randomIndex(random, count));
        } else if (kind == 5) {
            checkpoint();
        } else if (kind == 6) {
            rollback();
        } else if (kind == 7) {
            const int value = randomIndex(random, count);
            input << "find " << value << '\n';
            appendLine(
                output,
                inRange(value, count)
                    ? std::to_string(findPlainRb(parent, value))
                    : "OUT_OF_RANGE"
            );
        } else if (kind == 8) {
            const int value = randomIndex(random, count);
            input << "component_size " << value << '\n';
            appendLine(
                output,
                inRange(value, count)
                    ? std::to_string(sizes[findPlainRb(parent, value)])
                    : "OUT_OF_RANGE"
            );
        } else {
            input << "components\n";
            appendLine(output, components);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A34: persistent disjoint set ---------------------------------------

int findInVersion(const std::vector<int>& parent, int value) {
    while (parent[value] != value) value = parent[value];
    return value;
}

GeneratedCase generatePersistentDsu(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 14) {
        throw std::runtime_error(
            "A34-persistent-dsu operation_limit is too small"
        );
    }
    const int count = 6;
    std::vector<std::vector<int>> par;
    std::vector<std::vector<int>> sz;
    std::vector<int> comp;
    std::vector<int> base(count);
    for (int index = 0; index < count; ++index) base[index] = index;
    par.push_back(base);
    sz.push_back(std::vector<int>(count, 1));
    comp.push_back(count);

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto versions = [&]() { return static_cast<int>(par.size()); };
    auto validVersion = [&](int version) {
        return version >= 0 && version < versions();
    };
    auto unite = [&](int version, int left, int right) {
        input << "unite " << version << ' ' << left << ' ' << right << '\n';
        if (!validVersion(version) || !inRange(left, count) ||
            !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        int leftRoot = findInVersion(par[version], left);
        int rightRoot = findInVersion(par[version], right);
        if (leftRoot == rightRoot) {
            appendLine(output, "ALREADY");
            return;
        }
        std::vector<int> newPar = par[version];
        std::vector<int> newSz = sz[version];
        int newComp = comp[version];
        if (newSz[leftRoot] < newSz[rightRoot]) std::swap(leftRoot, rightRoot);
        newPar[rightRoot] = leftRoot;
        newSz[leftRoot] += newSz[rightRoot];
        --newComp;
        appendLine(output, versions());
        par.push_back(std::move(newPar));
        sz.push_back(std::move(newSz));
        comp.push_back(newComp);
    };
    auto connected = [&](int version, int left, int right) {
        input << "connected " << version << ' ' << left << ' ' << right << '\n';
        if (!validVersion(version) || !inRange(left, count) ||
            !inRange(right, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        appendLine(
            output,
            findInVersion(par[version], left) ==
                    findInVersion(par[version], right)
                ? "true"
                : "false"
        );
    };

    // Pinned prefix: fork several versions from version 0, then query the older
    // versions so a mutate-in-place bug diverges.
    unite(0, 0, 1);
    unite(0, 2, 3);
    connected(1, 0, 1);
    connected(2, 0, 1);
    unite(1, 1, 2);
    input << "snapshot 2\n";
    appendParentSnapshot(output, par[2], count);
    input << "snapshot 3\n";
    appendParentSnapshot(output, par[3], count);

    int emitted = 7;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 6);
        const int version = randomInt(random, 0, versions());  // may equal size
        if (kind <= 2) {
            unite(
                version,
                randomIndex(random, count),
                randomIndex(random, count)
            );
        } else if (kind == 3) {
            connected(
                version,
                randomIndex(random, count),
                randomIndex(random, count)
            );
        } else if (kind == 4) {
            const int value = randomIndex(random, count);
            input << "find " << version << ' ' << value << '\n';
            if (!validVersion(version) || !inRange(value, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(output, findInVersion(par[version], value));
            }
        } else if (kind == 5) {
            const int value = randomIndex(random, count);
            input << "component_size " << version << ' ' << value << '\n';
            if (!validVersion(version) || !inRange(value, count)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                appendLine(output, sz[version][findInVersion(par[version], value)]);
            }
        } else {
            input << "components " << version << '\n';
            appendLine(
                output,
                validVersion(version) ? std::to_string(comp[version])
                                      : "OUT_OF_RANGE"
            );
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A35: successor disjoint set ----------------------------------------

int findSuccessor(std::vector<int>& parent, int value) {
    if (parent[value] != value) parent[value] = findSuccessor(parent, parent[value]);
    return parent[value];
}

GeneratedCase generateSuccessorDsu(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 12) {
        throw std::runtime_error(
            "A35-successor-dsu operation_limit is too small"
        );
    }
    const int count = 8;
    std::vector<int> parent(count + 1);
    for (int index = 0; index <= count; ++index) parent[index] = index;
    int remaining = count;

    std::ostringstream input;
    std::ostringstream output;
    input << count << ' ' << operationCount << '\n';

    auto assign = [&](int value) {
        input << "assign " << value << '\n';
        if (!inRange(value, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        const int slot = findSuccessor(parent, value);
        if (slot == count) {
            appendLine(output, "NONE");
        } else {
            parent[slot] = slot + 1;
            --remaining;
            appendLine(output, slot);
        }
    };
    auto next = [&](int value) {
        input << "next " << value << '\n';
        if (!inRange(value, count)) {
            appendLine(output, "OUT_OF_RANGE");
            return;
        }
        const int slot = findSuccessor(parent, value);
        appendLine(output, slot == count ? "NONE" : std::to_string(slot));
    };

    // Pinned prefix: assigning the same start repeatedly must hand out
    // successive slots (catches the marks-x bug).
    assign(2);
    assign(2);
    assign(2);
    input << "used 2\n";
    appendLine(output, findSuccessor(parent, 2) != 2 ? 1 : 0);
    next(2);
    input << "snapshot\n";
    appendParentSnapshot(output, parent, count);

    int emitted = 6;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 6);
        if (kind <= 2) {
            assign(randomIndex(random, count));
        } else if (kind == 3) {
            next(randomIndex(random, count));
        } else if (kind == 4) {
            const int value = randomIndex(random, count);
            input << "used " << value << '\n';
            appendLine(
                output,
                inRange(value, count)
                    ? std::to_string(findSuccessor(parent, value) != value ? 1 : 0)
                    : "OUT_OF_RANGE"
            );
        } else if (kind == 5) {
            input << "remaining\n";
            appendLine(output, remaining);
        } else {
            input << "snapshot\n";
            appendParentSnapshot(output, parent, count);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

}  // namespace

void registerUnionFindGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace(
        "A29-union-by-rank-path-compression",
        generateUnionByRankPathCompression
    );
    registry.emplace("A30-dsu-amortized-cost", generateDsuAmortizedCost);
    registry.emplace("A31-potential-weighted-dsu", generatePotentialWeightedDsu);
    registry.emplace("A32-parity-dsu", generateParityDsu);
    registry.emplace("A33-rollback-dsu", generateRollbackDsu);
    registry.emplace("A34-persistent-dsu", generatePersistentDsu);
    registry.emplace("A35-successor-dsu", generateSuccessorDsu);
}

}  // namespace judge::generators
