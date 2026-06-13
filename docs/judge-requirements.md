---
type: judge_requirements
status: active
priority: p1
updated: 2026-06-13
context_policy: retrieve_when_planning
owner: project
---

# 本地 Judge 需求

## 1. 判題模式

Judge 必須支援三種資料結構題型：

1. `function`：由 harness 呼叫指定函式。
2. `adt`：由 harness 建立物件並執行操作序列。
3. `stdio`：使用標準輸入輸出完成結構查詢與更新。

教材基礎以前兩種為主，高等資料結構會逐步增加 `stdio` 題型。

## 2. 每題 metadata

```json
{
  "id": "F03-stack-array",
  "title": "Array Stack",
  "stage": "F",
  "difficulty": "D1",
  "judge_mode": "adt",
  "time_limit_ms": 1000,
  "memory_limit_mb": 64,
  "expected_time": "O(1) amortized per operation",
  "expected_space": "O(n)",
  "allowed_headers": ["iostream", "limits", "cstdint"],
  "forbidden_symbols": ["std::stack", "std::vector"],
  "random_tests": 10000,
  "operation_limit": 200000
}
```

時間限制需要依本機 benchmark 校準，不能直接套用線上 Judge 的毫秒數。

## 3. 驗證層次

每次 `algo test` 依序執行：

1. Metadata 與必要檔案驗證。
2. 編譯與 forbidden API 掃描。
3. 公開範例。
4. 邊界測試。
5. 隱藏 deterministic tests。
6. Random differential tests。
7. 大型效能測試。
8. Sanitizer 測試，於 debug profile 執行。

## 4. 資料結構正確性驗證

不同結構需要額外檢查 invariant：

- Stack/Queue：容量、索引、空與滿狀態。
- Linked List：節點連結、head/tail、cycle 與釋放狀態。
- Heap：shape property 與 heap-order property。
- BST：inorder ordering 與重複 key 規則。
- AVL：BST ordering、height 與 balance factor。
- Red-Black Tree：root color、red adjacency、black height。
- Hash Table：probe sequence、deleted marker、load factor。
- DSU：parent forest、size/rank 與 potential relation。
- Segment Tree：節點聚合值與 lazy tag 一致性。
- Persistent Structure：舊版本不可被新更新修改。

## 5. 複雜度驗證

Judge 不宣稱能從任意 C++ 程式嚴格證明 Big-O，而是提供多種證據：

- 使用 `n`、`2n`、`4n` 測量成長率。
- 計算 compare、swap、node visit、rotation、probe 和 allocation 次數。
- 將答案與小資料暴力結構對拍。
- 使用刻意構造的退化案例。
- 掃描禁止的容器和演算法。
- 對 hash 題加入 collision-heavy inputs。
- 對平衡樹檢查實際高度上界。

結果分為：

- `AC`：正確且通過限制。
- `WA`：答案或結構狀態錯誤。
- `TLE`：超過時間限制。
- `MLE`：超過記憶體限制。
- `RE`：執行錯誤。
- `CE`：編譯錯誤。
- `API`：使用禁止 API。
- `INV`：資料結構 invariant 被破壞。
- `CX`：複雜度成長可疑，需要人工檢查。

## 6. 限時測驗模式

`algo assessment start <assessment-id>` 應提供：

- 90 或 120 分鐘計時。
- 題目依任意順序提交。
- 保留每次提交原始碼與 verdict。
- 顯示各題通過的測試層次。
- 結束後產生需重寫與複習的題目清單。

目前不規劃完整 ICPC contest、penalty 或 frozen scoreboard 功能。

## 7. Progress 資料

每題至少記錄：

- 首次嘗試日期。
- 嘗試與提交次數。
- 最佳執行時間與記憶體。
- 操作次數或結構高度。
- 使用提示層級。
- 是否閱讀解答。
- 是否完成一週後重寫。
- 最近一次失敗分類。

完成題目不等於永久通過。核心題在 7 天和 30 天後安排間隔重測。
