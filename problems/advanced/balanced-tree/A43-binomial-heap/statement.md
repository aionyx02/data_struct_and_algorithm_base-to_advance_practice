# Binomial Heap

Maintain several min-binomial heaps. The first line contains `heapCount
operationCount`. Operations are `push heap key`, `meld destination source`
(source becomes empty), `min heap`, `extract_min heap`, `size heap`,
`root_degrees heap`, and `clear heap`. Empty minimum or extraction prints
`EMPTY`; an empty degree list also prints `EMPTY`.

`root_degrees` prints root degrees in increasing order. Consolidate until at
most one binomial tree of each degree remains; link equal-degree roots below the
smaller key. Self-meld is a no-op. Do not use a library priority queue.
