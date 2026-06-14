# Fenwick Point Update With Prefix And Range Sum

Maintain a sequence of `n` integers under point updates and prefix or range
sum queries. Use a Fenwick (binary indexed) tree so every update and query runs
in `O(log n)`.

## Input

The first line contains `n` and `operationCount`.

The second line contains `n` integers. It is empty when `n` is zero.

Each following line is one operation, using zero-based indices:

- `add i v`: add `v` to the value at index `i`. `i` is always valid.
- `set i v`: replace the value at index `i` with `v`.
- `get i`: print the current value at index `i`.
- `prefix i`: print the sum of indices `0` through `i` inclusive.
- `range l r`: print the sum of indices `l` through `r` inclusive.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is invalid, or when `range`/`prefix` receives
an index outside `[0, n)`, or when `range` is given `l > r`. Successful `add`
and `set` operations print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
