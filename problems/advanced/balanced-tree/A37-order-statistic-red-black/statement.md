# Order Statistic Red-Black Tree

Implement a red-black tree over unique integer keys. Maintain parent links,
colors, and subtree sizes through insertion and deletion fix-up so ordered
queries remain logarithmic.

The first line contains `operationCount`. Operations are `insert key` (print
`DUPLICATE` only when already present), `erase key` (`REMOVED` or `NOT_FOUND`),
`contains key`, `select k` (1-indexed, or `OUT_OF_RANGE`), `rank key` (number of
keys strictly smaller), inclusive `range_count low high`, strict `predecessor`
and `successor` (`NONE` when absent), `min`, `max`, `size`, `inorder`, and
`clear`. Empty extrema and traversal print `EMPTY`.

Use the standard red-black invariants: a black root, no red-red edge, equal
black height on every root-to-NIL path, and black sentinel leaves. Both update
fix-ups and subtree-size maintenance must work after rotations and transplants.
Do not use an ordered library container as the tree.
