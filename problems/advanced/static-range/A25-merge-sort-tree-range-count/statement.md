# Merge Sort Tree Range Count

Answer order-statistic queries over a static array with a Merge Sort Tree. Each
node of the segment tree stores the values from its interval in sorted order.
Range counting visits `O(log n)` canonical nodes and performs a binary search in
each node, for `O(log^2 n)` per query.

The array never changes, so there are no update operations.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `count_lt left right value`: print how many values in the inclusive range are
  strictly less than `value`.
- `count_le left right value`: print how many values in the inclusive range are
  less than or equal to `value`.
- `kth left right k`: print the 1-based `k`-th smallest value in the inclusive
  range.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, when a range has
reversed endpoints (`left > right`), or when `k` is outside
`[1, right - left + 1]`.

## Restrictions

- Build a Merge Sort Tree whose node arrays are sorted copies of the covered
  intervals.
- `count_lt` must use lower-bound semantics, and `count_le` must use upper-bound
  semantics so duplicate values are handled correctly.
- Implement `kth` by using the Merge Sort Tree count query over the sorted set of
  candidate values.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- The build is `O(n log n)` time and space; count queries are `O(log^2 n)`.
