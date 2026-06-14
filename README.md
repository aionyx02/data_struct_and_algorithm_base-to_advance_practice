# Data Structures Practice

這是一個以 C++20、CLion 與 CMake 建立的漸進式資料結構學習專案。

目前範圍只包含資料結構。題目主要依據 `資料結構/` 內的課程教材，再延伸到高等資料結構。
專案會先建立本地 CLI Judge，之後才增加 Web 練習介面；兩者共用同一套判題核心。

## Current Plan

- 68 題教材基礎。
- 64 題高等資料結構。
- ADT 實作題限制會直接完成核心工作的 STL 容器。
- Judge 驗證答案、invariant、時間、記憶體、禁止 API 與複雜度成長。

詳細規劃：

- `docs/learning-roadmap.md`
- `docs/judge-requirements.md`
- `docs/architecture.md`

## Context Workflow

```powershell
npm install
npm run team:status
npm run lint
npm run security:scan
npm test
npm run team:guard
npm run docs:refresh
npm run docs:ready
```

AI 或開發者開始工作前，先讀取 `CLAUDE.md`，再依 `docs/index.md` 路由到最小必要文件。

## C++ Workflow

使用 MinGW Debug preset：

```powershell
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

目前可練習的第一章：

| ID | 題目 | 難度 |
|---|---|---|
| `F01-array-adt` | Fixed Array ADT | D0 |
| `F02-dynamic-array` | Manual Dynamic Array | D1 |
| `F03-stack-array` | Fixed Capacity Array Stack | D1 |
| `F04-circular-queue` | Fixed Capacity Circular Queue | D1 |
| `F05-array-deque` | Fixed Capacity Array Deque | D2 |

第二章 Pointer-Based Linear Structures：

| ID | 題目 | 難度 |
|---|---|---|
| `F06-singly-linked-list` | Singly Linked List | D1 |
| `F07-linked-stack` | Linked Stack | D1 |
| `F08-linked-queue` | Linked Queue | D1 |
| `F09-doubly-linked-list` | Doubly Linked List | D2 |
| `F10-circular-linked-list` | Circular Singly Linked List | D2 |

第三章 List Transformations And Node Management：

| ID | 題目 | 難度 |
|---|---|---|
| `F11-singly-list-reverse` | Reverse A Singly Linked List | D2 |
| `F12-singly-list-concatenate` | Concatenate Singly Linked Lists | D2 |
| `F13-singly-list-split` | Split Singly Linked Lists | D2 |
| `F14-doubly-list-splice` | Splice Doubly Linked Lists | D3 |
| `F15-fixed-node-pool-list` | Fixed Node Pool List | D3 |

第四章 Complete List Representations：

| ID | 題目 | 難度 |
|---|---|---|
| `F16-sentinel-doubly-list` | Sentinel Doubly Linked List | D2 |
| `F17-cursor-doubly-list` | Cursor Doubly Linked List | D3 |

第五組 Arrays And Structures：

| ID | 題目 | 難度 |
|---|---|---|
| `F18-record-table` | Fixed Capacity Record Table | D1 |
| `F19-sparse-polynomial` | Sparse Polynomial Terms | D2 |
| `F20-triplet-sparse-matrix` | Triplet Sparse Matrix | D2 |
| `F21-csr-sparse-matrix` | Compressed Sparse Row Matrix | D3 |
| `F22-row-major-matrix` | Two-Dimensional Row-Major Array | D1 |
| `F23-row-major-tensor` | Three-Dimensional Row-Major Array | D2 |

第六組 Stack And Queue Representations：

| ID | 題目 | 難度 |
|---|---|---|
| `F24-two-stacks-one-array` | Two Stacks In One Array | D2 |
| `F25-circular-linked-queue` | Circular Linked Queue | D2 |
| `F26-linked-deque` | Linked Deque | D2 |

第七組 Core Tree Representations：

| ID | 題目 | 難度 |
|---|---|---|
| `F27-array-binary-tree` | Array Binary Tree | D2 |
| `F28-linked-binary-tree` | Linked Binary Tree | D2 |
| `F29-inorder-threaded-tree` | Inorder Threaded Binary Tree | D3 |
| `F30-binary-min-heap` | Fixed Capacity Binary Min Heap | D2 |

第八組 Tree Traversal And Heap Views：

| ID | 題目 | 難度 |
|---|---|---|
| `F31-binary-tree-traversal-views` | Binary Tree Traversal Views | D2 |
| `F32-level-order-tree-view` | Level-Order Binary Tree View | D2 |
| `F33-bottom-up-min-heap-build` | Bottom-Up Binary Min Heap Build | D2 |
| `F34-min-heap-removal-trace` | Binary Min Heap Removal Trace | D2 |

第九組 Binary Search Tree Foundations：

| ID | 題目 | 難度 |
|---|---|---|
| `F35-bst-insert-search` | Binary Search Tree Insert And Search | D2 |
| `F36-bst-deletion-cases` | Binary Search Tree Deletion Cases | D2 |
| `F37-bst-predecessor-successor` | BST Predecessor And Successor | D2 |
| `F38-bst-range-view` | Binary Search Tree Range View | D2 |
| `F39-bst-structural-validation` | Binary Search Tree Structural Validation | D3 |
| `F40-bst-replacement-trace` | BST Replacement Node Trace | D3 |

第十組 Forest And Union-Find Foundations：

| ID | 題目 | 難度 |
|---|---|---|
| `F41-parent-array-forest` | Parent Array Forest | D2 |
| `F42-child-sibling-forest` | Child Sibling Forest | D2 |
| `F43-forest-traversal-views` | Forest Traversal Views | D2 |
| `F44-quick-union-disjoint-set` | Quick Union Disjoint Set | D2 |
| `F45-weighted-union-disjoint-set` | Weighted Union Disjoint Set | D2 |
| `F46-path-compression-trace` | Union Find Path Compression Trace | D3 |

第十一組 Graph ADT Representations：

| ID | 題目 | 難度 |
|---|---|---|
| `F47-undirected-adjacency-matrix` | Undirected Adjacency Matrix | D2 |
| `F48-directed-adjacency-matrix` | Directed Adjacency Matrix | D2 |
| `F49-undirected-adjacency-list` | Undirected Sorted Adjacency List | D2 |
| `F50-directed-adjacency-list` | Directed Sorted Adjacency List | D2 |
| `F51-fixed-edge-table-graph` | Fixed Capacity Edge Table Graph | D3 |

第十二組 Hash Table Foundations：

| ID | 題目 | 難度 |
|---|---|---|
| `F52-modulo-hash-function` | Normalized Modulo Hash Function | D1 |
| `F53-linear-probing-hash-table` | Linear Probing Hash Table | D2 |
| `F54-quadratic-probing-hash-table` | Quadratic Probing Hash Table | D2 |
| `F55-double-hashing-table` | Double Hashing Table | D3 |
| `F56-tombstone-hash-table` | Linear Probing With Tombstones | D3 |
| `F57-node-pool-chained-hash-table` | Node Pool Chained Hash Table | D3 |
| `F58-rehashing-linear-probing-table` | Rehashing Linear Probing Table | D3 |

CLI 範例：

```powershell
.\build\mingw-debug\algo.exe list
.\build\mingw-debug\algo.exe show F03-stack-array
.\build\mingw-debug\algo.exe test F03-stack-array tests\fixtures\submissions\stack_correct.cpp
.\build\mingw-debug\algo.exe stress F03-stack-array tests\fixtures\submissions\stack_correct.cpp --seed 20260613
```

`stress` 預設依題目 metadata 執行 100 cases，每個 case 100 次操作。失敗時會輸出
master seed、case seed、重播命令與完整 failing input；可用 `--cases` 調整 case 數。
