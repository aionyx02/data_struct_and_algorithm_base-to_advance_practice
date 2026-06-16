# Segment Tree Lazy Mixed Tags

Maintain an array with range-add, range-assignment, and inclusive range-sum
queries. Use one Segment Tree that can compose both lazy tag types correctly.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `range_add left right delta`: add `delta` to every value in the inclusive
  range.
- `range_assign left right value`: replace every value in the inclusive range.
- `add index delta`: add `delta` to one value.
- `set index value`: replace one value.
- `get index`: print one value.
- `range left right`: print the inclusive range sum.
- `all`: print the sum of the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use lazy propagation with both assignment and add tags.
- A later assignment overrides any pending add on the covered segment.
- A later add after a pending assignment must increase the pending assignment
  value, or otherwise be pushed after that assignment.
- The assignment tag must distinguish "no pending assignment" from "assign
  zero".
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
