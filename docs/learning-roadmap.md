---
type: learning_roadmap
status: active
priority: p1
updated: 2026-06-13
context_policy: on_demand
owner: project
---

# 資料結構學習路線

## 1. 目前範圍

本階段只規劃資料結構，不建立一般演算法課綱。

目標：

1. 以 `資料結構/` 內的教材建立基礎。
2. 從新手逐步進入高等資料結構。
3. 使用本地 Judge 驗證正確性、時間、記憶體與操作複雜度。
4. 最終具備處理 ICPC 資料結構題的能力。

目前不規劃：

- Sorting 專題。
- Dynamic Programming。
- Greedy。
- Shortest Path、MST、Network Flow 等圖演算法。
- Number Theory、Combinatorics、Geometry。
- 一般字串演算法與完整 ICPC 綜合訓練。

ADT、Big-O、Tree Traversal、Graph Traversal 等內容會保留，因為它們是理解與驗證資料結構
不可缺少的基礎操作，但不延伸成獨立演算法課程。

## 2. 已選定的訓練模式：B1

### 題庫密度

- 教材基礎核心題：68 題。
- 高等資料結構核心題：64 題。
- 純資料結構核心路線：132 題。
- 進階資料結構挑戰題：約 20 題，依進度解鎖。

題目數量不是完成標準。每題都必須通過公開、隱藏、邊界、隨機與效能測試。

### STL 規則

「禁用 STL」採用精確限制：

- ADT 實作題不得使用會直接完成核心工作的容器或演算法。
- Stack 題不得使用 `std::stack`。
- Queue 題不得使用 `std::queue` 或 `std::deque`。
- Linked List 題不得使用 `std::list` 或 `std::forward_list`。
- Heap 題不得使用 `std::priority_queue` 或 heap algorithms。
- Hash Table 題不得使用 `std::unordered_map` 或 `std::unordered_set`。
- Tree、Graph 實作題不得使用現成圖或樹函式庫。
- `iostream`、`string`、`limits`、`cstdint`、`utility` 等基礎工具可以使用。
- 應用題可以使用已完成並解鎖的 STL 容器。
- 完成手寫版本後，才解鎖對應的競賽版 STL 工具。

Judge 的 API 掃描只能作為輔助，真正的驗收仍包含 code review、操作計數與大型測資。

## 3. 難度分級

| 等級 | 定位 | 能力要求 |
|---|---|---|
| D0 | 入門 | 能依照介面完成基本操作 |
| D1 | 基礎 | 能獨立實作 ADT 並處理邊界 |
| D2 | 應用 | 能辨識應使用的資料結構 |
| D3 | 進階 | 能組合資料結構完成查詢與更新 |
| D4 | 競賽中階 | 能處理離線、持久化或分解技巧 |
| D5 | 競賽高階 | 能實作動態樹或特殊資料結構 |

教材基礎以 D0 到 D2 為主，高等資料結構進入 D2 到 D5。

## 4. Stage F：教材基礎，68 題

教材以章節順序為主，但暫緩 CH7 Sorting，以及 CH6 中 MST、Shortest Path、
Topological Sort、Critical Path 等演算法內容。

| 單元 | 題數 | 核心內容 |
|---|---:|---|
| CH1 Basic Concepts | 3 | ADT、介面與表示分離、操作複雜度 |
| CH2 Arrays and Structures | 8 | Array ADT、Records、多項式表示、稀疏矩陣、多維陣列 |
| CH3 Stacks and Queues | 8 | Array Stack、Circular Queue、Linked Stack/Queue、操作序列 |
| CH4 Lists | 10 | 單向、雙向、環狀串列、反轉、串接、節點管理 |
| CH5-1 Trees | 14 | Tree representation、Traversal、Threaded Tree、Heap、BST |
| CH5-2 Forests | 6 | Forest representation、Union-Find、Weighting、Path Compression |
| CH6 Graph ADT | 5 | Adjacency Matrix/List、edge operations、DFS/BFS 結構走訪 |
| CH8 Hashing | 7 | Hash Function、Open Addressing、Double Hashing、Chaining |
| CH10 Search Trees | 7 | Optimal BST 結構、AVL、Red-Black Tree |

### Foundation 01：Linear Storage

第一個可執行的小章節依序建立連續儲存、容量管理與環狀索引的觀念：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F01-array-adt` | Fixed Array ADT | D0 | 索引、邊界與固定大小儲存 |
| `F02-dynamic-array` | Manual Dynamic Array | D1 | 容量倍增與 amortized `O(1)` push |
| `F03-stack-array` | Fixed Capacity Array Stack | D1 | LIFO 與 top invariant |
| `F04-circular-queue` | Fixed Capacity Circular Queue | D1 | FIFO 與環狀索引重用 |
| `F05-array-deque` | Fixed Capacity Array Deque | D2 | 兩端操作與雙向環狀索引 |

每題具備 public、boundary、invariant 與 hidden sequence 測資。完成順序固定，
因為後一題會重用前一題的容量與索引觀念。

### Foundation 02：Pointer-Based Linear Structures

第二章從陣列索引轉向節點生命週期與連結 invariant：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F06-singly-linked-list` | Singly Linked List | D1 | head/tail、索引走訪、刪除節點 |
| `F07-linked-stack` | Linked Stack | D1 | 單端節點配置與釋放 |
| `F08-linked-queue` | Linked Queue | D1 | front/back 同步與清空後重用 |
| `F09-doubly-linked-list` | Doubly Linked List | D2 | previous/next 雙向一致性 |
| `F10-circular-linked-list` | Circular Singly Linked List | D2 | tail-to-front cycle 與 `O(1)` rotate |

每題都有 seeded differential stress tests。F08 與 F10 的 generator 包含固定 invariant
前綴，確保 stale tail 與遺漏環狀旋轉的錯誤不依賴隨機運氣。

### Foundation 03：List Transformations And Node Management

第三章將基本 linked ADT 擴充到 ownership transfer 與儲存管理：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F11-singly-list-reverse` | Reverse A Singly Linked List | D2 | `O(1)` 額外空間反轉與 tail 更新 |
| `F12-singly-list-concatenate` | Concatenate Singly Linked Lists | D2 | `O(1)` ownership transfer |
| `F13-singly-list-split` | Split Singly Linked Lists | D2 | suffix detach 與兩組 head/tail |
| `F14-doubly-list-splice` | Splice Doubly Linked Lists | D3 | 範圍 relink 與 backlink consistency |
| `F15-fixed-node-pool-list` | Fixed Node Pool List | D3 | free list 與 erased-node reuse |

Generator 對 concat、splice 與 node pool 加入固定前綴，讓 source reuse、逆向刪除與
容量填滿後重用都成為每個 seed 必測的 invariant。

### Foundation 04：Complete List Representations

最後兩題補齊 CH4 的十題串列配置：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F16-sentinel-doubly-list` | Sentinel Doubly Linked List | D2 | dummy endpoints 與無分支 link/unlink |
| `F17-cursor-doubly-list` | Cursor Doubly Linked List | D3 | array cursor、雙向 index links、free list |

至此能直接比較 raw pointer、sentinel、circular、fixed pool 與 cursor 五種 linked
representation，並說明其邊界處理與記憶體管理取捨。

### Foundation 05：Arrays And Structures

這六題搭配 `F01` 與 `F02`，補齊 CH2 的八題配置：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F18-record-table` | Fixed Capacity Record Table | D1 | record array、unique key、slot reuse |
| `F19-sparse-polynomial` | Sparse Polynomial Terms | D2 | descending terms、zero removal |
| `F20-triplet-sparse-matrix` | Triplet Sparse Matrix | D2 | row-major triplets、ordered update |
| `F21-csr-sparse-matrix` | Compressed Sparse Row Matrix | D3 | row offsets、compressed columns |
| `F22-row-major-matrix` | Two-Dimensional Row-Major Array | D1 | contiguous storage、2D linearization |
| `F23-row-major-tensor` | Three-Dimensional Row-Major Array | D2 | multidimensional strides、inverse mapping |

固定測資與 seeded stress 都直接觀察 term order、CSR row offsets 與 row-major index，
避免只驗證 lookup 結果而漏掉錯誤的底層表示法。

### Foundation 06：Stack And Queue Representations

這三題搭配 `F03`、`F04`、`F05`、`F07`、`F08`，補齊 CH3 的八題配置：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F24-two-stacks-one-array` | Two Stacks In One Array | D2 | shared capacity、opposing growth |
| `F25-circular-linked-queue` | Circular Linked Queue | D2 | tail-only ring、`O(1)` rotate |
| `F26-linked-deque` | Linked Deque | D2 | endpoint symmetry、empty reset |

本章只處理 ADT 與表示法；maze、expression parsing 等應用演算法仍維持暫緩。

### Foundation 07：Core Tree Representations

CH5-1 的第一批先建立四種核心表示法：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F27-array-binary-tree` | Array Binary Tree | D2 | zero-based parent/child indexing、sparse slots |
| `F28-linked-binary-tree` | Linked Binary Tree | D2 | parent/left/right links、leaf lifecycle |
| `F29-inorder-threaded-tree` | Inorder Threaded Binary Tree | D3 | predecessor/successor threads、link tags |
| `F30-binary-min-heap` | Fixed Capacity Binary Min Heap | D2 | complete-tree array shape、bidirectional reheapify |

Generator 固定包含零基索引、parent backlink、非 ID 順序的 inorder threads，以及
heap key 增值後下沉，確保表示法錯誤不依賴隨機運氣。

### Foundation 08：Tree Traversal And Heap Views

CH5-1 的第二批讓 traversal 與 heap shape transition 可被直接觀察：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F31-binary-tree-traversal-views` | Binary Tree Traversal Views | D2 | preorder/inorder/postorder visit timing |
| `F32-level-order-tree-view` | Level-Order Binary Tree View | D2 | manual queue、depth、width、queue peak |
| `F33-bottom-up-min-heap-build` | Bottom-Up Binary Min Heap Build | D2 | Floyd `O(n)` construction、exact array shape |
| `F34-min-heap-removal-trace` | Binary Min Heap Removal Trace | D2 | replacement root、sift-down transitions |

固定 stress 前綴會區分 DFS 與 BFS、Floyd build 與 repeated push，以及移除後有無
sift-down。這些走訪只用於觀察樹 ADT，不擴張成一般演算法章節。

### Foundation 09：Binary Search Tree Foundations

CH5-1 的最後六題完成 BST 的查找、更新與結構驗證：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F35-bst-insert-search` | Binary Search Tree Insert And Search | D2 | unique keys、parent links、search path |
| `F36-bst-deletion-cases` | Binary Search Tree Deletion Cases | D2 | leaf、one-child、two-child deletion |
| `F37-bst-predecessor-successor` | BST Predecessor And Successor | D2 | subtree extrema、ancestor climb |
| `F38-bst-range-view` | Binary Search Tree Range View | D2 | inclusive range、lower/upper bound |
| `F39-bst-structural-validation` | Binary Search Tree Structural Validation | D3 | global key bounds、reachability、backlinks |
| `F40-bst-replacement-trace` | BST Replacement Node Trace | D3 | physical successor transplant、node identity |

固定 stress 前綴涵蓋 duplicate overwrite、predecessor 取代 successor、只看 immediate
parent 的鄰居查詢、排除 range endpoint、只驗證 local ordering，以及只複製 successor
key 的刪除。至此 CH5-1 規劃的 14 題全部完成。

### Foundation 10：Forest And Union-Find Foundations

CH5-2 的六題從一般森林表示法銜接到 disjoint-set forest：

| ID | 題目 | 難度 | 核心能力 |
|---|---|---|---|
| `F41-parent-array-forest` | Parent Array Forest | D2 | root chain、depth、children view |
| `F42-child-sibling-forest` | Child Sibling Forest | D2 | first-child/next-sibling representation |
| `F43-forest-traversal-views` | Forest Traversal Views | D2 | ordered roots、forest preorder/postorder/levelorder |
| `F44-quick-union-disjoint-set` | Quick Union Disjoint Set | D2 | root-to-root linking、raw parent array |
| `F45-weighted-union-disjoint-set` | Weighted Union Disjoint Set | D2 | union by component size |
| `F46-path-compression-trace` | Union Find Path Compression Trace | D3 | full path compression、parent transition |

固定 stress 前綴會命中只走一層 parent、忽略 sibling chain、只走第一棵樹、把非 root
直接相連、反向 weighting，以及 find 不壓縮路徑等錯誤。至此 CH5-2 規劃的六題完成。

### 暫緩的教材內容

- CH1 Selection Sort、Binary Search 演算法題。
- CH3 Maze 與完整 expression parsing 應用題。
- CH5 Counting Binary Trees 與 matrix multiplication 關聯。
- CH6 MST、Shortest Path、Topological Sort、Critical Path。
- CH7 全章 Sorting。

這些內容不刪除，只是不列入目前資料結構路線。

### Stage F 解鎖條件

- 能從空白檔案實作 Stack、Queue、Linked List、Heap、BST、Hash Table、DSU。
- 每個核心 ADT 通過至少 10,000 組隨機 differential tests。
- 能解釋每個操作的平均、最差時間與額外空間。
- 能說明 Array、Linked、Tree、Hash 等表示法的取捨。
- 90 分鐘綜合測驗中完成 3 題 D1/D2 題，且最多一次錯誤提交。

## 5. Stage A：高等資料結構，64 題

### A1. Fenwick Tree Family，8 題

- Point update / range query。
- Range update / point query。
- 雙 Fenwick Tree：range update / range query。
- Prefix lower bound。
- Frequency table 與 order statistics。
- Coordinate-indexed storage。
- 二維 Fenwick Tree。
- Sparse Fenwick Tree。

驗收：不看模板在 20 分鐘內完成標準 Fenwick Tree，並說明 `i & -i` 所代表的區間。

### A2. Segment Tree Family，12 題

- Recursive 與 iterative Segment Tree。
- Sum、minimum、maximum、GCD monoid。
- Point update 與 range query。
- Lazy propagation：range add。
- Lazy propagation：range assign。
- 混合 lazy tag 的組合順序。
- 找第一個或最後一個滿足條件的位置。
- Dynamic Segment Tree。
- Persistent Segment Tree。
- K-th order statistic。
- Segment Tree Beats，列為 D5 解鎖題。

驗收：能寫出不互相污染的 lazy tags，並使用暴力結構進行隨機對拍。

### A3. Static Range Structures，8 題

- Sparse Table。
- Disjoint Sparse Table。
- Square Root Decomposition。
- Block decomposition with lazy tags。
- Merge Sort Tree。
- Wavelet Tree。
- Wavelet Matrix。
- Bitset-based set representation。

驗收：能根據更新頻率、查詢頻率與 operation 性質選擇結構。

### A4. Union-Find Variants，7 題

- Size/rank Union-Find。
- Path Compression 與 amortized complexity。
- Potential/Weighted DSU。
- Parity DSU。
- Rollback DSU。
- Persistent DSU。
- Successor DSU。

驗收：能說明 path compression、rollback 與 persistence 之間的相容限制。

### A5. Balanced、Randomized 與 Meldable Trees，9 題

- AVL 完整插入與刪除。
- Red-Black Tree 完整插入與刪除。
- Treap：split、merge、insert、erase。
- Implicit Treap。
- Splay Tree。
- Leftist Heap。
- Skew Heap。
- Binomial Heap。
- Order Statistic Tree。

PBDS 只在理解手寫版本後作為工具解鎖。

驗收：能在 30 分鐘內完成 Treap，並通過重複 key、空樹、退化與亂數測試。

### A6. Tree Query Structures，10 題

- Euler Tour representation。
- Binary Lifting table。
- LCA query structure。
- Heavy-Light Decomposition。
- HLD 搭配 Segment Tree。
- Small-to-Large container merging。
- DSU on Tree。
- Centroid Decomposition。
- Virtual Tree representation。
- Link-Cut Tree，列為 D5 挑戰。

驗收：能辨識 subtree、path、static tree 與 dynamic tree query 的差異。

### A7. String Index Structures，6 題

- Trie。
- Compressed Trie / Radix Tree。
- Binary/XOR Trie。
- Aho-Corasick Automaton。
- Suffix Array 與 LCP structure。
- Suffix Automaton 或 Palindromic Tree。

本階段只處理結構建置、查詢與更新，不擴展成完整字串演算法課程。

驗收：能處理總字串長度至少 `2 * 10^5` 的索引與多模式查詢。

### A8. Persistent 與 Specialized Structures，4 題

- Persistent Stack、Queue 或 Array。
- Persistent Trie。
- Li Chao Tree。
- Interval Tree 或 Dynamic Ordered Set。

驗收：能說明結構共享、版本根節點與每次更新的額外空間成本。

## 6. 資料結構競賽能力驗收

完成以下條件後，標記為 `Data Structure Ready`：

- 完成 132 題核心題，且每題至少一次無提示 AC。
- 所有核心結構都通過 random differential testing。
- 能在 15 分鐘內完成 Stack、Queue、DSU、Fenwick Tree。
- 能在 25 分鐘內完成 Heap、BST、Hash Table、Segment Tree。
- 能在 35 分鐘內完成 Treap、LCA 或 HLD 骨架。
- 能針對 `n <= 2 * 10^5` 判斷時間與記憶體是否可行。
- 能說明 iterator/pointer invalidation、整數溢位、遞迴深度、hash collision。
- 能使用 brute structure 為高等資料結構產生對拍答案。
- 連續三次 120 分鐘資料結構測驗，各完成至少 3 題 D2 至 D4 題目。

這代表具備 ICPC 所需的資料結構能力，但不代表已具備完整 ICPC 能力。其他演算法領域留待
未來另行規劃。

## 7. 建議時間

以每週 8 至 10 小時估算：

| 階段 | 預估時間 |
|---|---|
| Stage F 教材基礎 | 5 至 7 個月 |
| Stage A 高等資料結構 | 6 至 8 個月 |
| 複習與限時整合 | 1 至 2 個月 |

完整資料結構路線約 12 至 17 個月。每週投入更多時間可以縮短，但不應省略隨機對拍、
間隔重寫與複雜度分析。

## 8. CLI 到 Web 的開發順序

### Phase B1：CLI Judge

先完成：

- `algo list`
- `algo show <problem-id>`
- `algo test <problem-id>`
- `algo stress <problem-id>`
- `algo benchmark <problem-id>`
- `algo progress`

CLI 必須支援答案錯誤、逾時、記憶體超限、執行錯誤、禁止 API 與複雜度警告。

### Phase C1：Web Dashboard

當 CLI 已穩定支援至少 25 題後開始：

- 題目列表與資料結構分類。
- 題目內容與限制。
- 執行結果與失敗案例摘要。
- 章節進度、錯題和複習排程。
- Benchmark 成長圖。

### Phase C2：Web Practice

當 CLI 已穩定支援全部 68 題教材核心後開始：

- 本地程式碼編輯與提交。
- 限時章節測驗。
- 操作次數與記憶體使用視覺化。
- 結構版本、節點和 query trace 顯示。

CLI Judge 保持唯一判定核心，Web 只透過 API 呼叫它，避免出現兩套判題規則。
