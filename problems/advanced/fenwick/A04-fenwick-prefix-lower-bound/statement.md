# Fenwick Prefix Lower Bound

Maintain a sequence of `n` non-negative integers under point additions, prefix
sum queries, and a prefix lower bound search. Because every value is
non-negative the prefix sums are non-decreasing, so the lower bound can be found
by walking the Fenwick (binary indexed) tree in `O(log n)` instead of scanning.

## Input

The first line contains `n` and `operationCount`.

The second line contains `n` non-negative integers. It is empty when `n` is zero.

Each following line is one operation, using zero-based indices:

- `add i v`: add `v` (with `v >= 0`) to the value at index `i`. `i` is always valid.
- `get i`: print the current value at index `i`.
- `prefix i`: print the sum of indices `0` through `i` inclusive.
- `lower_bound t`: print the smallest index `i` whose prefix sum
  (indices `0` through `i`) is at least `t`. Print `NONE` when the total sum is
  below `t`.
- `size`: print `n`.

Print `OUT_OF_RANGE` when `get` or `prefix` receives an index outside `[0, n)`.
Successful `add` operations print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time. A linear scan for
  `lower_bound` does not meet the intended complexity.
- Storage must be `O(n)`.
