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
GeneratedCase generateArrayBinaryTree(
    std::mt19937_64& random,
    int operationCount
) {
    const int capacity = randomInt(random, 7, 20);
    if (operationCount < 8) {
        throw std::runtime_error(
            "F27-array-binary-tree operation_limit is too small"
        );
    }
    std::vector<int> values(static_cast<std::size_t>(capacity), 0);
    std::vector<bool> occupied(static_cast<std::size_t>(capacity), false);
    int count = 0;
    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';

    input << "set 0 10\nset 1 20\nset 2 30\nset 3 40\n"
          << "parent_index 3\nleft_index 1\nright_index 1\noccupied\n";
    occupied[0] = occupied[1] = occupied[2] = occupied[3] = true;
    values[0] = 10;
    values[1] = 20;
    values[2] = 30;
    values[3] = 40;
    count = 4;
    appendLine(output, 1);
    appendLine(output, 3);
    appendLine(output, "NONE");
    appendLine(output, 4);

    auto valid = [&](int index) { return index >= 0 && index < capacity; };
    auto relative = [&](int index, int target) {
        if (!valid(index)) appendLine(output, "OUT_OF_RANGE");
        else if (!occupied[static_cast<std::size_t>(index)]) {
            appendLine(output, "EMPTY");
        } else if (!valid(target) ||
                   !occupied[static_cast<std::size_t>(target)]) {
            appendLine(output, "NONE");
        } else {
            appendLine(output, target);
        }
    };

    for (int operation = 8; operation < operationCount; ++operation) {
        const int index = randomInt(random, -2, capacity + 1);
        switch (randomInt(random, 0, 7)) {
            case 0:
            case 1: {
                const int value = randomValue(random);
                input << "set " << index << ' ' << value << '\n';
                if (!valid(index)) {
                    appendLine(output, "OUT_OF_RANGE");
                } else if (!occupied[static_cast<std::size_t>(index)] &&
                           index != 0 &&
                           !occupied[static_cast<std::size_t>(
                               (index - 1) / 2
                           )]) {
                    appendLine(output, "NO_PARENT");
                } else {
                    if (!occupied[static_cast<std::size_t>(index)]) ++count;
                    occupied[static_cast<std::size_t>(index)] = true;
                    values[static_cast<std::size_t>(index)] = value;
                }
                break;
            }
            case 2:
                input << "erase " << index << '\n';
                if (!valid(index)) appendLine(output, "OUT_OF_RANGE");
                else if (!occupied[static_cast<std::size_t>(index)]) {
                    appendLine(output, "EMPTY");
                } else {
                    const int left = 2 * index + 1;
                    const int right = left + 1;
                    if ((valid(left) &&
                         occupied[static_cast<std::size_t>(left)]) ||
                        (valid(right) &&
                         occupied[static_cast<std::size_t>(right)])) {
                        appendLine(output, "HAS_CHILDREN");
                    } else {
                        appendLine(
                            output,
                            values[static_cast<std::size_t>(index)]
                        );
                        occupied[static_cast<std::size_t>(index)] = false;
                        --count;
                    }
                }
                break;
            case 3:
                input << "get " << index << '\n';
                if (!valid(index)) appendLine(output, "OUT_OF_RANGE");
                else if (!occupied[static_cast<std::size_t>(index)]) {
                    appendLine(output, "EMPTY");
                } else {
                    appendLine(
                        output,
                        values[static_cast<std::size_t>(index)]
                    );
                }
                break;
            case 4:
                input << "parent_index " << index << '\n';
                relative(index, index == 0 ? -1 : (index - 1) / 2);
                break;
            case 5:
                input << "left_index " << index << '\n';
                relative(index, 2 * index + 1);
                break;
            case 6:
                input << "right_index " << index << '\n';
                relative(index, 2 * index + 2);
                break;
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "occupied\n";
                    appendLine(output, count);
                } else {
                    input << "clear\n";
                    std::fill(occupied.begin(), occupied.end(), false);
                    count = 0;
                }
                break;
        }
    }
    return {input.str(), output.str()};
}

struct LinkedTreeModelNode {
    int id;
    int value;
    int parent;
    int left;
    int right;
};

GeneratedCase generateLinkedBinaryTree(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 4) {
        throw std::runtime_error(
            "F28-linked-binary-tree operation_limit is too small"
        );
    }
    std::vector<LinkedTreeModelNode> nodes;
    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';
    input << "root 10 100\nattach_left 10 20 200\nparent 20\nleft 10\n";
    nodes.push_back({10, 100, -1, 20, -1});
    nodes.push_back({20, 200, 10, -1, -1});
    appendLine(output, 10);
    appendLine(output, 20);

    auto findIndex = [&](int id) {
        for (int index = 0; index < static_cast<int>(nodes.size()); ++index) {
            if (nodes[static_cast<std::size_t>(index)].id == id) return index;
        }
        return -1;
    };

    for (int operation = 4; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 8)) {
            case 0: {
                const int id = randomInt(random, 30, 60);
                const int value = randomValue(random);
                input << "root " << id << ' ' << value << '\n';
                if (!nodes.empty()) appendLine(output, "OCCUPIED");
                else nodes.push_back({id, value, -1, -1, -1});
                break;
            }
            case 1:
            case 2: {
                const bool left = randomInt(random, 0, 1) == 0;
                const int parentId = nodes.empty() ||
                        randomInt(random, 0, 3) == 0
                    ? randomInt(random, 70, 80)
                    : nodes[static_cast<std::size_t>(
                          randomInt(
                              random,
                              0,
                              static_cast<int>(nodes.size()) - 1
                          )
                      )].id;
                const int childId = randomInt(random, 30, 60);
                const int value = randomValue(random);
                input << (left ? "attach_left " : "attach_right ")
                      << parentId << ' ' << childId << ' ' << value << '\n';
                const int parentIndex = findIndex(parentId);
                if (parentIndex == -1) appendLine(output, "NOT_FOUND");
                else if (findIndex(childId) != -1) {
                    appendLine(output, "DUPLICATE");
                } else {
                    int& link = left
                        ? nodes[static_cast<std::size_t>(parentIndex)].left
                        : nodes[static_cast<std::size_t>(parentIndex)].right;
                    if (link != -1) appendLine(output, "OCCUPIED");
                    else {
                        link = childId;
                        nodes.push_back({
                            childId, value, parentId, -1, -1
                        });
                    }
                }
                break;
            }
            case 3:
            case 4:
            case 5:
            case 6: {
                const int id = nodes.empty() ||
                        randomInt(random, 0, 3) == 0
                    ? randomInt(random, 70, 80)
                    : nodes[static_cast<std::size_t>(
                          randomInt(
                              random,
                              0,
                              static_cast<int>(nodes.size()) - 1
                          )
                      )].id;
                const int query = randomInt(random, 0, 3);
                const char* command = query == 0
                    ? "value"
                    : (query == 1 ? "parent" : (query == 2 ? "left" : "right"));
                input << command << ' ' << id << '\n';
                const int index = findIndex(id);
                if (index == -1) appendLine(output, "NOT_FOUND");
                else {
                    const LinkedTreeModelNode& node =
                        nodes[static_cast<std::size_t>(index)];
                    if (query == 0) appendLine(output, node.value);
                    else {
                        const int link = query == 1
                            ? node.parent
                            : (query == 2 ? node.left : node.right);
                        if (link == -1) appendLine(output, "NONE");
                        else appendLine(output, link);
                    }
                }
                break;
            }
            case 7: {
                const int id = nodes.empty() ||
                        randomInt(random, 0, 3) == 0
                    ? randomInt(random, 70, 80)
                    : nodes[static_cast<std::size_t>(
                          randomInt(
                              random,
                              0,
                              static_cast<int>(nodes.size()) - 1
                          )
                      )].id;
                input << "erase_leaf " << id << '\n';
                const int index = findIndex(id);
                if (index == -1) appendLine(output, "NOT_FOUND");
                else {
                    const LinkedTreeModelNode node =
                        nodes[static_cast<std::size_t>(index)];
                    if (node.left != -1 || node.right != -1) {
                        appendLine(output, "NOT_LEAF");
                    } else {
                        appendLine(output, node.value);
                        if (node.parent != -1) {
                            const int parentIndex = findIndex(node.parent);
                            LinkedTreeModelNode& parent =
                                nodes[static_cast<std::size_t>(parentIndex)];
                            if (parent.left == id) parent.left = -1;
                            else parent.right = -1;
                        }
                        nodes.erase(nodes.begin() + index);
                    }
                }
                break;
            }
            default:
                if (randomInt(random, 0, 1) == 0) {
                    input << "size\n";
                    appendLine(output, static_cast<int>(nodes.size()));
                } else {
                    input << "clear\n";
                    nodes.clear();
                }
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateInorderThreadedTree(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F29-inorder-threaded-tree operation_limit is too small"
        );
    }
    constexpr int kNodeCount = 5;
    constexpr int kRootId = 4;
    const int left[kNodeCount] = {-1, -1, 0, -1, 1};
    const int right[kNodeCount] = {-1, 3, -1, -1, 2};
    const int inorder[kNodeCount] = {1, 3, 4, 0, 2};
    int values[kNodeCount] = {};
    int positions[kNodeCount] = {};
    for (int index = 0; index < kNodeCount; ++index) {
        positions[inorder[index]] = index;
    }
    std::ostringstream input;
    std::ostringstream output;
    input << kNodeCount << ' ' << kRootId << ' ' << operationCount << '\n';
    for (int id = 0; id < kNodeCount; ++id) {
        values[id] = randomValue(random);
        input << id << ' ' << values[id] << ' ' << left[id] << ' '
              << right[id] << '\n';
    }
    input << "inorder\nsuccessor 4\npredecessor 2\n";
    output << "1 3 4 0 2\n0\n0\n";

    for (int operation = 3; operation < operationCount; ++operation) {
        const int id = randomInt(random, -1, kNodeCount);
        switch (randomInt(random, 0, 5)) {
            case 0:
                input << "value " << id << '\n';
                if (id < 0 || id >= kNodeCount) appendLine(output, "NOT_FOUND");
                else appendLine(output, values[id]);
                break;
            case 1:
            case 2: {
                const bool predecessor = randomInt(random, 0, 1) == 0;
                input << (predecessor ? "predecessor " : "successor ")
                      << id << '\n';
                if (id < 0 || id >= kNodeCount) {
                    appendLine(output, "NOT_FOUND");
                } else {
                    const int position = positions[id] +
                        (predecessor ? -1 : 1);
                    if (position < 0 || position >= kNodeCount) {
                        appendLine(output, "NONE");
                    } else {
                        appendLine(output, inorder[position]);
                    }
                }
                break;
            }
            case 3:
            case 4: {
                const bool queryLeft = randomInt(random, 0, 1) == 0;
                input << (queryLeft ? "left_link " : "right_link ") << id
                      << '\n';
                if (id < 0 || id >= kNodeCount) {
                    appendLine(output, "NOT_FOUND");
                } else {
                    const int child = queryLeft ? left[id] : right[id];
                    if (child != -1) {
                        output << "CHILD " << child << '\n';
                    } else {
                        const int position = positions[id] +
                            (queryLeft ? -1 : 1);
                        if (position < 0 || position >= kNodeCount) {
                            appendLine(output, "NONE");
                        } else {
                            output << "THREAD " << inorder[position] << '\n';
                        }
                    }
                }
                break;
            }
            default:
                input << "inorder\n";
                output << "1 3 4 0 2\n";
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBinaryMinHeap(
    std::mt19937_64& random,
    int operationCount
) {
    const int capacity = randomInt(random, 6, 12);
    if (operationCount < 7) {
        throw std::runtime_error(
            "F30-binary-min-heap operation_limit is too small"
        );
    }
    std::vector<int> heap;
    heap.reserve(static_cast<std::size_t>(capacity));
    auto siftUp = [&](int index) {
        while (index > 0) {
            const int parent = (index - 1) / 2;
            if (heap[static_cast<std::size_t>(parent)] <=
                heap[static_cast<std::size_t>(index)]) break;
            std::swap(
                heap[static_cast<std::size_t>(parent)],
                heap[static_cast<std::size_t>(index)]
            );
            index = parent;
        }
    };
    auto siftDown = [&](int index) {
        while (true) {
            int smallest = index;
            const int left = 2 * index + 1;
            const int right = left + 1;
            if (left < static_cast<int>(heap.size()) &&
                heap[static_cast<std::size_t>(left)] <
                    heap[static_cast<std::size_t>(smallest)]) {
                smallest = left;
            }
            if (right < static_cast<int>(heap.size()) &&
                heap[static_cast<std::size_t>(right)] <
                    heap[static_cast<std::size_t>(smallest)]) {
                smallest = right;
            }
            if (smallest == index) break;
            std::swap(
                heap[static_cast<std::size_t>(index)],
                heap[static_cast<std::size_t>(smallest)]
            );
            index = smallest;
        }
    };
    auto push = [&](int value) {
        heap.push_back(value);
        siftUp(static_cast<int>(heap.size()) - 1);
    };

    std::ostringstream input;
    std::ostringstream output;
    input << capacity << ' ' << operationCount << '\n';
    input << "push 1\npush 3\npush 2\npush 7\npush 6\nchange 0 9\ntop\n";
    push(1);
    push(3);
    push(2);
    push(7);
    push(6);
    heap[0] = 9;
    siftDown(0);
    appendLine(output, heap[0]);

    for (int operation = 7; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0:
            case 1: {
                const int value = randomValue(random);
                input << "push " << value << '\n';
                if (static_cast<int>(heap.size()) == capacity) {
                    appendLine(output, "FULL");
                } else {
                    push(value);
                }
                break;
            }
            case 2:
                input << "pop\n";
                if (heap.empty()) appendLine(output, "EMPTY");
                else {
                    appendLine(output, heap[0]);
                    heap[0] = heap.back();
                    heap.pop_back();
                    if (!heap.empty()) siftDown(0);
                }
                break;
            case 3:
                input << "top\n";
                if (heap.empty()) appendLine(output, "EMPTY");
                else appendLine(output, heap[0]);
                break;
            case 4: {
                const int index = randomInt(
                    random,
                    -2,
                    static_cast<int>(heap.size()) + 1
                );
                const int value = randomValue(random);
                input << "change " << index << ' ' << value << '\n';
                if (index < 0 || index >= static_cast<int>(heap.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    const int old = heap[static_cast<std::size_t>(index)];
                    heap[static_cast<std::size_t>(index)] = value;
                    if (value < old) siftUp(index);
                    else siftDown(index);
                }
                break;
            }
            case 5: {
                const int index = randomInt(
                    random,
                    -2,
                    static_cast<int>(heap.size()) + 1
                );
                input << "at " << index << '\n';
                if (index < 0 || index >= static_cast<int>(heap.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(
                        output,
                        heap[static_cast<std::size_t>(index)]
                    );
                }
                break;
            }
            case 6:
                if (randomInt(random, 0, 1) == 0) {
                    input << "size\n";
                    appendLine(output, static_cast<int>(heap.size()));
                } else {
                    input << "empty\n";
                    appendLine(output, heap.empty() ? "true" : "false");
                }
                break;
            default:
                input << "clear\n";
                heap.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateBinaryTreeTraversalViews(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F31-binary-tree-traversal-views operation_limit is too small"
        );
    }
    constexpr int kNodeCount = 8;
    const int left[kNodeCount] = {-1, 0, -1, -1, -1, 3, 1, -1};
    const int right[kNodeCount] = {-1, 2, -1, 6, -1, 7, 4, -1};
    int values[kNodeCount] = {};
    const std::string preorder = "5 3 6 1 0 2 4 7";
    const std::string inorder = "3 0 1 2 6 4 5 7";
    const std::string postorder = "0 2 1 4 6 3 7 5";

    std::ostringstream input;
    std::ostringstream output;
    input << kNodeCount << " 5 " << operationCount << '\n';
    for (int id = 0; id < kNodeCount; ++id) {
        values[id] = randomValue(random);
        input << id << ' ' << values[id] << ' ' << left[id] << ' '
              << right[id] << '\n';
    }
    input << "preorder\ninorder\npostorder\n";
    appendLine(output, preorder);
    appendLine(output, inorder);
    appendLine(output, postorder);

    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0:
                input << "preorder\n";
                appendLine(output, preorder);
                break;
            case 1:
                input << "inorder\n";
                appendLine(output, inorder);
                break;
            case 2:
                input << "postorder\n";
                appendLine(output, postorder);
                break;
            case 3:
                input << "height\n";
                appendLine(output, 5);
                break;
            case 4:
                input << "size\n";
                appendLine(output, kNodeCount);
                break;
            default: {
                const int id = randomInt(random, -1, kNodeCount);
                input << "value " << id << '\n';
                if (id < 0 || id >= kNodeCount) appendLine(output, "NOT_FOUND");
                else appendLine(output, values[id]);
                break;
            }
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateLevelOrderTreeView(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F32-level-order-tree-view operation_limit is too small"
        );
    }
    constexpr int kNodeCount = 8;
    const int left[kNodeCount] = {-1, 0, -1, -1, -1, 3, 1, -1};
    const int right[kNodeCount] = {-1, 2, -1, 6, -1, 7, 4, -1};
    const int depths[kNodeCount] = {4, 3, 4, 1, 3, 0, 2, 1};
    const char* levels[5] = {"5", "3 7", "6", "1 4", "0 2"};
    std::ostringstream input;
    std::ostringstream output;
    input << kNodeCount << " 5 " << operationCount << '\n';
    for (int id = 0; id < kNodeCount; ++id) {
        input << id << ' ' << randomValue(random) << ' ' << left[id] << ' '
              << right[id] << '\n';
    }
    input << "levelorder\nqueue_peak\nlevels\n";
    output << "5 3 7 6 1 4 0 2\n3\n5\n";

    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 5)) {
            case 0:
                input << "levelorder\n";
                output << "5 3 7 6 1 4 0 2\n";
                break;
            case 1: {
                const int depth = randomInt(random, -1, 6);
                input << "level " << depth << '\n';
                if (depth < 0) appendLine(output, "OUT_OF_RANGE");
                else if (depth >= 5) appendLine(output, "EMPTY");
                else appendLine(output, levels[depth]);
                break;
            }
            case 2: {
                const int id = randomInt(random, -1, kNodeCount);
                input << "depth " << id << '\n';
                if (id < 0 || id >= kNodeCount) appendLine(output, "NOT_FOUND");
                else appendLine(output, depths[id]);
                break;
            }
            case 3: {
                const int depth = randomInt(random, -1, 6);
                input << "width " << depth << '\n';
                if (depth < 0) appendLine(output, "OUT_OF_RANGE");
                else if (depth >= 5) appendLine(output, 0);
                else appendLine(
                    output,
                    depth == 0 || depth == 2 ? 1 : 2
                );
                break;
            }
            case 4:
                input << "levels\n";
                appendLine(output, 5);
                break;
            default:
                input << "queue_peak\n";
                appendLine(output, 3);
                break;
        }
    }
    return {input.str(), output.str()};
}

void buildMinHeap(std::vector<int>& heap) {
    auto siftDown = [&](int start) {
        int index = start;
        while (true) {
            int smallest = index;
            const int left = 2 * index + 1;
            const int right = left + 1;
            if (left < static_cast<int>(heap.size()) &&
                heap[static_cast<std::size_t>(left)] <
                    heap[static_cast<std::size_t>(smallest)]) {
                smallest = left;
            }
            if (right < static_cast<int>(heap.size()) &&
                heap[static_cast<std::size_t>(right)] <
                    heap[static_cast<std::size_t>(smallest)]) {
                smallest = right;
            }
            if (smallest == index) break;
            std::swap(
                heap[static_cast<std::size_t>(index)],
                heap[static_cast<std::size_t>(smallest)]
            );
            index = smallest;
        }
    };
    for (int index = static_cast<int>(heap.size()) / 2 - 1;
         index >= 0;
         --index) {
        siftDown(index);
    }
}

void appendHeapArray(
    std::ostringstream& output,
    const std::vector<int>& heap
) {
    if (heap.empty()) {
        appendLine(output, "EMPTY");
        return;
    }
    for (std::size_t index = 0; index < heap.size(); ++index) {
        if (index != 0) output << ' ';
        output << heap[index];
    }
    output << '\n';
}

GeneratedCase generateBottomUpMinHeapBuild(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 1) {
        throw std::runtime_error(
            "F33-bottom-up-min-heap-build operation_limit is too small"
        );
    }
    std::vector<int> heap = {5, 4, 3, 2, 1};
    buildMinHeap(heap);
    std::ostringstream input;
    std::ostringstream output;
    input << heap.size() << ' ' << operationCount << "\n5 4 3 2 1\n";
    input << "array\n";
    appendHeapArray(output, heap);

    for (int operation = 1; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 7)) {
            case 0:
                input << "array\n";
                appendHeapArray(output, heap);
                break;
            case 1:
                input << "top\n";
                appendLine(output, heap[0]);
                break;
            case 2: {
                const int index = randomInt(
                    random,
                    -2,
                    static_cast<int>(heap.size()) + 1
                );
                input << "at " << index << '\n';
                if (index < 0 || index >= static_cast<int>(heap.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, heap[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 3:
            case 4:
            case 5: {
                const int index = randomInt(
                    random,
                    -2,
                    static_cast<int>(heap.size()) + 1
                );
                const int relation = randomInt(random, 0, 2);
                const char* command =
                    relation == 0 ? "parent" : (relation == 1 ? "left" : "right");
                input << command << ' ' << index << '\n';
                if (index < 0 || index >= static_cast<int>(heap.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    const int relative = relation == 0
                        ? (index == 0 ? -1 : (index - 1) / 2)
                        : (relation == 1 ? 2 * index + 1 : 2 * index + 2);
                    if (relative < 0 ||
                        relative >= static_cast<int>(heap.size())) {
                        appendLine(output, "NONE");
                    } else {
                        appendLine(output, relative);
                    }
                }
                break;
            }
            case 6:
                input << "size\n";
                appendLine(output, static_cast<int>(heap.size()));
                break;
            default:
                input << "valid\n";
                appendLine(output, "true");
                break;
        }
    }
    return {input.str(), output.str()};
}

GeneratedCase generateMinHeapRemovalTrace(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 3) {
        throw std::runtime_error(
            "F34-min-heap-removal-trace operation_limit is too small"
        );
    }
    std::vector<int> heap = {0, 3, 1, 7, 8, 4, 2, 9};
    buildMinHeap(heap);
    auto pop = [&]() {
        const int removed = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) buildMinHeap(heap);
        return removed;
    };
    std::ostringstream input;
    std::ostringstream output;
    input << heap.size() << ' ' << operationCount
          << "\n0 3 1 7 8 4 2 9\n";
    input << "array\npop\narray\n";
    appendHeapArray(output, heap);
    appendLine(output, pop());
    appendHeapArray(output, heap);

    for (int operation = 3; operation < operationCount; ++operation) {
        switch (randomInt(random, 0, 6)) {
            case 0:
            case 1:
                input << "pop\n";
                if (heap.empty()) appendLine(output, "EMPTY");
                else appendLine(output, pop());
                break;
            case 2:
                input << "top\n";
                if (heap.empty()) appendLine(output, "EMPTY");
                else appendLine(output, heap[0]);
                break;
            case 3:
                input << "array\n";
                appendHeapArray(output, heap);
                break;
            case 4: {
                const int index = randomInt(
                    random,
                    -2,
                    static_cast<int>(heap.size()) + 1
                );
                input << "at " << index << '\n';
                if (index < 0 || index >= static_cast<int>(heap.size())) {
                    appendLine(output, "OUT_OF_RANGE");
                } else {
                    appendLine(output, heap[static_cast<std::size_t>(index)]);
                }
                break;
            }
            case 5:
                if (randomInt(random, 0, 1) == 0) {
                    input << "size\n";
                    appendLine(output, static_cast<int>(heap.size()));
                } else {
                    input << "empty\n";
                    appendLine(output, heap.empty() ? "true" : "false");
                }
                break;
            default:
                input << "clear\n";
                heap.clear();
                break;
        }
    }
    return {input.str(), output.str()};
}


}  // namespace

void registerTreeGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("F27-array-binary-tree", generateArrayBinaryTree);
    registry.emplace("F28-linked-binary-tree", generateLinkedBinaryTree);
    registry.emplace("F29-inorder-threaded-tree", generateInorderThreadedTree);
    registry.emplace("F30-binary-min-heap", generateBinaryMinHeap);
    registry.emplace("F31-binary-tree-traversal-views", generateBinaryTreeTraversalViews);
    registry.emplace("F32-level-order-tree-view", generateLevelOrderTreeView);
    registry.emplace("F33-bottom-up-min-heap-build", generateBottomUpMinHeapBuild);
    registry.emplace("F34-min-heap-removal-trace", generateMinHeapRemovalTrace);
}

}  // namespace judge::generators
