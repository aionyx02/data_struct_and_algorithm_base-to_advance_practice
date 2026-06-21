# Leftist Heap Meld

Maintain several min-leftist heaps. The first line contains `heapCount
operationCount`. Operations are `push heap key`, `meld destination source`
(source becomes empty), `top heap`, `pop heap` (prints and removes the minimum),
`size heap`, `npl heap` (root null-path length, with empty `-1` and leaf `0`),
`preorder heap`, and `clear heap`. Empty top, pop, or traversal prints `EMPTY`.

Melding a heap with itself is a no-op. After every recursive meld, restore
`npl(left) >= npl(right)` and update both NPL and subtree size. Do not use a
library priority queue or ordered multiset.
