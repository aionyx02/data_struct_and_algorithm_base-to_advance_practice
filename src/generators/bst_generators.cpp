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
struct StressBstNode {
    int id = 0;
    int key = 0;
    int value = 0;
    int parent = -1;
    int left = -1;
    int right = -1;
    bool alive = true;
};

struct StressBst {
    std::vector<StressBstNode> nodes;
    int root = -1;

    int findKey(int key) const {
        int current = root;
        while (current >= 0 && nodes[static_cast<std::size_t>(current)].key != key) {
            const StressBstNode& node =
                nodes[static_cast<std::size_t>(current)];
            current = key < node.key ? node.left : node.right;
        }
        return current;
    }

    int findId(int id) const {
        for (std::size_t index = 0; index < nodes.size(); ++index) {
            if (nodes[index].alive && nodes[index].id == id) {
                return static_cast<int>(index);
            }
        }
        return -1;
    }

    bool insert(int id, int key, int value = 0) {
        if (findId(id) >= 0 || findKey(key) >= 0) return false;
        int parent = -1;
        int current = root;
        while (current >= 0) {
            parent = current;
            const StressBstNode& node =
                nodes[static_cast<std::size_t>(current)];
            current = key < node.key ? node.left : node.right;
        }
        const int index = static_cast<int>(nodes.size());
        nodes.push_back({id, key, value, parent, -1, -1, true});
        if (parent < 0) root = index;
        else if (key < nodes[static_cast<std::size_t>(parent)].key) {
            nodes[static_cast<std::size_t>(parent)].left = index;
        } else {
            nodes[static_cast<std::size_t>(parent)].right = index;
        }
        return true;
    }

    int minimum(int index) const {
        while (nodes[static_cast<std::size_t>(index)].left >= 0) {
            index = nodes[static_cast<std::size_t>(index)].left;
        }
        return index;
    }

    int maximum(int index) const {
        while (nodes[static_cast<std::size_t>(index)].right >= 0) {
            index = nodes[static_cast<std::size_t>(index)].right;
        }
        return index;
    }

    int predecessor(int index) const {
        if (nodes[static_cast<std::size_t>(index)].left >= 0) {
            return maximum(nodes[static_cast<std::size_t>(index)].left);
        }
        int parent = nodes[static_cast<std::size_t>(index)].parent;
        while (parent >= 0 &&
               nodes[static_cast<std::size_t>(parent)].left == index) {
            index = parent;
            parent = nodes[static_cast<std::size_t>(parent)].parent;
        }
        return parent;
    }

    int successor(int index) const {
        if (nodes[static_cast<std::size_t>(index)].right >= 0) {
            return minimum(nodes[static_cast<std::size_t>(index)].right);
        }
        int parent = nodes[static_cast<std::size_t>(index)].parent;
        while (parent >= 0 &&
               nodes[static_cast<std::size_t>(parent)].right == index) {
            index = parent;
            parent = nodes[static_cast<std::size_t>(parent)].parent;
        }
        return parent;
    }

    void transplant(int oldIndex, int newIndex) {
        const int parent =
            nodes[static_cast<std::size_t>(oldIndex)].parent;
        if (parent < 0) root = newIndex;
        else if (nodes[static_cast<std::size_t>(parent)].left == oldIndex) {
            nodes[static_cast<std::size_t>(parent)].left = newIndex;
        } else {
            nodes[static_cast<std::size_t>(parent)].right = newIndex;
        }
        if (newIndex >= 0) {
            nodes[static_cast<std::size_t>(newIndex)].parent = parent;
        }
    }

    int eraseKey(int key) {
        const int target = findKey(key);
        if (target < 0) return -1;
        StressBstNode& targetNode =
            nodes[static_cast<std::size_t>(target)];
        if (targetNode.left < 0) {
            transplant(target, targetNode.right);
        } else if (targetNode.right < 0) {
            transplant(target, targetNode.left);
        } else {
            const int replacement = minimum(targetNode.right);
            if (nodes[static_cast<std::size_t>(replacement)].parent != target) {
                const int replacementRight =
                    nodes[static_cast<std::size_t>(replacement)].right;
                transplant(replacement, replacementRight);
                nodes[static_cast<std::size_t>(replacement)].right =
                    targetNode.right;
                nodes[static_cast<std::size_t>(targetNode.right)].parent =
                    replacement;
            }
            transplant(target, replacement);
            nodes[static_cast<std::size_t>(replacement)].left =
                targetNode.left;
            nodes[static_cast<std::size_t>(targetNode.left)].parent =
                replacement;
        }
        targetNode.alive = false;
        targetNode.parent = -1;
        targetNode.left = -1;
        targetNode.right = -1;
        return target;
    }

    int size() const {
        int result = 0;
        for (const StressBstNode& node : nodes) {
            if (node.alive) ++result;
        }
        return result;
    }

    void inorder(int index, std::vector<int>& result, bool identifiers) const {
        if (index < 0) return;
        const StressBstNode& node =
            nodes[static_cast<std::size_t>(index)];
        inorder(node.left, result, identifiers);
        result.push_back(identifiers ? node.id : node.key);
        inorder(node.right, result, identifiers);
    }

    std::vector<int> inorder(bool identifiers = false) const {
        std::vector<int> result;
        inorder(root, result, identifiers);
        return result;
    }

    void clear() {
        nodes.clear();
        root = -1;
    }
};

GeneratedCase generateBstInsertSearch(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 9) {
        throw std::runtime_error(
            "F35-bst-insert-search operation_limit is too small"
        );
    }
    StressBst tree;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    const int value50 = randomValue(random);
    const int value30 = randomValue(random);
    const int value70 = randomValue(random);
    tree.insert(50, 50, value50);
    tree.insert(30, 30, value30);
    tree.insert(70, 70, value70);
    input << "insert 50 " << value50 << "\ninsert 30 " << value30
          << "\ninsert 70 " << value70 << "\ninsert 30 "
          << randomValue(random)
          << "\nvalue 30\nparent 30\ncontains 70\nroot\nsize\n";
    output << "DUPLICATE\n" << value30 << "\n50\ntrue\n50\n3\n";

    for (int operation = 9; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int key = randomInt(random, -20, 20);
                const int value = randomValue(random);
                input << "insert " << key << ' ' << value << '\n';
                if (!tree.insert(key, key, value)) {
                    appendLine(output, "DUPLICATE");
                }
                break;
            }
            case 1: {
                const int key = randomInt(random, -20, 20);
                input << "contains " << key << '\n';
                appendLine(
                    output,
                    tree.findKey(key) >= 0 ? "true" : "false"
                );
                break;
            }
            case 2:
            case 3: {
                const int key = randomInt(random, -20, 20);
                const bool parentQuery = randomInt(random, 0, 1) == 0;
                input << (parentQuery ? "parent " : "value ") << key << '\n';
                const int index = tree.findKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else if (!parentQuery) {
                    appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(index)].value
                    );
                } else {
                    const int parent =
                        tree.nodes[static_cast<std::size_t>(index)].parent;
                    if (parent < 0) appendLine(output, "NONE");
                    else appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(parent)].key
                    );
                }
                break;
            }
            case 4:
                input << "root\n";
                if (tree.root < 0) appendLine(output, "EMPTY");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(tree.root)].key
                );
                break;
            case 5:
                input << "size\n";
                appendLine(output, tree.size());
                break;
            default:
                input << "clear\n";
                tree.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBstDeletionCases(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 12) {
        throw std::runtime_error(
            "F36-bst-deletion-cases operation_limit is too small"
        );
    }
    StressBst tree;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    const int keys[8] = {40, 20, 60, 10, 30, 50, 70, 45};
    for (int key : keys) {
        tree.insert(key, key);
        input << "insert " << key << '\n';
    }
    input << "erase 40\nroot\nparent 20\ninorder\n";
    const int erased = tree.eraseKey(40);
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(erased)].key
    );
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(tree.root)].key
    );
    const int node20 = tree.findKey(20);
    const int parent20 =
        tree.nodes[static_cast<std::size_t>(node20)].parent;
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(parent20)].key
    );
    appendIntList(output, tree.inorder());

    for (int operation = 12; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0: {
                const int key = randomInt(random, -25, 75);
                input << "insert " << key << '\n';
                if (!tree.insert(key, key)) appendLine(output, "DUPLICATE");
                break;
            }
            case 1: {
                const int key = randomInt(random, -25, 75);
                input << "erase " << key << '\n';
                const int index = tree.eraseKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(index)].key
                );
                break;
            }
            case 2: {
                const int key = randomInt(random, -25, 75);
                input << "contains " << key << '\n';
                appendLine(
                    output,
                    tree.findKey(key) >= 0 ? "true" : "false"
                );
                break;
            }
            case 3: {
                const int key = randomInt(random, -25, 75);
                input << "parent " << key << '\n';
                const int index = tree.findKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else {
                    const int parent =
                        tree.nodes[static_cast<std::size_t>(index)].parent;
                    if (parent < 0) appendLine(output, "NONE");
                    else appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(parent)].key
                    );
                }
                break;
            }
            case 4:
                input << "root\n";
                if (tree.root < 0) appendLine(output, "EMPTY");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(tree.root)].key
                );
                break;
            case 5:
                input << "inorder\n";
                appendIntList(output, tree.inorder());
                break;
            case 6:
                input << "size\n";
                appendLine(output, tree.size());
                break;
            default:
                input << "clear\n";
                tree.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBstPredecessorSuccessor(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 10) {
        throw std::runtime_error(
            "F37-bst-predecessor-successor operation_limit is too small"
        );
    }
    StressBst tree;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    const int keys[6] = {10, 5, 20, 15, 17, 13};
    for (int key : keys) {
        tree.insert(key, key);
        input << "insert " << key << '\n';
    }
    input << "predecessor 13\nsuccessor 17\nmin\nmax\n";
    appendLine(output, 10);
    appendLine(output, 20);
    appendLine(output, 5);
    appendLine(output, 20);

    for (int operation = 10; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0: {
                const int key = randomInt(random, -20, 30);
                input << "insert " << key << '\n';
                if (!tree.insert(key, key)) appendLine(output, "DUPLICATE");
                break;
            }
            case 1:
            case 2: {
                const int key = randomInt(random, -20, 30);
                const bool predecessorQuery = randomInt(random, 0, 1) == 0;
                input << (predecessorQuery ? "predecessor " : "successor ")
                      << key << '\n';
                const int index = tree.findKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else {
                    const int neighbor = predecessorQuery
                        ? tree.predecessor(index)
                        : tree.successor(index);
                    if (neighbor < 0) appendLine(output, "NONE");
                    else appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(neighbor)].key
                    );
                }
                break;
            }
            case 3:
            case 4: {
                const bool minimumQuery = randomInt(random, 0, 1) == 0;
                input << (minimumQuery ? "min\n" : "max\n");
                if (tree.root < 0) appendLine(output, "EMPTY");
                else {
                    const int index = minimumQuery
                        ? tree.minimum(tree.root)
                        : tree.maximum(tree.root);
                    appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(index)].key
                    );
                }
                break;
            }
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "size\n";
                    appendLine(output, tree.size());
                } else {
                    input << "clear\n";
                    tree.clear();
                }
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBstRangeView(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 5) {
        throw std::runtime_error(
            "F38-bst-range-view operation_limit is too small"
        );
    }
    std::set<int> values = {30, 50, 70};
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount
          << "\ninsert 50\ninsert 30\ninsert 70\nrange 30 70\ncount 30 70\n";
    output << "30 50 70\n3\n";

    for (int operation = 5; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0: {
                const int key = randomInt(random, -30, 90);
                input << "insert " << key << '\n';
                if (!values.insert(key).second) appendLine(output, "DUPLICATE");
                break;
            }
            case 1:
            case 2: {
                const int low = randomInt(random, -35, 95);
                const int high = randomInt(random, -35, 95);
                const bool countQuery = randomInt(random, 0, 1) == 0;
                input << (countQuery ? "count " : "range ")
                      << low << ' ' << high << '\n';
                std::vector<int> matches;
                if (low <= high) {
                    for (auto iterator = values.lower_bound(low);
                         iterator != values.end() && *iterator <= high;
                         ++iterator) {
                        matches.push_back(*iterator);
                    }
                }
                if (countQuery) {
                    appendLine(output, static_cast<int>(matches.size()));
                } else {
                    appendIntList(output, matches);
                }
                break;
            }
            case 3:
            case 4: {
                const int key = randomInt(random, -35, 95);
                const bool lowerQuery = randomInt(random, 0, 1) == 0;
                input << (lowerQuery ? "lower " : "upper ") << key << '\n';
                const auto iterator = lowerQuery
                    ? values.lower_bound(key)
                    : values.upper_bound(key);
                if (iterator == values.end()) appendLine(output, "NONE");
                else appendLine(output, *iterator);
                break;
            }
            case 5:
                input << "size\n";
                appendLine(output, static_cast<int>(values.size()));
                break;
            default:
                input << "clear\n";
                values.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBstStructuralValidation(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 2) {
        throw std::runtime_error(
            "F39-bst-structural-validation operation_limit is too small"
        );
    }
    const int base = randomInt(random, -500, 500);
    const int keys[4] = {base, base - 20, base + 20, base + 10};
    const int parents[4] = {-1, 0, 0, 1};
    const int left[4] = {1, -1, -1, -1};
    const int right[4] = {2, 3, -1, -1};
    std::ostringstream input;
    std::ostringstream output;
    input << "4 0 " << operationCount << '\n';
    for (int id = 0; id < 4; ++id) {
        input << id << ' ' << keys[id] << ' ' << parents[id] << ' '
              << left[id] << ' ' << right[id] << '\n';
    }
    input << "valid\ninorder\n";
    output << "false\nINVALID\n";
    for (int operation = 2; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0:
                input << "valid\n";
                appendLine(output, "false");
                break;
            case 1:
                input << "inorder\n";
                appendLine(output, "INVALID");
                break;
            case 2:
                input << "root\n";
                appendLine(output, 0);
                break;
            case 3:
                input << "size\n";
                appendLine(output, 4);
                break;
            default: {
                const int id = randomInt(random, -1, 4);
                const int kind = randomInt(random, 0, 3);
                const char* command = kind == 0
                    ? "key"
                    : (kind == 1 ? "parent" : (kind == 2 ? "left" : "right"));
                input << command << ' ' << id << '\n';
                if (id < 0 || id >= 4) {
                    appendLine(output, "NOT_FOUND");
                } else if (kind == 0) {
                    appendLine(output, keys[id]);
                } else {
                    const int link = kind == 1
                        ? parents[id]
                        : (kind == 2 ? left[id] : right[id]);
                    if (link < 0) appendLine(output, "NONE");
                    else appendLine(output, link);
                }
                break;
            }
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBstReplacementTrace(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 13) {
        throw std::runtime_error(
            "F40-bst-replacement-trace operation_limit is too small"
        );
    }
    StressBst tree;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    const int ids[7] = {10, 20, 30, 40, 50, 60, 70};
    const int keys[7] = {50, 30, 70, 20, 40, 60, 80};
    for (int index = 0; index < 7; ++index) {
        tree.insert(ids[index], keys[index]);
        input << "insert " << ids[index] << ' ' << keys[index] << '\n';
    }
    input << "erase 50\nroot\nparent 20\nleft 60\nfind 60\ninorder\n";
    const int erased = tree.eraseKey(50);
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(erased)].id
    );
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(tree.root)].id
    );
    const int node20 = tree.findId(20);
    const int parent20 =
        tree.nodes[static_cast<std::size_t>(node20)].parent;
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(parent20)].id
    );
    const int node60 = tree.findId(60);
    appendLine(
        output,
        tree.nodes[static_cast<std::size_t>(
            tree.nodes[static_cast<std::size_t>(node60)].left
        )].id
    );
    appendLine(output, 60);
    appendIntList(output, tree.inorder(true));

    for (int operation = 13; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 8)) {
            case 0: {
                const int id = randomInt(random, 1, 30);
                const int key = randomInt(random, -30, 90);
                input << "insert " << id << ' ' << key << '\n';
                if (!tree.insert(id, key)) appendLine(output, "DUPLICATE");
                break;
            }
            case 1: {
                const int key = randomInt(random, -30, 90);
                input << "erase " << key << '\n';
                const int index = tree.eraseKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(index)].id
                );
                break;
            }
            case 2: {
                const int key = randomInt(random, -30, 90);
                input << "find " << key << '\n';
                const int index = tree.findKey(key);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(index)].id
                );
                break;
            }
            case 3:
            case 4:
            case 5: {
                const int id = randomInt(random, 1, 70);
                const int kind = randomInt(random, 0, 3);
                const char* command = kind == 0
                    ? "key"
                    : (kind == 1 ? "parent" : (kind == 2 ? "left" : "right"));
                input << command << ' ' << id << '\n';
                const int index = tree.findId(id);
                if (index < 0) appendLine(output, "NOT_FOUND");
                else if (kind == 0) {
                    appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(index)].key
                    );
                } else {
                    const StressBstNode& node =
                        tree.nodes[static_cast<std::size_t>(index)];
                    const int link = kind == 1
                        ? node.parent
                        : (kind == 2 ? node.left : node.right);
                    if (link < 0) appendLine(output, "NONE");
                    else appendLine(
                        output,
                        tree.nodes[static_cast<std::size_t>(link)].id
                    );
                }
                break;
            }
            case 6:
                input << "root\n";
                if (tree.root < 0) appendLine(output, "EMPTY");
                else appendLine(
                    output,
                    tree.nodes[static_cast<std::size_t>(tree.root)].id
                );
                break;
            case 7:
                input << "inorder\n";
                appendIntList(output, tree.inorder(true));
                break;
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "size\n";
                    appendLine(output, tree.size());
                } else {
                    input << "clear\n";
                    tree.clear();
                }
                break;
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerBstGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F35-bst-insert-search", generateBstInsertSearch);
    registry.emplace("F36-bst-deletion-cases", generateBstDeletionCases);
    registry.emplace("F37-bst-predecessor-successor", generateBstPredecessorSuccessor);
    registry.emplace("F38-bst-range-view", generateBstRangeView);
    registry.emplace("F39-bst-structural-validation", generateBstStructuralValidation);
    registry.emplace("F40-bst-replacement-trace", generateBstReplacementTrace);
}

}  // namespace judge::generators
