#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

// Shared brute-force ordered set over a sorted unique vector. Order-statistic
// answers come from this independent structure, never from the reference's tree,
// so a stress pass genuinely validates select / rank / range_count.
struct OrderedSet {
    std::vector<int> keys;

    bool insert(int key) {
        const auto position = std::lower_bound(keys.begin(), keys.end(), key);
        if (position != keys.end() && *position == key) return false;
        keys.insert(position, key);
        return true;
    }
    bool erase(int key) {
        const auto position = std::lower_bound(keys.begin(), keys.end(), key);
        if (position == keys.end() || *position != key) return false;
        keys.erase(position);
        return true;
    }
    bool contains(int key) const {
        const auto position = std::lower_bound(keys.begin(), keys.end(), key);
        return position != keys.end() && *position == key;
    }
    int rank(int key) const {
        return static_cast<int>(
            std::lower_bound(keys.begin(), keys.end(), key) - keys.begin()
        );
    }
    int rangeCount(int low, int high) const {
        if (low > high) return 0;
        return static_cast<int>(
            std::upper_bound(keys.begin(), keys.end(), high) -
            std::lower_bound(keys.begin(), keys.end(), low)
        );
    }
    int size() const { return static_cast<int>(keys.size()); }
};

// ----- A36: order-statistic AVL (embedded AVL only for the height output) ----

struct AvlNode {
    int key;
    int height;
    AvlNode* left;
    AvlNode* right;
};

int avlHeight(AvlNode* node) { return node ? node->height : 0; }
void avlUpdate(AvlNode* node) {
    node->height = 1 + std::max(avlHeight(node->left), avlHeight(node->right));
}
int avlBalance(AvlNode* node) {
    return node ? avlHeight(node->left) - avlHeight(node->right) : 0;
}
AvlNode* avlRotateRight(AvlNode* y) {
    AvlNode* x = y->left;
    y->left = x->right;
    x->right = y;
    avlUpdate(y);
    avlUpdate(x);
    return x;
}
AvlNode* avlRotateLeft(AvlNode* x) {
    AvlNode* y = x->right;
    x->right = y->left;
    y->left = x;
    avlUpdate(x);
    avlUpdate(y);
    return y;
}
AvlNode* avlRebalance(AvlNode* node) {
    avlUpdate(node);
    const int factor = avlBalance(node);
    if (factor > 1) {
        if (avlBalance(node->left) < 0) node->left = avlRotateLeft(node->left);
        return avlRotateRight(node);
    }
    if (factor < -1) {
        if (avlBalance(node->right) > 0) node->right = avlRotateRight(node->right);
        return avlRotateLeft(node);
    }
    return node;
}
AvlNode* avlInsert(AvlNode* node, int key) {
    if (!node) return new AvlNode{key, 1, nullptr, nullptr};
    if (key < node->key) node->left = avlInsert(node->left, key);
    else if (key > node->key) node->right = avlInsert(node->right, key);
    else return node;
    return avlRebalance(node);
}
AvlNode* avlMin(AvlNode* node) {
    while (node->left) node = node->left;
    return node;
}
AvlNode* avlErase(AvlNode* node, int key) {
    if (!node) return nullptr;
    if (key < node->key) {
        node->left = avlErase(node->left, key);
    } else if (key > node->key) {
        node->right = avlErase(node->right, key);
    } else {
        if (!node->left || !node->right) {
            AvlNode* child = node->left ? node->left : node->right;
            delete node;
            return child;
        }
        AvlNode* successor = avlMin(node->right);
        node->key = successor->key;
        node->right = avlErase(node->right, successor->key);
    }
    return avlRebalance(node);
}
void avlDestroy(AvlNode* node) {
    if (!node) return;
    avlDestroy(node->left);
    avlDestroy(node->right);
    delete node;
}

GeneratedCase generateOrderStatisticAvl(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 16) {
        throw std::runtime_error(
            "A36-order-statistic-avl operation_limit is too small"
        );
    }
    const int keyRange = 40;
    OrderedSet set;
    AvlNode* root = nullptr;

    std::ostringstream input;
    std::ostringstream output;
    input << operationCount << '\n';

    auto doInsert = [&](int key) {
        input << "insert " << key << '\n';
        if (!set.insert(key)) appendLine(output, "DUPLICATE");
        else root = avlInsert(root, key);
    };
    auto doErase = [&](int key) {
        input << "erase " << key << '\n';
        if (set.erase(key)) {
            root = avlErase(root, key);
            appendLine(output, "REMOVED");
        } else {
            appendLine(output, "NOT_FOUND");
        }
    };
    auto emitHeight = [&]() {
        input << "height\n";
        appendLine(output, avlHeight(root));
    };
    auto emitSelect = [&](int k) {
        input << "select " << k << '\n';
        if (k < 1 || k > set.size()) appendLine(output, "OUT_OF_RANGE");
        else appendLine(output, set.keys[static_cast<std::size_t>(k - 1)]);
    };
    auto emitRank = [&](int key) {
        input << "rank " << key << '\n';
        appendLine(output, set.rank(key));
    };

    // Pinned prefix: ascending inserts make the height query diverge for a
    // non-rebalancing BST; rank on a present key catches the inclusive-rank bug.
    for (int key = 0; key < 8; ++key) doInsert(key);
    emitHeight();
    emitRank(4);
    emitSelect(3);
    input << "range_count 2 6\n";
    appendLine(output, set.rangeCount(2, 6));
    doErase(3);
    emitSelect(3);
    emitHeight();

    int emitted = 15;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 11);
        const int key = randomInt(random, 0, keyRange);
        if (kind <= 3) {
            doInsert(key);
        } else if (kind == 4) {
            doErase(key);
        } else if (kind == 5) {
            input << "contains " << key << '\n';
            appendLine(output, set.contains(key) ? "true" : "false");
        } else if (kind == 6) {
            emitSelect(randomInt(random, -1, set.size() + 2));
        } else if (kind == 7) {
            emitRank(key);
        } else if (kind == 8) {
            const int low = randomInt(random, 0, keyRange);
            const int high = randomInt(random, 0, keyRange);
            input << "range_count " << low << ' ' << high << '\n';
            appendLine(output, set.rangeCount(low, high));
        } else if (kind == 9) {
            input << "size\n";
            appendLine(output, set.size());
        } else if (kind == 10) {
            emitHeight();
        } else {
            input << "min\n";
            appendLine(
                output,
                set.size() == 0 ? std::string("EMPTY")
                                : std::to_string(set.keys.front())
            );
        }
        ++emitted;
    }
    avlDestroy(root);
    return {input.str(), output.str()};
}

void appendSplit(
    std::ostringstream& output,
    const std::vector<int>& left,
    const std::vector<int>& right
) {
    if (left.empty()) output << "EMPTY";
    else for (std::size_t i = 0; i < left.size(); ++i) output << (i ? " " : "") << left[i];
    output << " | ";
    if (right.empty()) output << "EMPTY";
    else for (std::size_t i = 0; i < right.size(); ++i) output << (i ? " " : "") << right[i];
    output << '\n';
}

// ----- A37: order-statistic red-black tree -------------------------------

GeneratedCase generateOrderStatisticRedBlack(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 15) {
        throw std::runtime_error(
            "A37-order-statistic-red-black operation_limit is too small"
        );
    }
    OrderedSet set;
    std::ostringstream input, output;
    input << operationCount << '\n';
    auto insert = [&](int key) {
        input << "insert " << key << '\n';
        if (!set.insert(key)) appendLine(output, "DUPLICATE");
    };
    auto erase = [&](int key) {
        input << "erase " << key << '\n';
        appendLine(output, set.erase(key) ? "REMOVED" : "NOT_FOUND");
    };
    auto rank = [&](int key) {
        input << "rank " << key << '\n';
        appendLine(output, set.rank(key));
    };
    auto select = [&](int k) {
        input << "select " << k << '\n';
        if (k < 1 || k > set.size()) appendLine(output, "OUT_OF_RANGE");
        else appendLine(output, set.keys[static_cast<std::size_t>(k - 1)]);
    };
    for (int key : {10, 5, 15, 1, 7}) insert(key);
    rank(10);
    erase(5);
    input << "contains 5\n"; appendLine(output, "false");
    select(2);
    input << "predecessor 10\n"; appendLine(output, 7);
    input << "successor 10\n"; appendLine(output, 15);
    input << "range_count 1 15\n"; appendLine(output, 4);
    input << "size\n"; appendLine(output, 4);
    erase(99);
    input << "inorder\n"; appendIntList(output, set.keys);
    int emitted = 15;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 10);
        const int key = randomInt(random, -30, 30);
        if (kind <= 2) insert(key);
        else if (kind == 3) erase(key);
        else if (kind == 4) rank(key);
        else if (kind == 5) select(randomInt(random, 0, set.size() + 1));
        else if (kind == 6) {
            input << "contains " << key << '\n';
            appendLine(output, set.contains(key) ? "true" : "false");
        } else if (kind == 7) {
            const int high = randomInt(random, -30, 30);
            input << "range_count " << key << ' ' << high << '\n';
            appendLine(output, set.rangeCount(key, high));
        } else if (kind == 8) {
            input << "predecessor " << key << '\n';
            auto position = std::lower_bound(set.keys.begin(), set.keys.end(), key);
            if (position == set.keys.begin()) appendLine(output, "NONE");
            else appendLine(output, *--position);
        } else if (kind == 9) {
            input << "successor " << key << '\n';
            auto position = std::upper_bound(set.keys.begin(), set.keys.end(), key);
            if (position == set.keys.end()) appendLine(output, "NONE");
            else appendLine(output, *position);
        } else {
            input << "size\n"; appendLine(output, set.size());
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A38: explicit-priority Treap --------------------------------------

GeneratedCase generateTreapSplitMerge(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 14) {
        throw std::runtime_error("A38-treap-split-merge operation_limit is too small");
    }
    struct Entry { int key; int priority; };
    std::vector<Entry> entries;
    std::ostringstream input, output;
    input << operationCount << '\n';
    auto position = [&](int key) {
        return std::lower_bound(entries.begin(), entries.end(), key,
            [](const Entry& entry, int value) { return entry.key < value; });
    };
    auto insert = [&](int key, int priority) {
        input << "insert " << key << ' ' << priority << '\n';
        auto at = position(key);
        if (at != entries.end() && at->key == key) appendLine(output, "DUPLICATE");
        else entries.insert(at, Entry{key, priority});
    };
    auto erase = [&](int key) {
        input << "erase " << key << '\n';
        auto at = position(key);
        if (at == entries.end() || at->key != key) appendLine(output, "NOT_FOUND");
        else { entries.erase(at); appendLine(output, "REMOVED"); }
    };
    auto keys = [&]() {
        std::vector<int> values;
        for (const Entry& entry : entries) values.push_back(entry.key);
        return values;
    };
    auto root = [&]() {
        input << "root\n";
        if (entries.empty()) appendLine(output, "EMPTY");
        else appendLine(output, std::max_element(entries.begin(), entries.end(),
            [](const Entry& a, const Entry& b) { return a.priority < b.priority; })->key);
    };
    auto split = [&](int pivot) {
        input << "split " << pivot << '\n';
        std::vector<int> left, right;
        for (const Entry& entry : entries) (entry.key < pivot ? left : right).push_back(entry.key);
        appendSplit(output, left, right);
    };
    insert(10, 40); insert(5, 70); insert(15, 30); insert(7, 60);
    root(); split(7);
    input << "rank 10\n"; appendLine(output, 2);
    input << "kth 2\n"; appendLine(output, 7);
    erase(5); root();
    input << "contains 5\n"; appendLine(output, "false");
    input << "size\n"; appendLine(output, 3);
    erase(99);
    input << "inorder\n"; appendIntList(output, keys());
    int emitted = 14;
    int priority = 1000;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 8);
        const int key = randomInt(random, -30, 30);
        if (kind <= 2) insert(key, priority++);
        else if (kind == 3) erase(key);
        else if (kind == 4) root();
        else if (kind == 5) split(key);
        else if (kind == 6) {
            input << "rank " << key << '\n';
            appendLine(output, static_cast<int>(position(key) - entries.begin()));
        } else if (kind == 7) {
            const int k = randomInt(random, 0, static_cast<int>(entries.size()) + 1);
            input << "kth " << k << '\n';
            if (k < 1 || k > static_cast<int>(entries.size())) appendLine(output, "OUT_OF_RANGE");
            else appendLine(output, entries[static_cast<std::size_t>(k - 1)].key);
        } else {
            input << "size\n"; appendLine(output, static_cast<int>(entries.size()));
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A39: implicit Treap sequence --------------------------------------

GeneratedCase generateImplicitTreapSequence(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 12) {
        throw std::runtime_error("A39-implicit-treap-sequence operation_limit is too small");
    }
    std::vector<int> sequence;
    std::ostringstream input, output;
    input << operationCount << '\n';
    auto pushBack = [&](int value) { input << "push_back " << value << '\n'; sequence.push_back(value); };
    auto reverse = [&](int left, int right) {
        input << "reverse " << left << ' ' << right << '\n';
        if (left < 0 || right >= static_cast<int>(sequence.size()) || left > right) appendLine(output, "OUT_OF_RANGE");
        else std::reverse(sequence.begin() + left, sequence.begin() + right + 1);
    };
    auto rangeSum = [&](int left, int right) {
        input << "range_sum " << left << ' ' << right << '\n';
        if (left < 0 || right >= static_cast<int>(sequence.size()) || left > right) appendLine(output, "OUT_OF_RANGE");
        else { int sum = 0; for (int i = left; i <= right; ++i) sum += sequence[static_cast<std::size_t>(i)]; appendLine(output, sum); }
    };
    for (int value = 1; value <= 5; ++value) pushBack(value);
    reverse(1, 3);
    input << "print\n"; appendIntList(output, sequence);
    rangeSum(1, 3);
    input << "get 2\n"; appendLine(output, sequence[2]);
    input << "erase 2\n"; appendLine(output, sequence[2]); sequence.erase(sequence.begin() + 2);
    input << "insert 1 9\n"; sequence.insert(sequence.begin() + 1, 9);
    input << "print\n"; appendIntList(output, sequence);
    int emitted = 12;
    while (emitted < operationCount) {
        const int kind = randomInt(random, 0, 7);
        if (kind <= 1) pushBack(randomInt(random, -20, 20));
        else if (kind == 2) {
            const int position = randomInt(random, 0, static_cast<int>(sequence.size()));
            const int value = randomInt(random, -20, 20);
            input << "insert " << position << ' ' << value << '\n';
            sequence.insert(sequence.begin() + position, value);
        } else if (kind == 3) {
            const int position = sequence.empty() ? 0 : randomInt(random, 0, static_cast<int>(sequence.size()) - 1);
            input << "erase " << position << '\n';
            if (sequence.empty()) appendLine(output, "OUT_OF_RANGE");
            else { appendLine(output, sequence[static_cast<std::size_t>(position)]); sequence.erase(sequence.begin() + position); }
        } else if (kind == 4) {
            if (sequence.empty()) reverse(0, 0);
            else { int a=randomInt(random,0,static_cast<int>(sequence.size())-1),b=randomInt(random,0,static_cast<int>(sequence.size())-1); if(a>b)std::swap(a,b); reverse(a,b); }
        } else if (kind == 5) {
            if (sequence.empty()) rangeSum(0, 0);
            else { int a=randomInt(random,0,static_cast<int>(sequence.size())-1),b=randomInt(random,0,static_cast<int>(sequence.size())-1); if(a>b)std::swap(a,b); rangeSum(a,b); }
        } else if (kind == 6) {
            input << "size\n"; appendLine(output, static_cast<int>(sequence.size()));
        } else {
            input << "print\n"; appendIntList(output, sequence);
        }
        ++emitted;
    }
    return {input.str(), output.str()};
}

// ----- A40: Splay access --------------------------------------------------

GeneratedCase generateSplayTreeAccess(
    std::mt19937_64& random,
    int operationCount
) {
    if (operationCount < 13) {
        throw std::runtime_error("A40-splay-tree-access operation_limit is too small");
    }
    OrderedSet set;
    std::ostringstream input, output;
    input << operationCount << '\n';
    for (int key : {10, 5, 15, 2}) { input << "insert " << key << '\n'; set.insert(key); }
    input << "root\n"; appendLine(output, 2);
    input << "access 10\n"; appendLine(output, "FOUND");
    input << "root\n"; appendLine(output, 10);
    input << "access 7\n"; appendLine(output, "MISS");
    input << "root\n"; appendLine(output, 5);
    input << "contains 15\n"; appendLine(output, "true");
    input << "root\n"; appendLine(output, 15);
    input << "erase 5\n"; set.erase(5); appendLine(output, "REMOVED");
    input << "inorder\n"; appendIntList(output, set.keys);
    int emitted = 13;
    auto insert = [&](int key) { input << "insert " << key << '\n'; if (!set.insert(key)) appendLine(output, "DUPLICATE"); };
    auto erase = [&](int key) { input << "erase " << key << '\n'; appendLine(output, set.erase(key) ? "REMOVED" : "NOT_FOUND"); };
    while (emitted < operationCount) {
        const int kind=randomInt(random,0,7), key=randomInt(random,-30,30);
        if (kind <= 2) insert(key);
        else if (kind == 3) erase(key);
        else if (kind == 4) { input << "contains " << key << '\n'; appendLine(output,set.contains(key)?"true":"false"); }
        else if (kind == 5) { input << "rank " << key << '\n'; appendLine(output,set.rank(key)); }
        else if (kind == 6) { int k=randomInt(random,0,set.size()+1); input<<"kth "<<k<<'\n'; if(k<1||k>set.size())appendLine(output,"OUT_OF_RANGE");else appendLine(output,set.keys[static_cast<std::size_t>(k-1)]); }
        else { input << "size\n"; appendLine(output,set.size()); }
        ++emitted;
    }
    return {input.str(),output.str()};
}

struct HeapModels {
    std::vector<std::vector<int>> heaps;
    explicit HeapModels(int count) : heaps(static_cast<std::size_t>(count)) {}
    void push(int heap, int key) { heaps[static_cast<std::size_t>(heap)].push_back(key); }
    void meld(int destination, int source) {
        if (destination == source) return;
        auto& to=heaps[static_cast<std::size_t>(destination)]; auto& from=heaps[static_cast<std::size_t>(source)];
        to.insert(to.end(),from.begin(),from.end()); from.clear();
    }
    int pop(int heap) {
        auto& values=heaps[static_cast<std::size_t>(heap)];
        auto at=std::min_element(values.begin(),values.end()); int value=*at; values.erase(at); return value;
    }
};

void generateHeapTail(
    std::mt19937_64& random,
    int operationCount,
    int& emitted,
    HeapModels& model,
    std::ostringstream& input,
    std::ostringstream& output,
    const std::string& minimumCommand
) {
    const int heapCount=static_cast<int>(model.heaps.size());
    while (emitted < operationCount) {
        const int kind=randomInt(random,0,5), heap=randomInt(random,0,heapCount-1);
        if (kind <= 1) { int key=randomInt(random,-30,30); input<<"push "<<heap<<' '<<key<<'\n'; model.push(heap,key); }
        else if (kind == 2) { int source=randomInt(random,0,heapCount-1); input<<"meld "<<heap<<' '<<source<<'\n'; model.meld(heap,source); }
        else if (kind == 3) { input<<minimumCommand<<' '<<heap<<'\n'; auto& values=model.heaps[static_cast<std::size_t>(heap)]; if(values.empty())appendLine(output,"EMPTY");else appendLine(output,*std::min_element(values.begin(),values.end())); }
        else if (kind == 4) { input<<(minimumCommand=="min"?"extract_min":"pop")<<' '<<heap<<'\n'; auto& values=model.heaps[static_cast<std::size_t>(heap)]; if(values.empty())appendLine(output,"EMPTY");else appendLine(output,model.pop(heap)); }
        else { input<<"size "<<heap<<'\n'; appendLine(output,static_cast<int>(model.heaps[static_cast<std::size_t>(heap)].size())); }
        ++emitted;
    }
}

// ----- A41 / A42: meldable heaps -----------------------------------------

GeneratedCase generateLeftistHeapMeld(std::mt19937_64& random,int operationCount) {
    if(operationCount<13)throw std::runtime_error("A41-leftist-heap-meld operation_limit is too small");
    HeapModels model(3);std::ostringstream input,output;input<<"3 "<<operationCount<<'\n';
    for(int key:{1,4,7,10}){input<<"push 0 "<<key<<'\n';model.push(0,key);}for(int key:{2,3,5,6}){input<<"push 1 "<<key<<'\n';model.push(1,key);}
    input<<"meld 0 1\n";model.meld(0,1);input<<"npl 0\n";appendLine(output,1);input<<"preorder 0\n";appendLine(output,"1 2 5 6 7 10 3 4");input<<"size 0\n";appendLine(output,8);input<<"size 1\n";appendLine(output,0);
    int emitted=13;generateHeapTail(random,operationCount,emitted,model,input,output,"top");return{input.str(),output.str()};
}

GeneratedCase generateSkewHeapMeld(std::mt19937_64& random,int operationCount) {
    if(operationCount<12)throw std::runtime_error("A42-skew-heap-meld operation_limit is too small");
    HeapModels model(3);std::ostringstream input,output;input<<"3 "<<operationCount<<'\n';
    for(int key:{1,4,7,10}){input<<"push 0 "<<key<<'\n';model.push(0,key);}for(int key:{2,3,5,6}){input<<"push 1 "<<key<<'\n';model.push(1,key);}
    input<<"meld 0 1\n";model.meld(0,1);input<<"preorder 0\n";appendLine(output,"1 2 5 7 3 6 4 10");input<<"size 0\n";appendLine(output,8);input<<"size 1\n";appendLine(output,0);
    int emitted=12;generateHeapTail(random,operationCount,emitted,model,input,output,"top");return{input.str(),output.str()};
}

// ----- A43: binomial heap -------------------------------------------------

GeneratedCase generateBinomialHeap(std::mt19937_64& random,int operationCount) {
    if(operationCount<12)throw std::runtime_error("A43-binomial-heap operation_limit is too small");
    HeapModels model(3);std::ostringstream input,output;input<<"3 "<<operationCount<<'\n';
    for(int key:{8,7,6,5,4,3,2}){input<<"push 0 "<<key<<'\n';model.push(0,key);}
    input<<"root_degrees 0\n";appendLine(output,"0 1 2");input<<"push 1 0\n";model.push(1,0);input<<"meld 0 1\n";model.meld(0,1);input<<"root_degrees 0\n";appendLine(output,3);input<<"size 1\n";appendLine(output,0);
    int emitted=12;generateHeapTail(random,operationCount,emitted,model,input,output,"min");return{input.str(),output.str()};
}

// ----- A44: duplicate-aware order-statistic tree -------------------------

GeneratedCase generateOrderStatisticMultiset(std::mt19937_64& random,int operationCount) {
    if(operationCount<13)throw std::runtime_error("A44-order-statistic-multiset operation_limit is too small");
    std::vector<int> values;std::ostringstream input,output;input<<operationCount<<'\n';
    auto insert=[&](int key){input<<"insert "<<key<<'\n';values.insert(std::upper_bound(values.begin(),values.end(),key),key);};
    auto erase=[&](int key){input<<"erase_one "<<key<<'\n';auto at=std::lower_bound(values.begin(),values.end(),key);if(at==values.end()||*at!=key)appendLine(output,"NOT_FOUND");else{values.erase(at);appendLine(output,"REMOVED");}};
    for (int key : {5, 2, 5, 8, 5}) insert(key);
    input << "count 5\n"; appendLine(output, 3);
    input << "order_of_key 5\n"; appendLine(output, 1);
    input << "kth 4\n"; appendLine(output, 5);
    input << "range_count 5 8\n"; appendLine(output, 4);
    erase(5);
    input << "count 5\n"; appendLine(output, 2);
    input << "size\n"; appendLine(output, 4);
    input << "print\n"; appendIntList(output, values);
    int emitted=13;while(emitted<operationCount){int kind=randomInt(random,0,8),key=randomInt(random,-20,20);if(kind<=2)insert(key);else if(kind==3)erase(key);else if(kind==4){input<<"count "<<key<<'\n';appendLine(output,static_cast<int>(std::upper_bound(values.begin(),values.end(),key)-std::lower_bound(values.begin(),values.end(),key)));}else if(kind==5){input<<"order_of_key "<<key<<'\n';appendLine(output,static_cast<int>(std::lower_bound(values.begin(),values.end(),key)-values.begin()));}else if(kind==6){int k=randomInt(random,0,static_cast<int>(values.size())+1);input<<"kth "<<k<<'\n';if(k<1||k>static_cast<int>(values.size()))appendLine(output,"OUT_OF_RANGE");else appendLine(output,values[static_cast<std::size_t>(k-1)]);}else if(kind==7){int high=randomInt(random,-20,20);input<<"range_count "<<key<<' '<<high<<'\n';appendLine(output,key>high?0:static_cast<int>(std::upper_bound(values.begin(),values.end(),high)-std::lower_bound(values.begin(),values.end(),key)));}else{input<<"size\n";appendLine(output,static_cast<int>(values.size()));}++emitted;}return{input.str(),output.str()};
}

}  // namespace

void registerBalancedTreeGenerators(CaseGeneratorRegistry& registry) {
    registry.emplace("A36-order-statistic-avl", generateOrderStatisticAvl);
    registry.emplace(
        "A37-order-statistic-red-black",
        generateOrderStatisticRedBlack
    );
    registry.emplace("A38-treap-split-merge", generateTreapSplitMerge);
    registry.emplace(
        "A39-implicit-treap-sequence",
        generateImplicitTreapSequence
    );
    registry.emplace("A40-splay-tree-access", generateSplayTreeAccess);
    registry.emplace("A41-leftist-heap-meld", generateLeftistHeapMeld);
    registry.emplace("A42-skew-heap-meld", generateSkewHeapMeld);
    registry.emplace("A43-binomial-heap", generateBinomialHeap);
    registry.emplace(
        "A44-order-statistic-multiset",
        generateOrderStatisticMultiset
    );
}

}  // namespace judge::generators
