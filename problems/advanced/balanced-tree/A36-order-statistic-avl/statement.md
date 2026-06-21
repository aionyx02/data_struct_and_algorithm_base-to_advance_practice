# Order Statistic AVL Tree

Implement an AVL tree over unique integer keys, augmented with subtree sizes so
it answers order-statistic queries in `O(log n)`. The tree stays height-balanced
after every insertion and deletion; two-child deletion replaces the node with
its in-order successor.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `insert key`: insert `key`. Successful insertion prints nothing; inserting an
  existing key prints `DUPLICATE`.
- `erase key`: delete `key`. Print `REMOVED`, or `NOT_FOUND` if absent.
- `contains key`: print `true` or `false`.
- `select k`: print the `k`-th smallest key (1-indexed). Print `OUT_OF_RANGE`
  when `k < 1` or `k > size`.
- `rank key`: print the number of stored keys strictly less than `key` (`key`
  need not be present).
- `range_count low high`: print how many stored keys lie in the inclusive range
  `[low, high]`; print `0` when `low > high`.
- `min` / `max`: print the smallest / largest key, or `EMPTY`.
- `size`: print the number of stored keys.
- `height`: print the tree height in levels (an empty tree is `0`, a single node
  is `1`).
- `inorder`: print all keys in ascending order, space-separated, or `EMPTY`.
- `clear`: remove all keys (prints nothing).

## Restrictions

- Maintain a subtree size in every node and keep it correct across rotations,
  so `select`, `rank`, and `range_count` are `O(log n)`, not linear scans.
- Keep the AVL balance invariant after every insertion and deletion; the
  reported `height` reflects the rebalanced tree.
- `rank key` counts keys strictly less than `key`; `range_count` is inclusive on
  both ends.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::multiset`,
  `std::vector`, or `std::list`.
