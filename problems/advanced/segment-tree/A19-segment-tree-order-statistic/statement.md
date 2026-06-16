# Segment Tree Order Statistic

Maintain a multiset of keys drawn from the universe `[0, U)` with a count Segment
Tree. Each leaf stores the multiplicity of one key and every internal node stores
the total count in its subtree. Rank queries become range counts, and the k-th
smallest key is found by descending the tree: enter the left child when its stored
count is at least `k`, otherwise enter the right child after subtracting the left
count from `k`.

## Input

The first line contains `U` (the universe size) and `operationCount`.

Each following line is one operation. Keys are in `[0, U)`:

- `insert x`: add one occurrence of key `x`.
- `erase x`: remove one occurrence of key `x`.
- `count x`: print how many occurrences of key `x` are stored.
- `less x`: print how many stored elements are strictly less than `x`
  (`x` may equal `U`).
- `kth k`: print the `k`-th smallest key (one-based) using the Segment Tree
  descend. Print `NONE` when `k` is below `1` or above the current multiset size.
- `size`: print the total number of stored elements.

Print `OUT_OF_RANGE` when `insert`, `erase`, or `count` receives a key outside
`[0, U)`, or when `less` receives a bound outside `[0, U]`. Print `NOT_FOUND`
when `erase` targets a key with no stored occurrence. Successful `insert` and
`erase` operations print nothing.

## Restrictions

- Use a count Segment Tree over the value universe `[0, U)`.
- The k-th query must descend the tree in `O(log U)` time, subtracting the left
  subtree count when it moves right. Do not scan keys linearly.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- Every update and query must run in `O(log U)` time.
- Storage must be `O(U)`.
