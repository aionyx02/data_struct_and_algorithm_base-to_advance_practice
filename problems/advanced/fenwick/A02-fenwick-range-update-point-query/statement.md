# Fenwick Range Update With Point Query

Maintain a sequence of `n` integers under range updates and point queries. Use a
difference Fenwick (binary indexed) tree so every update and query runs in
`O(log n)`: storing the differences of the sequence turns a range add into two
point updates, and a point read into one prefix query.

## Input

The first line contains `n` and `operationCount`.

The second line contains `n` integers. It is empty when `n` is zero.

Each following line is one operation, using zero-based indices:

- `range_add l r v`: add `v` to every value at indices `l` through `r` inclusive.
- `add i v`: add `v` to the single value at index `i`.
- `get i`: print the current value at index `i`.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is invalid, or when `range_add` is given
`l > r`. Successful `range_add` and `add` operations print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
