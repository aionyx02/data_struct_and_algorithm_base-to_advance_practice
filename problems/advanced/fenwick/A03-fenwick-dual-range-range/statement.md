# Dual Fenwick Range Update With Range Query

Maintain a sequence of `n` integers under range updates and range sum queries.
A single difference tree is not enough: keep two Fenwick (binary indexed) trees
so a range add and a prefix sum both stay `O(log n)`. The second tree corrects
the `index * delta` term that a plain difference tree leaves out.

## Input

The first line contains `n` and `operationCount`.

The second line contains `n` integers. It is empty when `n` is zero.

Each following line is one operation, using zero-based indices:

- `range_add l r v`: add `v` to every value at indices `l` through `r` inclusive.
- `range_sum l r`: print the sum of indices `l` through `r` inclusive.
- `prefix i`: print the sum of indices `0` through `i` inclusive.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is invalid, when `range_add`/`range_sum` is
given `l > r`, or when `prefix` receives an index outside `[0, n)`. Successful
`range_add` operations print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
