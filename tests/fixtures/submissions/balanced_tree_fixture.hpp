#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#ifndef BALANCED_TREE_BUG
#define BALANCED_TREE_BUG 0
#endif

namespace balanced_fixture {

template <class T>
void printValues(const std::vector<T>& values) {
    if (values.empty()) {
        std::cout << "EMPTY\n";
        return;
    }
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i) std::cout << ' ';
        std::cout << values[i];
    }
    std::cout << '\n';
}

#if BALANCED_TREE_MODE == 37

int run() {
    int operationCount = 0;
    std::cin >> operationCount;
    std::vector<int> keys;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key;
            std::cin >> key;
            auto position = std::lower_bound(keys.begin(), keys.end(), key);
            if (position != keys.end() && *position == key) std::cout << "DUPLICATE\n";
            else keys.insert(position, key);
        } else if (command == "erase") {
            int key;
            std::cin >> key;
            auto position = std::lower_bound(keys.begin(), keys.end(), key);
            if (position == keys.end() || *position != key) {
                std::cout << "NOT_FOUND\n";
            } else {
                std::cout << "REMOVED\n";
                if (BALANCED_TREE_BUG != 2) keys.erase(position);
            }
        } else if (command == "contains") {
            int key;
            std::cin >> key;
            std::cout << (std::binary_search(keys.begin(), keys.end(), key) ? "true" : "false") << '\n';
        } else if (command == "select") {
            int k;
            std::cin >> k;
            if (k < 1 || k > static_cast<int>(keys.size())) std::cout << "OUT_OF_RANGE\n";
            else std::cout << keys[static_cast<std::size_t>(k - 1)] << '\n';
        } else if (command == "rank") {
            int key;
            std::cin >> key;
            auto position = BALANCED_TREE_BUG == 1
                ? std::upper_bound(keys.begin(), keys.end(), key)
                : std::lower_bound(keys.begin(), keys.end(), key);
            std::cout << (position - keys.begin()) << '\n';
        } else if (command == "range_count") {
            int low, high;
            std::cin >> low >> high;
            std::cout << (low > high ? 0 : std::upper_bound(keys.begin(), keys.end(), high) -
                std::lower_bound(keys.begin(), keys.end(), low)) << '\n';
        } else if (command == "predecessor") {
            int key;
            std::cin >> key;
            auto position = std::lower_bound(keys.begin(), keys.end(), key);
            if (position == keys.begin()) std::cout << "NONE\n";
            else std::cout << *--position << '\n';
        } else if (command == "successor") {
            int key;
            std::cin >> key;
            auto position = std::upper_bound(keys.begin(), keys.end(), key);
            if (position == keys.end()) std::cout << "NONE\n";
            else std::cout << *position << '\n';
        } else if (command == "min" || command == "max") {
            if (keys.empty()) std::cout << "EMPTY\n";
            else std::cout << (command == "min" ? keys.front() : keys.back()) << '\n';
        } else if (command == "size") {
            std::cout << keys.size() << '\n';
        } else if (command == "inorder") {
            printValues(keys);
        } else if (command == "clear") {
            keys.clear();
        }
    }
    return 0;
}

#elif BALANCED_TREE_MODE == 38

struct TreapNode {
    int key;
    int priority;
    int size;
    TreapNode* left;
    TreapNode* right;
};

int size(TreapNode* node) { return node ? node->size : 0; }
void update(TreapNode* node) { if (node) node->size = 1 + size(node->left) + size(node->right); }

void split(TreapNode* node, int key, TreapNode*& left, TreapNode*& right) {
    if (!node) { left = right = nullptr; return; }
    const bool goesLeft = BALANCED_TREE_BUG == 2 ? node->key <= key : node->key < key;
    if (goesLeft) {
        left = node;
        split(node->right, key, left->right, right);
        update(left);
    } else {
        right = node;
        split(node->left, key, left, right->left);
        update(right);
    }
}

TreapNode* merge(TreapNode* left, TreapNode* right) {
    if (!left) return right;
    if (!right) return left;
    const bool chooseLeft = BALANCED_TREE_BUG == 1
        ? left->priority < right->priority
        : left->priority > right->priority;
    if (chooseLeft) {
        left->right = merge(left->right, right);
        update(left);
        return left;
    }
    right->left = merge(left, right->left);
    update(right);
    return right;
}

bool contains(TreapNode* node, int key) {
    while (node) {
        if (node->key == key) return true;
        node = key < node->key ? node->left : node->right;
    }
    return false;
}

TreapNode* insert(TreapNode* root, int key, int priority) {
    TreapNode* left = nullptr;
    TreapNode* right = nullptr;
    split(root, key, left, right);
    return merge(merge(left, new TreapNode{key, priority, 1, nullptr, nullptr}), right);
}

TreapNode* erase(TreapNode* node, int key) {
    if (!node) return nullptr;
    if (node->key == key) {
        TreapNode* result = merge(node->left, node->right);
        delete node;
        return result;
    }
    if (key < node->key) node->left = erase(node->left, key);
    else node->right = erase(node->right, key);
    update(node);
    return node;
}

void inorder(TreapNode* node, std::vector<int>& values) {
    if (!node) return;
    inorder(node->left, values);
    values.push_back(node->key);
    inorder(node->right, values);
}

int rank(TreapNode* node, int key) {
    if (!node) return 0;
    if (key <= node->key) return rank(node->left, key);
    return size(node->left) + 1 + rank(node->right, key);
}

TreapNode* kth(TreapNode* node, int k) {
    const int leftSize = size(node->left);
    if (k == leftSize + 1) return node;
    if (k <= leftSize) return kth(node->left, k);
    return kth(node->right, k - leftSize - 1);
}

void destroy(TreapNode* node) {
    if (!node) return;
    destroy(node->left); destroy(node->right); delete node;
}

int run() {
    int operationCount = 0;
    std::cin >> operationCount;
    TreapNode* root = nullptr;
    for (int operation = 0; operation < operationCount; ++operation) {
        std::string command;
        std::cin >> command;
        if (command == "insert") {
            int key, priority;
            std::cin >> key >> priority;
            if (contains(root, key)) std::cout << "DUPLICATE\n";
            else root = insert(root, key, priority);
        } else if (command == "erase") {
            int key;
            std::cin >> key;
            if (!contains(root, key)) std::cout << "NOT_FOUND\n";
            else { root = erase(root, key); std::cout << "REMOVED\n"; }
        } else if (command == "contains") {
            int key; std::cin >> key;
            std::cout << (contains(root, key) ? "true" : "false") << '\n';
        } else if (command == "root") {
            if (!root) std::cout << "EMPTY\n"; else std::cout << root->key << '\n';
        } else if (command == "split") {
            int pivot; std::cin >> pivot;
            TreapNode* left = nullptr; TreapNode* right = nullptr;
            split(root, pivot, left, right);
            std::vector<int> a, b; inorder(left, a); inorder(right, b);
            if (a.empty()) std::cout << "EMPTY"; else for (std::size_t i=0;i<a.size();++i) std::cout << (i?" ":"") << a[i];
            std::cout << " | ";
            if (b.empty()) std::cout << "EMPTY"; else for (std::size_t i=0;i<b.size();++i) std::cout << (i?" ":"") << b[i];
            std::cout << '\n';
            root = merge(left, right);
        } else if (command == "rank") {
            int key; std::cin >> key; std::cout << rank(root, key) << '\n';
        } else if (command == "kth") {
            int k; std::cin >> k;
            if (k < 1 || k > size(root)) std::cout << "OUT_OF_RANGE\n";
            else std::cout << kth(root, k)->key << '\n';
        } else if (command == "size") {
            std::cout << size(root) << '\n';
        } else if (command == "inorder") {
            std::vector<int> values; inorder(root, values); printValues(values);
        } else if (command == "clear") {
            destroy(root); root = nullptr;
        }
    }
    destroy(root);
    return 0;
}

#elif BALANCED_TREE_MODE == 39

struct ImplicitNode {
    int value;
    unsigned priority;
    int size;
    long long sum;
    bool reversed;
    ImplicitNode* left;
    ImplicitNode* right;
};

unsigned nextPriority() {
    static unsigned state = 0x9e3779b9U;
    state ^= state << 13; state ^= state >> 17; state ^= state << 5;
    return state;
}
int size(ImplicitNode* node) { return node ? node->size : 0; }
long long sum(ImplicitNode* node) { return node ? node->sum : 0; }
void update(ImplicitNode* node) { if (node) { node->size=1+size(node->left)+size(node->right); node->sum=node->value+sum(node->left)+sum(node->right); } }
void push(ImplicitNode* node) {
    if (!node || !node->reversed) return;
    std::swap(node->left, node->right);
    if (node->left) node->left->reversed = !node->left->reversed;
    if (node->right) node->right->reversed = !node->right->reversed;
    node->reversed = false;
}
void split(ImplicitNode* node, int count, ImplicitNode*& left, ImplicitNode*& right) {
    if (!node) { left=right=nullptr; return; }
    push(node);
    if (size(node->left) >= count) { right=node; split(node->left,count,left,right->left); update(right); }
    else { left=node; split(node->right,count-size(node->left)-1,left->right,right); update(left); }
}
ImplicitNode* merge(ImplicitNode* left, ImplicitNode* right) {
    if (!left) return right; if (!right) return left;
    push(left); push(right);
    if (left->priority > right->priority) { left->right=merge(left->right,right); update(left); return left; }
    right->left=merge(left,right->left); update(right); return right;
}
void collect(ImplicitNode* node, std::vector<int>& values) { if(!node)return; push(node); collect(node->left,values); values.push_back(node->value); collect(node->right,values); }
void destroy(ImplicitNode* node) { if(!node)return; destroy(node->left); destroy(node->right); delete node; }

int run() {
    int operationCount=0; std::cin>>operationCount; ImplicitNode* root=nullptr;
    auto insertAt=[&](int position,int value){ ImplicitNode *a,*b; split(root,position,a,b); root=merge(merge(a,new ImplicitNode{value,nextPriority(),1,value,false,nullptr,nullptr}),b); };
    for(int operation=0;operation<operationCount;++operation){
        std::string command; std::cin>>command;
        if(command=="push_back"){int value;std::cin>>value;insertAt(size(root),value);}
        else if(command=="insert"){int position,value;std::cin>>position>>value;if(position<0||position>size(root))std::cout<<"OUT_OF_RANGE\n";else insertAt(position,value);}
        else if(command=="erase"){int position;std::cin>>position;if(position<0||position>=size(root))std::cout<<"OUT_OF_RANGE\n";else{ImplicitNode *a,*b,*c;split(root,position,a,b);split(b,1,b,c);std::cout<<b->value<<'\n';delete b;root=merge(a,c);}}
        else if(command=="reverse"){int left,right;std::cin>>left>>right;if(left<0||right>=size(root)||left>right)std::cout<<"OUT_OF_RANGE\n";else{ImplicitNode *a,*b,*c;split(root,left,a,b);int length=right-left+(BALANCED_TREE_BUG==1?0:1);split(b,length,b,c);if(b)b->reversed=!b->reversed;root=merge(a,merge(b,c));}}
        else if(command=="range_sum"){int left,right;std::cin>>left>>right;if(left<0||right>=size(root)||left>right)std::cout<<"OUT_OF_RANGE\n";else{ImplicitNode *a,*b,*c;split(root,left,a,b);int length=right-left+(BALANCED_TREE_BUG==2?0:1);split(b,length,b,c);std::cout<<sum(b)<<'\n';root=merge(a,merge(b,c));}}
        else if(command=="get"){int position;std::cin>>position;if(position<0||position>=size(root))std::cout<<"OUT_OF_RANGE\n";else{ImplicitNode *a,*b,*c;split(root,position,a,b);split(b,1,b,c);std::cout<<b->value<<'\n';root=merge(a,merge(b,c));}}
        else if(command=="size")std::cout<<size(root)<<'\n';
        else if(command=="print"){std::vector<int> values;collect(root,values);printValues(values);}
        else if(command=="clear"){destroy(root);root=nullptr;}
    }
    destroy(root); return 0;
}

#elif BALANCED_TREE_MODE == 40

struct SplayNode { int key,size; SplayNode *left,*right,*parent; };
int size(SplayNode* node){return node?node->size:0;}
void update(SplayNode* node){if(node)node->size=1+size(node->left)+size(node->right);}
void attachParent(SplayNode* child,SplayNode* parent){if(child)child->parent=parent;}
void rotate(SplayNode*& root,SplayNode* x){SplayNode* p=x->parent;SplayNode* g=p->parent;if(x==p->left){p->left=x->right;attachParent(p->left,p);x->right=p;}else{p->right=x->left;attachParent(p->right,p);x->left=p;}p->parent=x;x->parent=g;if(!g)root=x;else if(g->left==p)g->left=x;else g->right=x;update(p);update(x);}
void splay(SplayNode*& root,SplayNode* x){if(BALANCED_TREE_BUG==1)return;while(x&&x->parent){SplayNode* p=x->parent;SplayNode* g=p->parent;if(!g)rotate(root,x);else if((g->left==p)==(p->left==x)){rotate(root,p);rotate(root,x);}else{rotate(root,x);rotate(root,x);}}}
SplayNode* find(SplayNode*& root,int key){SplayNode* node=root;SplayNode* last=nullptr;while(node){last=node;if(key==node->key)break;node=key<node->key?node->left:node->right;}if(last)splay(root,last);return root&&root->key==key?root:nullptr;}
void destroy(SplayNode* node){if(!node)return;destroy(node->left);destroy(node->right);delete node;}
void inorder(SplayNode* node,std::vector<int>& values){if(!node)return;inorder(node->left,values);values.push_back(node->key);inorder(node->right,values);}
int rank(SplayNode* node,int key){if(!node)return 0;if(key<=node->key)return rank(node->left,key);return size(node->left)+1+rank(node->right,key);}
SplayNode* kth(SplayNode* node,int k){while(node){int left=size(node->left);if(k==left+1)return node;if(k<=left)node=node->left;else{k-=left+1;node=node->right;}}return nullptr;}

int run(){int operationCount=0;std::cin>>operationCount;SplayNode* root=nullptr;for(int operation=0;operation<operationCount;++operation){std::string command;std::cin>>command;
    if(command=="insert"){int key;std::cin>>key;if(BALANCED_TREE_BUG==1){SplayNode* parent=nullptr;SplayNode* cursor=root;while(cursor&&cursor->key!=key){parent=cursor;cursor=key<cursor->key?cursor->left:cursor->right;}if(cursor)std::cout<<"DUPLICATE\n";else{SplayNode* node=new SplayNode{key,1,nullptr,nullptr,parent};if(!parent)root=node;else if(key<parent->key)parent->left=node;else parent->right=node;for(SplayNode* p=parent;p;p=p->parent)update(p);}}else if(find(root,key))std::cout<<"DUPLICATE\n";else if(!root)root=new SplayNode{key,1,nullptr,nullptr,nullptr};else{SplayNode* node=new SplayNode{key,1,nullptr,nullptr,nullptr};if(key<root->key){node->left=root->left;attachParent(node->left,node);node->right=root;root->parent=node;root->left=nullptr;}else{node->right=root->right;attachParent(node->right,node);node->left=root;root->parent=node;root->right=nullptr;}update(root);update(node);root=node;}}
    else if(command=="access"||command=="contains"){int key;std::cin>>key;bool found=find(root,key)!=nullptr;if(command=="access")std::cout<<(found?"FOUND":"MISS")<<'\n';else std::cout<<(found?"true":"false")<<'\n';}
    else if(command=="erase"){int key;std::cin>>key;if(!find(root,key))std::cout<<"NOT_FOUND\n";else{std::cout<<"REMOVED\n";if(BALANCED_TREE_BUG==2)continue;SplayNode* left=root->left;SplayNode* right=root->right;delete root;if(left)left->parent=nullptr;if(right)right->parent=nullptr;if(!left)root=right;else{root=left;SplayNode* maximum=left;while(maximum->right)maximum=maximum->right;splay(root,maximum);root->right=right;attachParent(right,root);update(root);}}}
    else if(command=="root"){if(root)std::cout<<root->key<<'\n';else std::cout<<"EMPTY\n";}
    else if(command=="rank"){int key;std::cin>>key;std::cout<<rank(root,key)<<'\n';}
    else if(command=="kth"){int k;std::cin>>k;if(k<1||k>size(root))std::cout<<"OUT_OF_RANGE\n";else{SplayNode* node=kth(root,k);std::cout<<node->key<<'\n';splay(root,node);}}
    else if(command=="size")std::cout<<size(root)<<'\n';
    else if(command=="inorder"){std::vector<int> values;inorder(root,values);printValues(values);}
    else if(command=="clear"){destroy(root);root=nullptr;}
}destroy(root);return 0;}

#elif BALANCED_TREE_MODE == 41

struct LeftistNode{int key,npl,size;LeftistNode* left;LeftistNode* right;};
int npl(LeftistNode* node){return node?node->npl:-1;}int heapSize(LeftistNode* node){return node?node->size:0;}
LeftistNode* meld(LeftistNode* a,LeftistNode* b){if(!a)return b;if(!b)return a;if(a->key>b->key)std::swap(a,b);a->right=meld(a->right,b);if(npl(a->left)<npl(a->right))std::swap(a->left,a->right);a->npl=1+(BALANCED_TREE_BUG==1?std::max(npl(a->left),npl(a->right)):std::min(npl(a->left),npl(a->right)));a->size=1+heapSize(a->left)+heapSize(a->right);return a;}
void preorder(LeftistNode* node,std::vector<int>& values){if(!node)return;values.push_back(node->key);preorder(node->left,values);preorder(node->right,values);}void destroy(LeftistNode* node){if(!node)return;destroy(node->left);destroy(node->right);delete node;}
int run(){int heapCount,operationCount;std::cin>>heapCount>>operationCount;std::vector<LeftistNode*> heaps(heapCount,nullptr);for(int operation=0;operation<operationCount;++operation){std::string command;std::cin>>command;
    if(command=="push"){int h,key;std::cin>>h>>key;heaps[h]=meld(heaps[h],new LeftistNode{key,0,1,nullptr,nullptr});}
    else if(command=="meld"){int destination,source;std::cin>>destination>>source;if(destination!=source){if(BALANCED_TREE_BUG==2){destroy(heaps[source]);heaps[source]=nullptr;}else{heaps[destination]=meld(heaps[destination],heaps[source]);heaps[source]=nullptr;}}}
    else if(command=="top"){int h;std::cin>>h;if(heaps[h])std::cout<<heaps[h]->key<<'\n';else std::cout<<"EMPTY\n";}
    else if(command=="pop"){int h;std::cin>>h;if(!heaps[h])std::cout<<"EMPTY\n";else{LeftistNode* old=heaps[h];std::cout<<old->key<<'\n';heaps[h]=meld(old->left,old->right);delete old;}}
    else if(command=="size"){int h;std::cin>>h;std::cout<<heapSize(heaps[h])<<'\n';}
    else if(command=="npl"){int h;std::cin>>h;std::cout<<npl(heaps[h])<<'\n';}
    else if(command=="preorder"){int h;std::cin>>h;std::vector<int> values;preorder(heaps[h],values);printValues(values);}
    else if(command=="clear"){int h;std::cin>>h;destroy(heaps[h]);heaps[h]=nullptr;}
}for(auto* heap:heaps)destroy(heap);return 0;}

#elif BALANCED_TREE_MODE == 42

struct SkewNode{int key,size;SkewNode* left;SkewNode* right;};int heapSize(SkewNode* node){return node?node->size:0;}
SkewNode* meld(SkewNode* a,SkewNode* b){if(!a)return b;if(!b)return a;if(a->key>b->key)std::swap(a,b);if(BALANCED_TREE_BUG==1){a->right=meld(a->right,b);}else{a->right=meld(a->right,b);std::swap(a->left,a->right);}a->size=1+heapSize(a->left)+heapSize(a->right);return a;}
SkewNode* clone(SkewNode* node){if(!node)return nullptr;return new SkewNode{node->key,node->size,clone(node->left),clone(node->right)};}
void preorder(SkewNode* node,std::vector<int>& values){if(!node)return;values.push_back(node->key);preorder(node->left,values);preorder(node->right,values);}void destroy(SkewNode* node){if(!node)return;destroy(node->left);destroy(node->right);delete node;}
int run(){int heapCount,operationCount;std::cin>>heapCount>>operationCount;std::vector<SkewNode*> heaps(heapCount,nullptr);for(int operation=0;operation<operationCount;++operation){std::string command;std::cin>>command;
    if(command=="push"){int h,key;std::cin>>h>>key;heaps[h]=meld(heaps[h],new SkewNode{key,1,nullptr,nullptr});}
    else if(command=="meld"){int destination,source;std::cin>>destination>>source;if(destination!=source){heaps[destination]=meld(heaps[destination],BALANCED_TREE_BUG==2?clone(heaps[source]):heaps[source]);if(BALANCED_TREE_BUG!=2)heaps[source]=nullptr;}}
    else if(command=="top"){int h;std::cin>>h;if(heaps[h])std::cout<<heaps[h]->key<<'\n';else std::cout<<"EMPTY\n";}
    else if(command=="pop"){int h;std::cin>>h;if(!heaps[h])std::cout<<"EMPTY\n";else{SkewNode* old=heaps[h];std::cout<<old->key<<'\n';heaps[h]=meld(old->left,old->right);delete old;}}
    else if(command=="size"){int h;std::cin>>h;std::cout<<heapSize(heaps[h])<<'\n';}
    else if(command=="preorder"){int h;std::cin>>h;std::vector<int> values;preorder(heaps[h],values);printValues(values);}
    else if(command=="clear"){int h;std::cin>>h;destroy(heaps[h]);heaps[h]=nullptr;}
}for(auto* heap:heaps)destroy(heap);return 0;}

#elif BALANCED_TREE_MODE == 43

struct BinomialNode{int key,degree;BinomialNode* parent;std::vector<BinomialNode*> children;};
void destroy(BinomialNode* node){if(!node)return;for(auto* child:node->children)destroy(child);delete node;}
BinomialNode* clone(BinomialNode* node,BinomialNode* parent=nullptr){if(!node)return nullptr;BinomialNode* copy=new BinomialNode{node->key,node->degree,parent,{}};for(auto* child:node->children)copy->children.push_back(clone(child,copy));return copy;}
void linkTrees(BinomialNode* parent,BinomialNode* child){child->parent=parent;parent->children.push_back(child);++parent->degree;}
void consolidate(std::vector<BinomialNode*>& roots){std::vector<BinomialNode*> byDegree(32,nullptr);for(BinomialNode* tree:roots){while(byDegree[tree->degree]){BinomialNode* other=byDegree[tree->degree];byDegree[tree->degree]=nullptr;if(other->key<tree->key)std::swap(tree,other);linkTrees(tree,other);}byDegree[tree->degree]=tree;}roots.clear();for(auto* tree:byDegree)if(tree)roots.push_back(tree);}
int heapSize(const std::vector<BinomialNode*>& roots){int total=0;for(auto* root:roots)total+=1<<root->degree;return total;}
int minimumIndex(const std::vector<BinomialNode*>& roots){int best=0;for(int i=1;i<static_cast<int>(roots.size());++i)if(roots[i]->key<roots[best]->key)best=i;return best;}
int run(){int heapCount,operationCount;std::cin>>heapCount>>operationCount;std::vector<std::vector<BinomialNode*>> heaps(heapCount);for(int operation=0;operation<operationCount;++operation){std::string command;std::cin>>command;
    if(command=="push"){int h,key;std::cin>>h>>key;heaps[h].push_back(new BinomialNode{key,0,nullptr,{}});consolidate(heaps[h]);}
    else if(command=="meld"){int destination,source;std::cin>>destination>>source;if(destination!=source){if(BALANCED_TREE_BUG==2){for(auto* root:heaps[source])heaps[destination].push_back(clone(root));}else heaps[destination].insert(heaps[destination].end(),heaps[source].begin(),heaps[source].end());consolidate(heaps[destination]);if(BALANCED_TREE_BUG!=2)heaps[source].clear();}}
    else if(command=="min"){int h;std::cin>>h;if(heaps[h].empty())std::cout<<"EMPTY\n";else std::cout<<heaps[h][minimumIndex(heaps[h])]->key<<'\n';}
    else if(command=="extract_min"){int h;std::cin>>h;if(heaps[h].empty())std::cout<<"EMPTY\n";else{int index=minimumIndex(heaps[h]);BinomialNode* root=heaps[h][index];std::cout<<root->key<<'\n';heaps[h].erase(heaps[h].begin()+index);for(auto* child:root->children){child->parent=nullptr;heaps[h].push_back(child);}root->children.clear();delete root;consolidate(heaps[h]);}}
    else if(command=="size"){int h;std::cin>>h;std::cout<<heapSize(heaps[h])<<'\n';}
    else if(command=="root_degrees"){int h;std::cin>>h;std::vector<int> degrees;for(auto* root:heaps[h])degrees.push_back(root->degree);if(BALANCED_TREE_BUG==1)std::reverse(degrees.begin(),degrees.end());printValues(degrees);}
    else if(command=="clear"){int h;std::cin>>h;for(auto* root:heaps[h])destroy(root);heaps[h].clear();}
}for(auto& heap:heaps)for(auto* root:heap)destroy(root);return 0;}

#elif BALANCED_TREE_MODE == 44

struct OrderNode{int key,count,size;unsigned priority;OrderNode* left;OrderNode* right;};unsigned nextPriority(){static unsigned state=0x243f6a88U;state^=state<<13;state^=state>>17;state^=state<<5;return state;}int size(OrderNode* node){return node?node->size:0;}void update(OrderNode* node){if(node)node->size=node->count+size(node->left)+size(node->right);}
OrderNode* rotateRight(OrderNode* node){OrderNode* child=node->left;node->left=child->right;child->right=node;update(node);update(child);return child;}OrderNode* rotateLeft(OrderNode* node){OrderNode* child=node->right;node->right=child->left;child->left=node;update(node);update(child);return child;}
OrderNode* insert(OrderNode* node,int key){if(!node)return new OrderNode{key,1,1,nextPriority(),nullptr,nullptr};if(key==node->key)++node->count;else if(key<node->key){node->left=insert(node->left,key);if(node->left->priority>node->priority)node=rotateRight(node);}else{node->right=insert(node->right,key);if(node->right->priority>node->priority)node=rotateLeft(node);}update(node);return node;}
OrderNode* eraseOne(OrderNode* node,int key,bool& removed){if(!node)return nullptr;if(key<node->key)node->left=eraseOne(node->left,key,removed);else if(key>node->key)node->right=eraseOne(node->right,key,removed);else{removed=true;if(node->count>1&&BALANCED_TREE_BUG!=2)--node->count;else if(!node->left||!node->right){OrderNode* child=node->left?node->left:node->right;delete node;return child;}else if(node->left->priority>node->right->priority){node=rotateRight(node);node->right=eraseOne(node->right,key,removed);}else{node=rotateLeft(node);node->left=eraseOne(node->left,key,removed);}}update(node);return node;}
int count(OrderNode* node,int key){while(node){if(key==node->key)return node->count;node=key<node->key?node->left:node->right;}return 0;}int less(OrderNode* node,int key){if(!node)return 0;if(key<=node->key)return less(node->left,key);return size(node->left)+node->count+less(node->right,key);}int atMost(OrderNode* node,int key){if(!node)return 0;if(key<node->key)return atMost(node->left,key);return size(node->left)+node->count+atMost(node->right,key);}OrderNode* kth(OrderNode* node,int k){int left=size(node->left);if(k<=left)return kth(node->left,k);if(k<=left+node->count)return node;return kth(node->right,k-left-node->count);}void collect(OrderNode* node,std::vector<int>& values){if(!node)return;collect(node->left,values);for(int i=0;i<node->count;++i)values.push_back(node->key);collect(node->right,values);}void destroy(OrderNode* node){if(!node)return;destroy(node->left);destroy(node->right);delete node;}
int run(){int operationCount=0;std::cin>>operationCount;OrderNode* root=nullptr;for(int operation=0;operation<operationCount;++operation){std::string command;std::cin>>command;
    if(command=="insert"){int key;std::cin>>key;root=insert(root,key);}
    else if(command=="erase_one"){int key;std::cin>>key;bool removed=false;root=eraseOne(root,key,removed);std::cout<<(removed?"REMOVED":"NOT_FOUND")<<'\n';}
    else if(command=="count"){int key;std::cin>>key;std::cout<<count(root,key)<<'\n';}
    else if(command=="kth"){int k;std::cin>>k;if(k<1||k>size(root))std::cout<<"OUT_OF_RANGE\n";else std::cout<<kth(root,k)->key<<'\n';}
    else if(command=="order_of_key"){int key;std::cin>>key;std::cout<<(BALANCED_TREE_BUG==1?atMost(root,key):less(root,key))<<'\n';}
    else if(command=="range_count"){int low,high;std::cin>>low>>high;std::cout<<(low>high?0:atMost(root,high)-less(root,low))<<'\n';}
    else if(command=="lower_bound"||command=="upper_bound"){int key;std::cin>>key;int position=command=="lower_bound"?less(root,key)+1:atMost(root,key)+1;if(position>size(root))std::cout<<"NONE\n";else std::cout<<kth(root,position)->key<<'\n';}
    else if(command=="size")std::cout<<size(root)<<'\n';
    else if(command=="print"){std::vector<int> values;collect(root,values);printValues(values);}
    else if(command=="clear"){destroy(root);root=nullptr;}
}destroy(root);return 0;}

#endif

}  // namespace balanced_fixture

int main() { return balanced_fixture::run(); }
