# Skew Heap Meld

Maintain several min-skew heaps. Input begins with `heapCount operationCount`.
Operations are `push heap key`, `meld destination source` (source becomes
empty), `top heap`, `pop heap`, `size heap`, `preorder heap`, and `clear heap`.
Top and pop print the minimum; empty top, pop, or traversal prints `EMPTY`.
Self-meld is a no-op.

The meld primitive recursively melds with the right subtree and then swaps the
children on every visited node. Maintain subtree sizes, and do not add leftist
NPL metadata or use a library priority queue.
