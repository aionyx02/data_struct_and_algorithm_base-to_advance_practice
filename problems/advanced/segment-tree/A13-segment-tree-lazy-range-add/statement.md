# Segment Tree Lazy Range Add

Maintain an array with range-add updates and inclusive range-sum queries. Use a
Segment Tree with a pending add tag on each node, and push that tag before
descending into children.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `range_add left right delta`: add `delta` to every value in the inclusive
  range.
- `add index delta`: add `delta` to one value.
- `set index value`: replace one value.
- `get index`: print one value.
- `range left right`: print the inclusive range sum.
- `all`: print the sum of the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use lazy propagation with a range-add tag.
- A point `set` may be implemented by querying the current value and applying a
  point add with the difference.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
