---
type: learning_roadmap
status: active
priority: p1
updated: 2026-06-22
context_policy: on_demand
owner: project
---

# 演算法學習路線

## 0. 定位與範圍

這是一份**個人學習路線**，與本專案的資料結構課綱並行但獨立，並與
`docs/math-roadmap.md`（數論與數學專線）互補。

- **範圍擴張（ADR-0006，proposed）**：本路線原本明確止於 ICPC regional，
  且不為演算法主題新增 Judge 題。開發者已**明確要求**把範圍擴張到國際賽
  （ICPC World Finals / IOI）等級，並把主題設計成**未來可落地為本地 Judge 題**。
  此擴張記錄於 `docs/adr/0006-...`，狀態為 `proposed`：在開發者把該 ADR 標記為
  `accepted` 前，`docs/project.md` 的 non-goals 仍然成立，**本 repo 暫不新增任何
  演算法 / 數學 Judge 題**，練習仍在外部平台（Codeforces、AtCoder、洛谷、
  Library Checker、Project Euler 等）進行。
- 每一階段標出 **🧪 Judge 候選**：說明該主題日後落地為本地 Judge 題時的差分
  oracle 與限制策略，便於 ADR-0006 被接受後逐題實作。非確定性輸出（如多解構造、
  幾何浮點）需在實作前另立後續 ADR。
- 資料結構是演算法的載體：本路線在每一階段標出與 `docs/learning-roadmap.md`
  的銜接點，避免重複學習。

目標分兩層：

1. **Regional 基礎層（G1–G8）**：補齊一般演算法能力，達到 ICPC 區域賽可穩定處理
   中後段題目的水準，並具備離散數學、多項式與 FFT 等高等數學基礎。
2. **International 進階層（G9–G15）**：補齊頂尖選手的知識體系，達到 **ICPC World
   Finals / IOI 獎牌**等級——進階圖論與樹分治、進階 DP 與凸優化、後綴自動機族、
   計算幾何、隨機化與啟發式、互動與構造題，以及把上述能力整合進限時訓練。

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
| L6 | 國際賽頂尖 | 能處理 WF / IOI 壓軸：重型模板（LCT、SAM、Blossom、Min_25）、多技巧縫合、構造與證明 |

## 2. 階段總覽

**Regional 基礎層**

| 階段 | 主題 | 建議題量 | 難度重心 | 主要前置 |
|---|---|---:|---|---|
| G1 | 基礎技巧 | 50 | L0–L2 | 複雜度、陣列 |
| G2 | Greedy | 30 | L1–L3 | 排序、證明 |
| G3 | Dynamic Programming | 70 | L1–L4 | G1、遞迴 |
| G4 | 圖論演算法 | 60 | L2–L4 | DS 圖表示、G3 |
| G5 | 字串演算法 | 35 | L2–L4 | G1、雜湊、Trie |
| G6 | 數學基礎 | 45 | L1–L3 | 取模運算（詳見 `docs/math-roadmap.md` M0–M4） |
| G7 | 高等數學專題 | 40 | L3–L5 | G6、複數、多項式（詳見 `math-roadmap.md` M5–M11） |
| G8 | 整合與限時訓練 | 持續 | L3–L5 | 全部 |

**International 進階層（本次新增，WF / IOI）**

| 階段 | 主題 | 建議題量 | 難度重心 | 主要前置 |
|---|---|---:|---|---|
| G9  | 進階圖論與樹分治 | 60 | L4–L6 | G4、Stage A6 |
| G10 | 進階 DP 與凸優化 | 55 | L4–L6 | G3、G7 |
| G11 | 進階字串（後綴自動機族） | 40 | L4–L6 | G5、Stage A7 |
| G12 | 計算幾何 | 45 | L3–L6 | G1、向量與極角 |
| G13 | 隨機化、近似與啟發式 | 30 | L3–L6 | G2、機率 |
| G14 | 構造、互動與 IOI 風格 | 30 | L3–L6 | 全部 |
| G15 | 國際賽整合與壓軸訓練 | 持續 | L5–L6 | G9–G14 |

以每週 8–10 小時估算，G1–G8 約需 14–20 個月；在其上完成 G9–G15 再約 12–18 個月。
基礎層與進階層可與資料結構路線、數學專線交錯進行；建議先讓 G1–G8 各達 80% 再進
進階層，但圖論 / DP / 字串三線可在基礎達標後即向上延伸。

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

---

# International 進階層（G9–G15，WF / IOI）

> 以下為本次新增的國際賽層。每階段附 **🧪 Judge 候選** 說明落地策略
> （ADR-0006 接受後逐題實作）。數學相關深化見 `docs/math-roadmap.md`。

## 13. G9 進階圖論與樹分治（約 60 題，L4–L6）

- 重鏈剖分（HLD）：路徑 / 子樹查詢與修改，配合 Segment Tree（Stage A2）。
- 樹分治：點分治、點分樹（動態點分治）、邊分治；虛樹（auxiliary tree）。
- 啟發式合併（DSU on tree / small-to-large）：子樹統計離線。
- 動態樹：Link-Cut Tree（LCT）、Euler Tour Tree、維護連通性與子樹聚合。
- 進階連通性：圓方樹（block-cut tree）、雙連通分量應用、支配樹（dominator tree）、
  3-邊 / 3-點連通。
- 進階網路流：Dinic 當前弧最佳化、最小割建模（最大權閉合子圖、最大密度子圖、
  專案選擇）、有上下界流、Gomory-Hu 樹、全局最小割（Stoer-Wagner）。
- 進階費用流：SSP + Johnson 勢能、消圈、zkw 費用流；流式建模。
- 一般圖匹配：帶花樹（Blossom）、帶權一般圖匹配；二分圖最佳化（Hopcroft-Karp、
  KM 的 `O(n^3)`）。
- 🧪 Judge 候選：路徑 / 連通性查詢輸出唯一 → 直接差分；最大流 / 最小割輸出**最佳值
  唯一**可差分，但流的具體分配多解，oracle 只比對最佳值並用獨立流演算法交叉驗證。
- 銜接 DS：HLD / LCT ↔ Stage A6 樹查詢、Stage A8 持久化；DSU on tree ↔ Stage A4。

驗收：能在 `n,m ~ 2*10^5` 下選擇 HLD / LCT / 點分治正確建模，並把建模題化為標準流 /
割 / 匹配模型且說明複雜度。

## 14. G10 進階 DP 與凸優化（約 55 題，L4–L6）

- 凸優化決策單調性：斜率最佳化（含 Li Chao Tree / 動態凸包）、四邊形不等式、
  D&C DP、Knuth 最佳化、SMAWK / 完全單調矩陣。
- Aliens / WQS 二分（Lagrangian 鬆弛）：把「恰選 k 個」轉成帶懲罰的凸性參數搜尋。
- 子集 DP：SOS DP（高維前綴和）、子集卷積、FWT（與 / 或 / 異或卷積，見 math M11）。
- 輪廓線 / 插頭 DP（broken profile / plug DP）：連通性狀態壓縮。
- 進階計數 DP：矩陣快速冪轉移、Berlekamp-Massey 猜遞迴、容斥型 DP。
- 樹形 DP 進階：換根、樹上背包（複雜度均攤證明）、長鏈剖分優化的深度 DP。
- 機率 / 期望 DP、博弈 DP 的高階建模（與 math M8 / M10 互補）。
- 🧪 Judge 候選：輸出最佳值 / 計數（取模）皆為唯一值 → 強差分；以暴力 `O(n^2)` DP
  作 oracle 對拍最佳化版本，pin 住會觸發決策單調性 / 凸性的構造前綴。
- 銜接 DS：凸優化 ↔ 單調結構、Li Chao ↔ Stage A2；SOS / FWT ↔ math M11。

驗收：見到 `O(n^2)` 轉移能判斷是否具決策單調性 / 凸性，選對最佳化把它降到
`O(n log n)` 或 `O(n)`，並能證明。

## 15. G11 進階字串（後綴自動機族，約 40 題，L4–L6）

- 後綴自動機（SAM）：endpos 等價類、parent 樹、子串計數 / 出現次數 / 字典序。
- 廣義 SAM（多串）、SAM 上 DP 與 parent 樹樹分治。
- 後綴陣列進階：SA-IS / DC3 線性構造、height 陣列的單調棧應用、本質不同子串。
- 後綴樹與 Ukkonen；SAM ↔ 後綴樹對偶。
- 回文自動機（PAM / eertree）：回文計數、雙端 PAM。
- AC 自動機 + DP / 矩陣快速冪（多模式串計數）。
- Z 函式 / KMP 自動機、Lyndon 分解（Duval）、最小表示法、Main–Lorentz 找所有重複。
- 字串雜湊的對抗性（anti-hash）與雙模防護。
- 🧪 Judge 候選：子串統計 / 計數 / 字典序第 k 等輸出唯一 → 差分；oracle 用後綴陣列
  暴力或 `O(n^2)` 直接統計交叉驗證 SAM 版本。
- 銜接 DS：Trie / AC 自動機 ↔ Stage A7；SAM parent 樹 ↔ G9 樹分治。

驗收：能用 SAM 或 SA 處理 `|s| ~ 10^6` 的本質不同子串、出現次數與多串公共子串問題。

## 16. G12 計算幾何（約 45 題，L3–L6）

- 基礎：向量運算、叉積定向、點 / 線 / 線段關係、極角排序、面積與重心。
- 凸包（Andrew / Graham）、旋轉卡尺（直徑 / 寬度 / 最近最遠對）、閔可夫斯基和。
- 半平面交（增量 / 排序 + 雙端佇列）、線性規劃的幾何解。
- 掃描線幾何：線段相交（Bentley–Ottmann）、矩形面積並、最近點對（分治）。
- 圓：交點、面積並、最小圓覆蓋（Welzl 隨機增量）、阿波羅尼圓。
- 三維幾何：3D 向量、3D 凸包、立體角；點在多面體內。
- 整點幾何：Pick 定理、格點計數（與 math 數論互補）。
- 數值穩定性：`long double` / 整數運算取捨、eps 策略、避免退化。
- 🧪 Judge 候選：**多為浮點輸出 → 需後續 ADR**（special judge / 容差比對）；整數幾何
  （凸包頂點、格點數、整點面積 ×2）可精確差分。建議先落地整數類，浮點類延後。
- 銜接 DS：掃描線 ↔ Stage A1–A2；極角有序集合 ↔ 平衡樹（Stage A5）。

驗收：能寫出穩定的凸包、半平面交與最小圓覆蓋，並判斷題目該用整數還是浮點建模。

## 17. G13 隨機化、近似與啟發式（約 30 題，L3–L6）

- 隨機化資料結構與演算法：Treap / 跳表（Stage A5）、隨機化快速選擇、
  Karger 最小割、Schwartz–Zippel 多項式判定零。
- 雜湊隨機化、隨機質數模、生日悖論與碰撞估計。
- Meet in the middle、折半枚舉、雙向 BFS。
- 模擬退火 / 爬山 / 遺傳：對 NP-hard（對應 `演算法/7_NP...pdf`）求近似解（IOI 部分分）。
- 近似演算法：2-近似頂點覆蓋、TSP 的 Christofides 直覺、LP 鬆弛與捨入。
- 🧪 Judge 候選：**評分制 / 啟發式輸出不適合精確差分**，需後續 ADR 設計
  scorer（如「輸出任一可行解並達到分數門檻」）；確定性隨機化（如 MITM 計數）可差分。
- 銜接 DS：Treap ↔ Stage A5；MITM 常配合雜湊表（Stage A 雜湊）。

驗收：能對 NP-hard 題以啟發式拿到穩定部分分，並用隨機化把期望複雜度說清楚。

## 18. G14 構造、互動與 IOI 風格（約 30 題，L3–L6）

- 構造題：不變量 / 對稱 / 歸納構造，輸出任一合法解；可行性與下界論證。
- 互動題：詢問策略、二分 / 三分互動、自適應對手模型、詢問次數下界。
- IOI 風格：子任務拆分、輸出限定 / 評分函式、批次（batch）與通訊（communication）題型。
- 位元 / bitset 黑魔法、`__builtin` 技巧、cache 友善與常數最佳化。
- 提交工程：對拍（brute-force checker）、隨機資料生成器、邊界（溢位、遞迴深度、
  精度）自查清單。
- 🧪 Judge 候選：構造題需 **special judge**（驗證解的合法性而非比對字串）；互動題需
  雙向協定 runner——兩者都要在 ADR-0006 之後各立實作 ADR，屬於 Judge 架構擴張。
- 銜接 DS：構造常用並查集 / 平衡樹維護不變量。

驗收：能對構造題給出合法性證明，能設計把詢問次數壓到下界的互動策略。

## 19. G15 國際賽整合與壓軸訓練（持續，L5–L6）

- 重型離線縫合：CDQ 分治、整體二分、莫隊（帶修 / 樹上 / 回滾）、分塊套樹。
- 多技巧疊加：資料結構 + DP 最佳化 + 數學在單題交會的壓軸題。
- 限時訓練：每週至少一場 5 小時 WF / IOI 難度模擬；維持團隊三人分工與讀題策略。
- 錯題系統：對 WA / TLE 建立 7 / 30 天間隔重寫（與本 repo 間隔複習機制一致）。
- 賽前清單：模板庫（含複雜度註記）、常見坑（溢位、特判 n=1、空輸入）、計時策略。
- 銜接 DS：整體二分 ↔ 持久化 / 可並結構（Stage A8）；掃描線 ↔ Stage A1–A3。

## 11. ICPC Regional 能力驗收

達成以下條件後，標記為 `Algorithm Regional Ready`：

- G1–G7 各階段至少完成建議題量的 80%，且每階段有無提示 AC 紀錄。
- 能在 60 分鐘內完成一題 L3 的圖論或 DP 建模題並通過。
- 連續三場 5 小時 regional 難度模擬，各至少解出 5 題、含一題 L4。
- 能對任一提交說明複雜度、邊界（溢位、遞迴深度、精度）與為何此演算法可行。
- 能用暴力對拍驗證非模板解。

達標代表具備 ICPC 區域賽中後段競爭力；國際賽（WF / IOI）能力驗收見下節。

## 11b. International（WF / IOI）能力驗收

達成以下條件後，標記為 `Algorithm International Ready`：

- G9–G14 各階段至少完成建議題量的 70%，且每階段有無提示 AC 紀錄。
- 模板庫齊備且自證：HLD、LCT、點分治、SAM / SA、Dinic + 費用流、帶花樹、半平面交、
  凸優化（斜率 / D&C / Aliens）、SOS / FWT，皆能在 30 分鐘內手寫並通過對拍。
- 能在 90 分鐘內完成一題 L5 壓軸（資料結構 + DP / 數學縫合）並通過。
- 連續三場 5 小時 WF / IOI 難度模擬，各至少解出一題 L5–L6，且總解題數穩定在中上段。
- 能對任一提交說明複雜度、邊界（溢位、遞迴深度、精度退化）與正確性證明 / 構造合法性。
- 對非確定性題（構造 / 互動 / 啟發式）能說明驗證方式（special judge / 對手模型 /
  scorer 門檻）。

達標代表具備 ICPC World Finals / IOI 獎牌競爭力。

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
