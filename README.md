# Data Structures Practice

這是一個以 C++20、CLion 與 CMake 建立的漸進式資料結構學習專案。

目前範圍只包含資料結構。題目主要依據 `資料結構/` 內的課程教材，再延伸到高等資料結構。
專案內建本地 CLI Judge，可驗證固定測資、執行可重播的隨機差分測試，並檢查時間限制、
輸出限制與禁止使用的 API。固定測資結果會原子寫入本機進度檔。

## 環境需求

- Windows 10/11
- Node.js 20 或更新版本
- CMake 3.25 或更新版本
- MinGW `g++`，支援 C++20
- CLion 可選；所有操作也能直接在 PowerShell 執行

確認 `node`、`npm`、`cmake`、`g++` 都能從 PowerShell 的 `PATH` 找到。

## 快速開始

第一次使用：

```powershell
npm install
npm run problems
```

選擇題目並建立解答：

```powershell
npm run problem -- F03-stack-array
npm run new:solution -- F03-stack-array
```

這會建立：

```text
solutions/F03-stack-array.cpp
```

完成程式後執行固定測資與隨機測試：

```powershell
npm run judge -- F03-stack-array
npm run stress -- F03-stack-array
npm run progress -- F03-stack-array
npm run review
```

`judge` 與 `stress` 第一次執行時，若找不到 Judge 執行檔，會自動執行 CMake 建置。

## 解答放置與執行位置

使用者的解答原始碼預設放在專案根目錄下：

```text
solutions/<problem-id>.cpp
```

例如 `npm run new:solution -- A09-segment-tree-recursive-sum` 會建立
`solutions/A09-segment-tree-recursive-sum.cpp`。之後執行 `judge` 或 `stress`
時若未指定來源檔，就會使用這個預設路徑。

也可以將解答放在專案內的其他資料夾，並在指令中指定 `.cpp` 路徑：

```powershell
npm run judge -- A09-segment-tree-recursive-sum .\my-solutions\a09.cpp
npm run stress -- A09-segment-tree-recursive-sum .\my-solutions\a09.cpp
```

來源檔必須位於本專案根目錄內。Judge 會使用 C++20 將來源檔編譯成
`.judge/runtime/.../submission.exe`，在該暫存目錄執行測資，完成後自動清除；
使用者不需要自行管理編譯產物。

目前 `solutions/` 沒有列在 `.gitignore`，因此個人解答會出現在
`git status`，可依需求納入版本控制。

## 日常練習流程

1. 用 `npm run problems` 查看目前所有題目。
2. 用 `npm run problem -- <problem-id>` 查看難度、限制與題目文件位置。
3. 用 `npm run new:solution -- <problem-id>` 建立初始程式。
4. 閱讀題目的 `statement.md`，在 `solutions/` 實作資料結構。
5. 用 `npm run judge -- <problem-id>` 通過固定測資。
6. 用 `npm run stress -- <problem-id>` 通過 seeded differential tests。
7. 記錄時間、空間複雜度，以及失敗時破壞的 invariant。

## 簡化指令

| 指令 | 用途 |
|---|---|
| `npm run build` | 設定並建置本地 Judge |
| `npm run problems` | 列出所有可練習題目 |
| `npm run problem -- F03-stack-array` | 顯示指定題目的 metadata 與 statement 路徑 |
| `npm run new:solution -- F03-stack-array` | 建立 `solutions/F03-stack-array.cpp` |
| `npm run judge -- F03-stack-array` | 對預設 solution 執行固定測資 |
| `npm run stress -- F03-stack-array` | 對預設 solution 執行隨機差分測試 |
| `npm run progress -- F03-stack-array` | 顯示指定題目的本機嘗試、AC 與最佳時間 |
| `npm run review` | 顯示今天以前到期的複習題目 |
| `npm run check` | 建置並執行全部 C++ integration tests |
| `npm run verify` | 執行 C++、文件、lint 與 Node.js 完整驗證 |

也可以指定自己的來源檔，不必放在 `solutions/`：

```powershell
npm run judge -- F03-stack-array .\my-solutions\stack.cpp
npm run stress -- F03-stack-array .\my-solutions\stack.cpp --seed 20260614 --cases 300
```

只執行符合名稱的 integration tests：

```powershell
npm run check -- "f5[2-8]"
```

所有簡化指令都由同一入口提供，可查看完整說明：

```powershell
npm run practice -- help
```

## 判題結果

| Verdict | 意義 |
|---|---|
| `AC` | 所有測試通過 |
| `WA` | 輸出錯誤或資料結構 invariant 被破壞 |
| `TLE` | 超過題目時間限制 |
| `RE` | 執行期間發生錯誤 |
| `CE` | C++ 編譯失敗 |
| `API` | 使用題目禁止的容器或 API |

`stress` 預設依題目 metadata 執行 100 cases，每個 case 100 次操作。失敗時會顯示
master seed、case seed、重播指令與完整 failing input，因此可以重現同一個錯誤。

`judge` 會將每次 verdict 記錄在忽略版控的 `.judge/progress.json`。寫入採同目錄
暫存檔與原子替換，避免中斷時截斷唯一紀錄。自動化可對 `algo test` 加
`--no-progress`；`stress` 不會改寫學習進度。未通過會安排隔天複習，第一次 AC
安排 7 天後，後續 AC 安排 30 天後；`npm run review` 可列出已到期題目。

## 專案結構

| 路徑 | 內容 |
|---|---|
| `problems/` | 題目 metadata、敘述與固定測資 |
| `solutions/` | 使用 `new:solution` 建立的個人解答 |
| `src/`、`include/` | 本地 Judge 核心與 CLI |
| `tests/fixtures/submissions/` | Judge 自我驗證用的正確與刻意錯誤程式 |
| `資料結構/` | 資料結構教材 |
| `演算法/` | 已加入專案但目前暫不納入課程規劃的演算法教材 |
| `docs/` | 學習路線、Judge 規格、架構與 context engineering |

## 題庫範圍

- 68 題教材基礎。
- 64 題高等資料結構。
- ADT 實作題會限制可直接完成核心工作的 STL 容器。
- 目前只規劃資料結構；一般演算法、MST、最短路徑與排序課程仍暫緩。

詳細規劃請見：

- `docs/learning-roadmap.md`
- `docs/judge-requirements.md`
- `docs/architecture.md`

### 第一組 Linear Storage

| ID | 題目 | 難度 |
|---|---|---|
| `F01-array-adt` | Fixed Array ADT | D0 |
| `F02-dynamic-array` | Manual Dynamic Array | D1 |
| `F03-stack-array` | Fixed Capacity Array Stack | D1 |
| `F04-circular-queue` | Fixed Capacity Circular Queue | D1 |
| `F05-array-deque` | Fixed Capacity Array Deque | D2 |

### 第二組 Pointer-Based Linear Structures

| ID | 題目 | 難度 |
|---|---|---|
| `F06-singly-linked-list` | Singly Linked List | D1 |
| `F07-linked-stack` | Linked Stack | D1 |
| `F08-linked-queue` | Linked Queue | D1 |
| `F09-doubly-linked-list` | Doubly Linked List | D2 |
| `F10-circular-linked-list` | Circular Singly Linked List | D2 |

### 第三組 List Transformations And Node Management

| ID | 題目 | 難度 |
|---|---|---|
| `F11-singly-list-reverse` | Reverse A Singly Linked List | D2 |
| `F12-singly-list-concatenate` | Concatenate Singly Linked Lists | D2 |
| `F13-singly-list-split` | Split Singly Linked Lists | D2 |
| `F14-doubly-list-splice` | Splice Doubly Linked Lists | D3 |
| `F15-fixed-node-pool-list` | Fixed Node Pool List | D3 |

### 第四組 Complete List Representations

| ID | 題目 | 難度 |
|---|---|---|
| `F16-sentinel-doubly-list` | Sentinel Doubly Linked List | D2 |
| `F17-cursor-doubly-list` | Cursor Doubly Linked List | D3 |

### 第五組 Arrays And Structures

| ID | 題目 | 難度 |
|---|---|---|
| `F18-record-table` | Fixed Capacity Record Table | D1 |
| `F19-sparse-polynomial` | Sparse Polynomial Terms | D2 |
| `F20-triplet-sparse-matrix` | Triplet Sparse Matrix | D2 |
| `F21-csr-sparse-matrix` | Compressed Sparse Row Matrix | D3 |
| `F22-row-major-matrix` | Two-Dimensional Row-Major Array | D1 |
| `F23-row-major-tensor` | Three-Dimensional Row-Major Array | D2 |

### 第六組 Stack And Queue Representations

| ID | 題目 | 難度 |
|---|---|---|
| `F24-two-stacks-one-array` | Two Stacks In One Array | D2 |
| `F25-circular-linked-queue` | Circular Linked Queue | D2 |
| `F26-linked-deque` | Linked Deque | D2 |

### 第七組 Core Tree Representations

| ID | 題目 | 難度 |
|---|---|---|
| `F27-array-binary-tree` | Array Binary Tree | D2 |
| `F28-linked-binary-tree` | Linked Binary Tree | D2 |
| `F29-inorder-threaded-tree` | Inorder Threaded Binary Tree | D3 |
| `F30-binary-min-heap` | Fixed Capacity Binary Min Heap | D2 |

### 第八組 Tree Traversal And Heap Views

| ID | 題目 | 難度 |
|---|---|---|
| `F31-binary-tree-traversal-views` | Binary Tree Traversal Views | D2 |
| `F32-level-order-tree-view` | Level-Order Binary Tree View | D2 |
| `F33-bottom-up-min-heap-build` | Bottom-Up Binary Min Heap Build | D2 |
| `F34-min-heap-removal-trace` | Binary Min Heap Removal Trace | D2 |

### 第九組 Binary Search Tree Foundations

| ID | 題目 | 難度 |
|---|---|---|
| `F35-bst-insert-search` | Binary Search Tree Insert And Search | D2 |
| `F36-bst-deletion-cases` | Binary Search Tree Deletion Cases | D2 |
| `F37-bst-predecessor-successor` | BST Predecessor And Successor | D2 |
| `F38-bst-range-view` | Binary Search Tree Range View | D2 |
| `F39-bst-structural-validation` | Binary Search Tree Structural Validation | D3 |
| `F40-bst-replacement-trace` | BST Replacement Node Trace | D3 |

### 第十組 Forest And Union-Find Foundations

| ID | 題目 | 難度 |
|---|---|---|
| `F41-parent-array-forest` | Parent Array Forest | D2 |
| `F42-child-sibling-forest` | Child Sibling Forest | D2 |
| `F43-forest-traversal-views` | Forest Traversal Views | D2 |
| `F44-quick-union-disjoint-set` | Quick Union Disjoint Set | D2 |
| `F45-weighted-union-disjoint-set` | Weighted Union Disjoint Set | D2 |
| `F46-path-compression-trace` | Union Find Path Compression Trace | D3 |

### 第十一組 Graph ADT Representations

| ID | 題目 | 難度 |
|---|---|---|
| `F47-undirected-adjacency-matrix` | Undirected Adjacency Matrix | D2 |
| `F48-directed-adjacency-matrix` | Directed Adjacency Matrix | D2 |
| `F49-undirected-adjacency-list` | Undirected Sorted Adjacency List | D2 |
| `F50-directed-adjacency-list` | Directed Sorted Adjacency List | D2 |
| `F51-fixed-edge-table-graph` | Fixed Capacity Edge Table Graph | D3 |

### 第十二組 Hash Table Foundations

| ID | 題目 | 難度 |
|---|---|---|
| `F52-modulo-hash-function` | Normalized Modulo Hash Function | D1 |
| `F53-linear-probing-hash-table` | Linear Probing Hash Table | D2 |
| `F54-quadratic-probing-hash-table` | Quadratic Probing Hash Table | D2 |
| `F55-double-hashing-table` | Double Hashing Table | D3 |
| `F56-tombstone-hash-table` | Linear Probing With Tombstones | D3 |
| `F57-node-pool-chained-hash-table` | Node Pool Chained Hash Table | D3 |
| `F58-rehashing-linear-probing-table` | Rehashing Linear Probing Table | D3 |

### 第十三組 Advanced Search Tree Foundations

| ID | 題目 | 難度 |
|---|---|---|
| `F59-avl-single-rotation-trace` | AVL Single Rotation Trace | D2 |
| `F60-avl-double-rotation-trace` | AVL Double Rotation Trace | D2 |
| `F61-avl-insertion-tree` | AVL Insertion Tree | D3 |
| `F62-avl-deletion-tree` | AVL Deletion Tree | D3 |
| `F63-avl-structural-validation` | AVL Structural Validation | D3 |
| `F64-order-four-search-tree-validation` | Order-Four Search Tree Validation | D3 |
| `F65-red-black-insertion-tree` | Red-Black Insertion Tree | D3 |

### 第十四組 Basic Concepts

| ID | 題目 | 難度 |
|---|---|---|
| `F66-sequence-shift-cost-trace` | Sequence Shift Cost Trace | D1 |
| `F67-switchable-stack-representation` | Switchable Stack Representation | D2 |
| `F68-amortized-growth-ledger` | Amortized Growth Ledger | D2 |

至此 Stage F 的 68 題教材基礎皆可透過本地 Judge 練習與壓力測試。

## 維護者工作流程

修改 Judge、題庫或文件後執行：

```powershell
npm run verify
```

AI 或開發者開始工作前先讀取 `CLAUDE.md`，再依 `docs/index.md` 路由到最小必要文件。
