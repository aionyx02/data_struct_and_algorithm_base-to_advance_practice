# Fenwick Frequency Table And Order Statistics

Maintain a multiset of keys drawn from the universe `[0, U)` using a frequency
Fenwick (binary indexed) tree. Counts are stored per key, so rank and order
statistic queries both run in `O(log U)`: a rank is a prefix sum, and the k-th
smallest key is a prefix lower bound walk over the tree.

## Input

The first line contains `U` (the universe size) and `operationCount`.

Each following line is one operation. Keys are in `[0, U)`:

- `insert x`: add one occurrence of key `x`.
- `erase x`: remove one occurrence of key `x`.
- `count x`: print how many occurrences of key `x` are stored.
- `less x`: print how many stored elements are strictly less than `x`
  (`x` may equal `U`).
- `kth k`: print the `k`-th smallest key (one-based). Print `NONE` when `k` is
  below `1` or above the current multiset size.
- `size`: print the total number of stored elements.

Print `OUT_OF_RANGE` when `insert`, `erase`, or `count` receives a key outside
`[0, U)`, or when `less` receives a bound outside `[0, U]`. Print `NOT_FOUND`
when `erase` targets a key with no stored occurrence. Successful `insert` and
`erase` operations print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- Every update and query must run in `O(log U)` time.
- Storage must be `O(U)`.
