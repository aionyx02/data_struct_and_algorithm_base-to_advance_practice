# Recursive Segment Tree Range Sum

Maintain an array with point updates and inclusive range-sum queries. Build and
traverse a recursive Segment Tree whose nodes represent closed intervals.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set index value`: replace one array value.
- `add index delta`: add `delta` to one array value.
- `get index`: print one array value.
- `range left right`: print the inclusive sum from `left` through `right`.
- `all`: print the sum of the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Implement recursive build, point-update, and range-query functions.
- Use explicit array storage for the values and tree. Do not use `std::vector`,
  `std::map`, `std::unordered_map`, `std::set`, or `std::list`.
- Every point update and range query must run in `O(log n)` time.
- Storage must be `O(n)`; an array of approximately `4n` tree slots is allowed.
