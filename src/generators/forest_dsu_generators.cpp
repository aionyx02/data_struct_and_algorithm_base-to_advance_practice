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
int forestRoot(const std::vector<int>& parent, int node) {
    while (parent[static_cast<std::size_t>(node)] != -1) {
        node = parent[static_cast<std::size_t>(node)];
    }
    return node;
}

int forestDepth(const std::vector<int>& parent, int node) {
    int depth = 0;
    while (parent[static_cast<std::size_t>(node)] != -1) {
        node = parent[static_cast<std::size_t>(node)];
        ++depth;
    }
    return depth;
}

void forestPreorder(
    const std::vector<int>& parent,
    int node,
    std::vector<int>& result
) {
    result.push_back(node);
    for (int child = 0; child < static_cast<int>(parent.size()); ++child) {
        if (parent[static_cast<std::size_t>(child)] == node) {
            forestPreorder(parent, child, result);
        }
    }
}

void forestPostorder(
    const std::vector<int>& parent,
    int node,
    std::vector<int>& result
) {
    for (int child = 0; child < static_cast<int>(parent.size()); ++child) {
        if (parent[static_cast<std::size_t>(child)] == node) {
            forestPostorder(parent, child, result);
        }
    }
    result.push_back(node);
}

int forestHeight(const std::vector<int>& parent, int node) {
    int best = 0;
    for (int child = 0; child < static_cast<int>(parent.size()); ++child) {
        if (parent[static_cast<std::size_t>(child)] == node) {
            best = std::max(best, forestHeight(parent, child));
        }
    }
    return best + 1;
}

std::vector<int> forestRoots(const std::vector<int>& parent) {
    std::vector<int> result;
    for (int node = 0; node < static_cast<int>(parent.size()); ++node) {
        if (parent[static_cast<std::size_t>(node)] == -1) {
            result.push_back(node);
        }
    }
    return result;
}

GeneratedCase generateParentArrayForest(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F41-parent-array-forest operation_limit is too small"
        );
    }
    const std::vector<int> parent = {-1, -1, 0, 2, 3, 1, 5, 5};
    std::ostringstream input;
    std::ostringstream output;
    input << parent.size() << ' ' << operationCount
          << "\n-1 -1 0 2 3 1 5 5\n"
          << "root 4\ndepth 4\nroots\n";
    output << "0\n3\n0 1\n";
    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0:
            case 1:
            case 2:
            case 3: {
                const int id = randomInt(random, -1, 8);
                const int kind = randomInt(random, 0, 3);
                const char* command = kind == 0
                    ? "parent"
                    : (kind == 1 ? "root" : (kind == 2 ? "depth" : "children"));
                input << command << ' ' << id << '\n';
                if (id < 0 || id >= static_cast<int>(parent.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else if (kind == 0) {
                    if (parent[static_cast<std::size_t>(id)] == -1) {
                        appendLine(output, "NONE");
                    } else {
                        appendLine(output, parent[static_cast<std::size_t>(id)]);
                    }
                } else if (kind == 1) {
                    appendLine(output, forestRoot(parent, id));
                } else if (kind == 2) {
                    appendLine(output, forestDepth(parent, id));
                } else {
                    std::vector<int> children;
                    for (int child = 0;
                         child < static_cast<int>(parent.size());
                         ++child) {
                        if (parent[static_cast<std::size_t>(child)] == id) {
                            children.push_back(child);
                        }
                    }
                    appendIntList(output, children);
                }
                break;
            }
            case 4:
                input << "roots\n";
                appendIntList(output, forestRoots(parent));
                break;
            default:
                input << "size\n";
                appendLine(output, static_cast<int>(parent.size()));
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateChildSiblingForest(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F42-child-sibling-forest operation_limit is too small"
        );
    }
    const int parent[8] = {-1, 0, 0, 1, 1, -1, 5, 6};
    const int firstChild[8] = {1, 3, -1, -1, -1, 6, 7, -1};
    const int nextSibling[8] = {5, 2, -1, 4, -1, -1, -1, -1};
    const int subtree[8] = {5, 3, 1, 1, 1, 3, 2, 1};
    std::ostringstream input;
    std::ostringstream output;
    input << "8 2 " << operationCount << "\n0 5\n";
    for (int id = 0; id < 8; ++id) {
        input << id << ' ' << parent[id] << ' ' << firstChild[id] << ' '
              << nextSibling[id] << '\n';
    }
    input << "children 0\nsubtree_size 0\nchildren 1\n";
    output << "1 2\n5\n3 4\n";
    for (int operation = 3; operation < operationCount; ++operation) {
        if (randomInt(random, 0, 6) == 0) {
            input << "roots\n";
            output << "0 5\n";
            continue;
        }
        const int id = randomInt(random, -1, 8);
        const int kind = randomInt(random, 0, 5);
        const char* command = kind == 0
            ? "parent"
            : (kind == 1
                ? "first_child"
                : (kind == 2
                    ? "next_sibling"
                    : (kind == 3
                        ? "children"
                        : (kind == 4 ? "root" : "subtree_size"))));
        input << command << ' ' << id << '\n';
        if (id < 0 || id >= 8) {
            appendLine(output, "OUT_OF_RANGE");
        } else if (kind <= 2) {
            const int link = kind == 0
                ? parent[id]
                : (kind == 1 ? firstChild[id] : nextSibling[id]);
            if (link < 0) appendLine(output, "NONE");
            else appendLine(output, link);
        } else if (kind == 3) {
            std::vector<int> children;
            for (int child = firstChild[id];
                 child >= 0;
                 child = nextSibling[child]) {
                children.push_back(child);
            }
            appendIntList(output, children);
        } else if (kind == 4) {
            int root = id;
            while (parent[root] >= 0) root = parent[root];
            appendLine(output, root);
        } else {
            appendLine(output, subtree[id]);
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateForestTraversalViews(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F43-forest-traversal-views operation_limit is too small"
        );
    }
    const std::vector<int> parent = {-1, -1, 0, 0, 1, 4, 1};
    const std::vector<int> roots = forestRoots(parent);
    std::vector<int> preorder;
    std::vector<int> postorder;
    for (int root : roots) {
        forestPreorder(parent, root, preorder);
        forestPostorder(parent, root, postorder);
    }
    std::vector<int> levelorder;
    std::deque<int> queue;
    for (int root : roots) queue.push_back(root);
    while (!queue.empty()) {
        const int node = queue.front();
        queue.pop_front();
        levelorder.push_back(node);
        for (int child = 0; child < static_cast<int>(parent.size()); ++child) {
            if (parent[static_cast<std::size_t>(child)] == node) {
                queue.push_back(child);
            }
        }
    }
    std::ostringstream input;
    std::ostringstream output;
    input << parent.size() << ' ' << operationCount
          << "\n-1 -1 0 0 1 4 1\n"
          << "preorder\npostorder\nlevelorder\n";
    appendIntList(output, preorder);
    appendIntList(output, postorder);
    appendIntList(output, levelorder);
    for (int operation = 3; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 7);
        if (kind == 0 || kind == 1 || kind == 2 || kind == 3) {
            const char* command = kind == 0
                ? "preorder"
                : (kind == 1
                    ? "postorder"
                    : (kind == 2 ? "levelorder" : "roots"));
            input << command << '\n';
            if (kind == 0) appendIntList(output, preorder);
            else if (kind == 1) appendIntList(output, postorder);
            else if (kind == 2) appendIntList(output, levelorder);
            else appendIntList(output, roots);
            continue;
        }
        const int id = randomInt(random, -1, 7);
        const char* command = kind == 4
            ? "tree_preorder"
            : (kind == 5
                ? "tree_postorder"
                : (kind == 6 ? "depth" : "height"));
        input << command << ' ' << id << '\n';
        if (id < 0 || id >= static_cast<int>(parent.size())) {
            appendLine(output, "OUT_OF_RANGE");
        } else if (kind == 4 || kind == 5) {
            std::vector<int> result;
            if (kind == 4) forestPreorder(parent, id, result);
            else forestPostorder(parent, id, result);
            appendIntList(output, result);
        } else if (kind == 6) {
            appendLine(output, forestDepth(parent, id));
        } else {
            appendLine(output, forestHeight(parent, id));
        }
    }
    return {input.str(), output.str()};
}

struct StressDsu {
    std::vector<int> parent;
    std::vector<int> sizes;
    int components = 0;

    explicit StressDsu(int count)
        : parent(static_cast<std::size_t>(count)),
          sizes(static_cast<std::size_t>(count), 1),
          components(count) {
        for (int index = 0; index < count; ++index) {
            parent[static_cast<std::size_t>(index)] = index;
        }
    }

    explicit StressDsu(std::vector<int> initialParent)
        : parent(std::move(initialParent)),
          sizes(parent.size(), 0),
          components(0) {
        for (int index = 0; index < static_cast<int>(parent.size()); ++index) {
            if (parent[static_cast<std::size_t>(index)] == index) {
                ++components;
            }
            ++sizes[static_cast<std::size_t>(findPlain(index))];
        }
    }

    int findPlain(int value) const {
        while (parent[static_cast<std::size_t>(value)] != value) {
            value = parent[static_cast<std::size_t>(value)];
        }
        return value;
    }

    int findCompressed(int value) {
        if (parent[static_cast<std::size_t>(value)] != value) {
            parent[static_cast<std::size_t>(value)] =
                findCompressed(parent[static_cast<std::size_t>(value)]);
        }
        return parent[static_cast<std::size_t>(value)];
    }

    int componentSizeSlow(int value) const {
        const int root = findPlain(value);
        int size = 0;
        for (int index = 0; index < static_cast<int>(parent.size()); ++index) {
            if (findPlain(index) == root) ++size;
        }
        return size;
    }
};

void appendDsuSnapshot(std::ostringstream& output, const StressDsu& dsu) {
    appendIntList(output, dsu.parent);
}

bool validDsuIndex(const StressDsu& dsu, int value) {
    return value >= 0 && value < static_cast<int>(dsu.parent.size());
}

GeneratedCase generateQuickUnionDisjointSet(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 9) {
        throw std::runtime_error(
            "F44-quick-union-disjoint-set operation_limit is too small"
        );
    }
    StressDsu dsu(7);
    std::ostringstream input;
    std::ostringstream output;
    input << "7 " << operationCount
          << "\nunite 0 1\nunite 2 3\nunite 4 5\n"
          << "unite 1 3\nunite 3 5\nsnapshot\nparent 5\nfind 5\ncomponents\n";
    const int unions[5][2] = {{0, 1}, {2, 3}, {4, 5}, {1, 3}, {3, 5}};
    for (const auto& edge : unions) {
        const int leftRoot = dsu.findPlain(edge[0]);
        const int rightRoot = dsu.findPlain(edge[1]);
        dsu.parent[static_cast<std::size_t>(rightRoot)] = leftRoot;
        --dsu.components;
        appendLine(output, leftRoot);
    }
    appendDsuSnapshot(output, dsu);
    appendLine(output, dsu.parent[5]);
    appendLine(output, dsu.findPlain(5));
    appendLine(output, dsu.components);

    for (int operation = 9; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 6);
        if (kind == 5) {
            input << "components\n";
            appendLine(output, dsu.components);
        } else if (kind == 6) {
            input << "snapshot\n";
            appendDsuSnapshot(output, dsu);
        } else if (kind <= 2) {
            const int value = randomInt(random, -1, 7);
            const char* command = kind == 0
                ? "find"
                : (kind == 1 ? "parent" : "component_size");
            input << command << ' ' << value << '\n';
            if (!validDsuIndex(dsu, value)) appendLine(output, "OUT_OF_RANGE");
            else if (kind == 0) appendLine(output, dsu.findPlain(value));
            else if (kind == 1) {
                appendLine(output, dsu.parent[static_cast<std::size_t>(value)]);
            } else {
                appendLine(output, dsu.componentSizeSlow(value));
            }
        } else {
            const int left = randomInt(random, -1, 7);
            const int right = randomInt(random, -1, 7);
            const bool unite = kind == 3;
            input << (unite ? "unite " : "connected ")
                  << left << ' ' << right << '\n';
            if (!validDsuIndex(dsu, left) || !validDsuIndex(dsu, right)) {
                appendLine(output, "OUT_OF_RANGE");
            } else {
                const int leftRoot = dsu.findPlain(left);
                const int rightRoot = dsu.findPlain(right);
                if (!unite) {
                    appendLine(
                        output,
                        leftRoot == rightRoot ? "true" : "false"
                    );
                } else if (leftRoot == rightRoot) {
                    appendLine(output, "ALREADY");
                } else {
                    dsu.parent[static_cast<std::size_t>(rightRoot)] = leftRoot;
                    --dsu.components;
                    appendLine(output, leftRoot);
                }
            }
        }
    }
    return {input.str(), output.str()};
}

int weightedUnion(StressDsu& dsu, int left, int right) {
    int leftRoot = dsu.findPlain(left);
    int rightRoot = dsu.findPlain(right);
    if (leftRoot == rightRoot) return -1;
    if (dsu.sizes[static_cast<std::size_t>(leftRoot)] <
        dsu.sizes[static_cast<std::size_t>(rightRoot)]) {
        std::swap(leftRoot, rightRoot);
    }
    dsu.parent[static_cast<std::size_t>(rightRoot)] = leftRoot;
    dsu.sizes[static_cast<std::size_t>(leftRoot)] +=
        dsu.sizes[static_cast<std::size_t>(rightRoot)];
    --dsu.components;
    return leftRoot;
}

GeneratedCase generateWeightedUnionDisjointSet(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 10) {
        throw std::runtime_error(
            "F45-weighted-union-disjoint-set operation_limit is too small"
        );
    }
    StressDsu dsu(8);
    std::ostringstream input;
    std::ostringstream output;
    input << "8 " << operationCount
          << "\nunite 0 1\nunite 2 3\nunite 2 4\nunite 0 2\n"
          << "parent 0\nparent 1\nfind 1\ncomponent_size 0\nsnapshot\ncomponents\n";
    appendLine(output, weightedUnion(dsu, 0, 1));
    appendLine(output, weightedUnion(dsu, 2, 3));
    appendLine(output, weightedUnion(dsu, 2, 4));
    appendLine(output, weightedUnion(dsu, 0, 2));
    appendLine(output, dsu.parent[0]);
    appendLine(output, dsu.parent[1]);
    appendLine(output, dsu.findPlain(1));
    appendLine(
        output,
        dsu.sizes[static_cast<std::size_t>(dsu.findPlain(0))]
    );
    appendDsuSnapshot(output, dsu);
    appendLine(output, dsu.components);

    for (int operation = 10; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 6);
        if (kind == 5) {
            input << "components\n";
            appendLine(output, dsu.components);
        } else if (kind == 6) {
            input << "snapshot\n";
            appendDsuSnapshot(output, dsu);
        } else if (kind <= 2) {
            const int value = randomInt(random, -1, 8);
            const char* command = kind == 0
                ? "find"
                : (kind == 1 ? "parent" : "component_size");
            input << command << ' ' << value << '\n';
            if (!validDsuIndex(dsu, value)) appendLine(output, "OUT_OF_RANGE");
            else if (kind == 0) appendLine(output, dsu.findPlain(value));
            else if (kind == 1) {
                appendLine(output, dsu.parent[static_cast<std::size_t>(value)]);
            } else {
                appendLine(
                    output,
                    dsu.sizes[static_cast<std::size_t>(dsu.findPlain(value))]
                );
            }
        } else {
            const int left = randomInt(random, -1, 8);
            const int right = randomInt(random, -1, 8);
            const bool unite = kind == 3;
            input << (unite ? "unite " : "connected ")
                  << left << ' ' << right << '\n';
            if (!validDsuIndex(dsu, left) || !validDsuIndex(dsu, right)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (!unite) {
                appendLine(
                    output,
                    dsu.findPlain(left) == dsu.findPlain(right)
                        ? "true"
                        : "false"
                );
            } else {
                const int root = weightedUnion(dsu, left, right);
                if (root < 0) appendLine(output, "ALREADY");
                else appendLine(output, root);
            }
        }
    }
    return {input.str(), output.str()};
}

int compressedWeightedUnion(StressDsu& dsu, int left, int right) {
    int leftRoot = dsu.findCompressed(left);
    int rightRoot = dsu.findCompressed(right);
    if (leftRoot == rightRoot) return -1;
    if (dsu.sizes[static_cast<std::size_t>(leftRoot)] <
        dsu.sizes[static_cast<std::size_t>(rightRoot)]) {
        std::swap(leftRoot, rightRoot);
    }
    dsu.parent[static_cast<std::size_t>(rightRoot)] = leftRoot;
    dsu.sizes[static_cast<std::size_t>(leftRoot)] +=
        dsu.sizes[static_cast<std::size_t>(rightRoot)];
    --dsu.components;
    return leftRoot;
}

GeneratedCase generatePathCompressionTrace(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F46-path-compression-trace operation_limit is too small"
        );
    }
    StressDsu dsu(std::vector<int>{0, 0, 1, 2, 3, 5, 5, 6});
    std::ostringstream input;
    std::ostringstream output;
    input << "8 " << operationCount
          << "\n0 0 1 2 3 5 5 6\nsnapshot\nfind 4\nsnapshot\n";
    appendDsuSnapshot(output, dsu);
    appendLine(output, dsu.findCompressed(4));
    appendDsuSnapshot(output, dsu);
    for (int operation = 3; operation < operationCount; ++operation) {
        const int kind = randomInt(random, 0, 6);
        if (kind == 5) {
            input << "components\n";
            appendLine(output, dsu.components);
        } else if (kind == 6) {
            input << "snapshot\n";
            appendDsuSnapshot(output, dsu);
        } else if (kind <= 2) {
            const int value = randomInt(random, -1, 8);
            const char* command = kind == 0
                ? "find"
                : (kind == 1 ? "parent" : "component_size");
            input << command << ' ' << value << '\n';
            if (!validDsuIndex(dsu, value)) appendLine(output, "OUT_OF_RANGE");
            else if (kind == 0) appendLine(output, dsu.findCompressed(value));
            else if (kind == 1) {
                appendLine(output, dsu.parent[static_cast<std::size_t>(value)]);
            } else {
                const int root = dsu.findCompressed(value);
                appendLine(
                    output,
                    dsu.sizes[static_cast<std::size_t>(root)]
                );
            }
        } else {
            const int left = randomInt(random, -1, 8);
            const int right = randomInt(random, -1, 8);
            const bool unite = kind == 3;
            input << (unite ? "unite " : "connected ")
                  << left << ' ' << right << '\n';
            if (!validDsuIndex(dsu, left) || !validDsuIndex(dsu, right)) {
                appendLine(output, "OUT_OF_RANGE");
            } else if (!unite) {
                appendLine(
                    output,
                    dsu.findCompressed(left) == dsu.findCompressed(right)
                        ? "true"
                        : "false"
                );
            } else {
                const int root = compressedWeightedUnion(dsu, left, right);
                if (root < 0) appendLine(output, "ALREADY");
                else appendLine(output, root);
            }
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerForestDsuGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F41-parent-array-forest", generateParentArrayForest);
    registry.emplace("F42-child-sibling-forest", generateChildSiblingForest);
    registry.emplace("F43-forest-traversal-views", generateForestTraversalViews);
    registry.emplace("F44-quick-union-disjoint-set", generateQuickUnionDisjointSet);
    registry.emplace("F45-weighted-union-disjoint-set", generateWeightedUnionDisjointSet);
    registry.emplace("F46-path-compression-trace", generatePathCompressionTrace);
}

}  // namespace judge::generators
