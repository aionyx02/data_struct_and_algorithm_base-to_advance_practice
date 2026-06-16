# Segment Tree Lazy Range Assign

Maintain an array with range-assignment updates and inclusive range-sum queries.
Use a Segment Tree with an assignment tag that records whether a pending value
exists, and push that value before descending.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `range_assign left right value`: replace every value in the inclusive range.
- `set index value`: replace one value.
- `get index`: print one value.
- `range left right`: print the inclusive range sum.
- `all`: print the sum of the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use lazy propagation with a range-assignment tag.
- The tag must distinguish "no pending assignment" from "assign zero".
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- Storage must be `O(n)`.
