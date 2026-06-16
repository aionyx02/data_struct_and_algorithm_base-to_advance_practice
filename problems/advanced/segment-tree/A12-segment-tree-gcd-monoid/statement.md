# Segment Tree GCD Monoid

Maintain signed array values under point updates while answering inclusive
range greatest-common-divisor queries. Segment Tree aggregates are always
nonnegative, and `gcd(0, x) = |x|` makes zero the no-overlap identity.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set index value`: replace one array value.
- `add index delta`: add `delta` to one array value.
- `get index`: print the signed value at one index.
- `gcd left right`: print the nonnegative GCD of the inclusive range.
- `all`: print the nonnegative GCD of the entire array.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use a Segment Tree whose combine operation is GCD and whose no-overlap
  identity is zero.
- Normalize leaf aggregates so query results are never negative.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every point update and range query must run in `O(log n)` time.
- Storage must be `O(n)`.
