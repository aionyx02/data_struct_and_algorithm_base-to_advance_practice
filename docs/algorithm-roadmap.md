---
type: learning_roadmap
status: active
priority: p1
updated: 2026-06-14
context_policy: on_demand
owner: project
---

# 演算法學習路線

## 0. 定位與範圍

這是一份**個人學習路線**，與本專案的資料結構課綱並行但**獨立**。

- 它**不改變**本專案 DS-only 的實作範圍：`docs/project.md` 的 non-goals 仍然成立，
  本地 Judge 與題庫目前只收資料結構題。
- 演算法練習在外部平台（Codeforces、AtCoder、洛谷、Library Checker 等）進行；
  本 repo 不為這些主題新增 Judge 題。
- 未來若要把任一演算法主題正式納入本地 Judge，需另立 ADR（見 `docs/CLAUDE.md`
  第 7 節 ADR 觸發清單），再回填到 `docs/learning-roadmap.md`。
- 資料結構是演算法的載體：本路線會在每一階段標出與 `docs/learning-roadmap.md`
  的銜接點，避免重複學習。

目標：在既有資料結構能力之上，補齊**一般演算法**能力，最終達到 **ICPC 區域賽**
（regional）可穩定處理中後段題目的水準，並具備離散數學、多項式與 FFT 等高等數學基礎。

## 1. 難度分級

沿用 `docs/learning-roadmap.md` 的 D0–D5 精神，但對應演算法主題：

| 等級 | 定位 | 能力要求 |
|---|---|---|
| L0 | 入門 | 能套用標準模板解裸題 |
| L1 | 基礎 | 能獨立推導並實作核心演算法 |
| L2 | 應用 | 能辨識題目該用哪個演算法並組合 |
| L3 | 進階 | 能做轉化、證明正確性與複雜度 |
| L4 | 競賽中階 | 能處理離線、最佳化與多技巧疊加 |
| L5 | 競賽高階 | 能推導非模板解、處理 regional 後段題 |

## 2. 階段總覽

| 階段 | 主題 | 建議題量 | 難度重心 | 主要前置 |
|---|---|---:|---|---|
| G1 | 基礎技巧 | 50 | L0–L2 | 複雜度、陣列 |
| G2 | Greedy | 30 | L1–L3 | 排序、證明 |
| G3 | Dynamic Programming | 70 | L1–L4 | G1、遞迴 |
| G4 | 圖論演算法 | 60 | L2–L4 | DS 圖表示、G3 |
| G5 | 字串演算法 | 35 | L2–L4 | G1、雜湊、Trie |
| G6 | 數學基礎 | 45 | L1–L3 | 取模運算 |
| G7 | 高等數學專題 | 40 | L3–L5 | G6、複數、多項式 |
| G8 | 整合與限時訓練 | 持續 | L3–L5 | 全部 |

以每週 8–10 小時估算，G1–G8 約需 14–20 個月，可與資料結構路線交錯進行。

## 3. G1 基礎技巧（約 50 題，L0–L2）

- 複雜度分析、常數最佳化、I/O 加速。
- 排序：comparator、穩定性、自訂鍵；計數/基數排序的適用情境。
- Binary search 與**二分答案**：可行性判定函式的設計。
- Two pointers / sliding window：單調性與視窗維護。
- Prefix sum / 差分：一維與二維、區間批次更新。
- Bit 技巧：lowbit、子集列舉、bitset 加速。
- 銜接 DS：差分與 prefix sum ↔ Fenwick（`A01`–`A03`）、二分答案 ↔ 各種可行性結構。

驗收：能在看到「最大化最小值 / 最小化最大值」時立即想到二分答案，並寫出 `O(n log n)`
判定。

## 4. G2 Greedy（約 30 題，L1–L3）

- 交換論證（exchange argument）與反證法。
- 排序型貪心、區間排程與區間點覆蓋。
- Huffman 與優先佇列型貪心。
- 反悔貪心（regret / slope trick 入門）。
- 銜接 DS：優先佇列 ↔ Heap（`F30`、`F33`、`F34`）。

驗收：能對一個貪心策略給出交換論證或反例，而不是靠直覺提交。

## 5. G3 Dynamic Programming（約 70 題，L1–L4）

- 線性 DP、LIS（含 `O(n log n)`）、LCS、編輯距離。
- 背包族：0/1、完全、多重、分組、價值/方案數變體。
- 區間 DP、樹形 DP、換根 DP。
- 狀態壓縮 DP、輪廓線 DP。
- 數位 DP、機率與期望 DP、博弈 DP。
- DP 最佳化：單調隊列、單調堆疊、斜率最佳化（凸包技巧）、
  四邊形不等式、divide and conquer DP、Knuth 最佳化。
- 銜接 DS：DP 最佳化常用 Segment Tree / Fenwick / 單調結構（對應 Stage A2、A1）。

驗收：能定義狀態與轉移、證明無後效性，並判斷是否需要、用哪種最佳化把
`O(n^2)` 降到 `O(n log n)`。

## 6. G4 圖論演算法（約 60 題，L2–L4）

> 對應 `docs/learning-roadmap.md` 中目前「暫緩」的 MST / Shortest Path /
> Topological Sort / Critical Path 等內容，在此作為**演算法**主題學習。

- 圖走訪：DFS / BFS 樹、時間戳、染色。
- 最短路：Dijkstra（堆最佳化）、Bellman-Ford、SPFA、Floyd-Warshall、
  0/1 BFS、差分約束。
- 最小生成樹：Kruskal、Prim、Borůvka。
- 連通性：拓樸排序、SCC（Tarjan / Kosaraju）、2-SAT、
  橋與割點、雙連通分量、Euler 路徑與迴路。
- 網路流：最大流（Dinic / ISAP）、最小割、費用流（MCMF）、上下界流。
- 二分圖：匈牙利演算法、Hopcroft-Karp、König 定理、最小點覆蓋與最大獨立集。
- 樹上問題：LCA、樹上差分、直徑與重心（與 Stage A6 樹查詢結構互補）。
- 銜接 DS：Kruskal ↔ DSU（`F44`–`F46`、Stage A4）、樹查詢 ↔ Stage A6。

驗收：能依資料規模在 Dijkstra / Bellman-Ford / Floyd 之間選擇，並把建模題化為
標準最短路或流模型。

## 7. G5 字串演算法（約 35 題，L2–L4）

- 字串雜湊（單/雙模、滾動雜湊）與碰撞風險評估。
- KMP / 失配函式、Z 函式。
- Manacher 回文。
- Trie 與 AC 自動機（多模式匹配）。
- 後綴結構：Suffix Array + LCP、Suffix Automaton；回文樹（PAM）。
- 銜接 DS：Trie / AC 自動機 ↔ Stage A7 String Index Structures。

驗收：能處理總長度至少 `2 * 10^5` 的多模式匹配與最長回文/重複子串問題。

## 8. G6 數學基礎（約 45 題，L1–L3）

- 數論：GCD/擴展歐幾里得、線性篩、快速冪、模逆元、CRT、
  歐拉定理、費馬小定理、Möbius 與積性函數入門。
- 組合數學：排列組合、Pascal、Lucas、容斥原理、卡特蘭數。
- 線性代數：高斯消去、矩陣快速冪、線性遞迴。
- 機率與期望的基本計算與線性性。
- 銜接 DS：矩陣快速冪 ↔ 線性遞迴轉移；計數常與 Fenwick（逆序對）結合。

驗收：能在取模環境下正確處理除法（逆元）、組合數與線性遞迴加速。

## 9. G7 高等數學專題（約 40 題，L3–L5）

- 離散數學：集合與關係、生成函數（普通/指數）、
  Burnside 引理與 Pólya 計數。
- 多項式與**快速傅立葉變換**：FFT、NTT、任意模數 NTT（MTT）。
- 多項式進階：多項式求逆、開根、ln/exp、除法取模、多點求值與插值。
- 線性規劃概念與對偶直覺、單純形法入門。
- 博弈論：Nim、Sprague-Grundy、對稱策略。
- 計算幾何：向量運算、凸包、半平面交、旋轉卡尺、最近點對、極角排序。
- 銜接 DS：FFT 卷積 ↔ 計數 DP 加速；計算幾何常配合掃描線與有序集合。

驗收：能用 NTT 在 `O(n log n)` 完成多項式乘法並應用於計數，能寫出穩定的凸包與
半平面交。

## 10. G8 整合與限時訓練（持續，L3–L5）

- 離線技巧串接：分塊、莫隊（含帶修/樹上莫隊）、CDQ 分治、整體二分。
- 掃描線：矩形面積並、區間統計，配合 Segment Tree / Fenwick。
- 限時訓練：每週至少一場 regional 難度模擬（5 小時、團隊或個人）。
- 錯題重練：對 WA/TLE 題建立 7 天與 30 天間隔重寫。
- 銜接 DS：CDQ / 掃描線 ↔ Stage A1–A3 區間結構；整體二分 ↔ 持久化/可並結構。

## 11. ICPC Regional 能力驗收

達成以下條件後，標記為 `Algorithm Regional Ready`：

- G1–G7 各階段至少完成建議題量的 80%，且每階段有無提示 AC 紀錄。
- 能在 60 分鐘內完成一題 L3 的圖論或 DP 建模題並通過。
- 連續三場 5 小時 regional 難度模擬，各至少解出 5 題、含一題 L4。
- 能對任一提交說明複雜度、邊界（溢位、遞迴深度、精度）與為何此演算法可行。
- 能用暴力對拍驗證非模板解。

達標代表具備 ICPC 區域賽中後段競爭力；world final 等級需另行規劃。

## 12. 與資料結構路線的關係

| 演算法主題 | 對應資料結構（`docs/learning-roadmap.md`） |
|---|---|
| 二分答案 / 差分 / 逆序對 | Fenwick Tree（`A01`–`A03`、Stage A1） |
| DP 最佳化 / 掃描線 | Segment Tree（Stage A2） |
| Kruskal / 連通性 | Union-Find（`F44`–`F46`、Stage A4） |
| LCA / 樹上路徑 | Tree Query Structures（Stage A6） |
| 多模式匹配 | String Index Structures（Stage A7） |
| 離線查詢 / 版本 | Persistent 與 Specialized（Stage A8） |

建議節奏：先用資料結構路線打穩結構基礎，再以本路線補演算法；兩者在 DP 最佳化、
圖論與字串三處交會時集中練習，效益最高。
