# Segment Tree Minimum And Maximum Monoid

Maintain an array under point assignment while answering inclusive range
minimum and maximum queries. Every Segment Tree node stores the pair
`(minimum, maximum)` and combines children component-wise.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set index value`: replace one array value.
- `get index`: print one array value.
- `minimum left right`: print the inclusive range minimum.
- `maximum left right`: print the inclusive range maximum.
- `bounds left right`: print `minimum maximum` for the inclusive range.
- `all`: print `minimum maximum` for the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use a Segment Tree whose node aggregate is a minimum/maximum pair.
- The no-overlap identity must not change either component when combined.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every point update and range query must run in `O(log n)` time.
- Storage must be `O(n)`.
