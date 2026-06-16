# Segment Tree Positional Search

Maintain an array while supporting point updates, range maximum queries, and
positional searches. Use a Segment Tree whose node aggregate stores the maximum
value in that segment, then descend only into children that can still contain an
answer.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set index value`: replace one value.
- `add index delta`: add `delta` to one value.
- `get index`: print one value.
- `range_max left right`: print the maximum value in the inclusive range.
- `first_at_least left threshold`: print the smallest index `i >= left` whose
  value is at least `threshold`, or `NONE` when no such index exists.
- `last_at_least right threshold`: print the largest index `i <= right` whose
  value is at least `threshold`, or `NONE` when no such index exists.
- `all_max`: print the maximum value in the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use a Segment Tree with maximum aggregates.
- Positional searches must use the tree aggregate to prune subtrees and run in
  `O(log n)` time.
- Search commands must respect their `left` or `right` boundary.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update, query, and search must run in `O(log n)` time.
- Storage must be `O(n)`.
