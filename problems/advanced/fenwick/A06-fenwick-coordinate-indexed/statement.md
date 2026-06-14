# Coordinate-Indexed Fenwick Tree

Maintain values on every integer coordinate in the closed interval
`[minimumCoordinate, maximumCoordinate]`. Map each coordinate directly to an
array slot and use a Fenwick (binary indexed) tree for updates and sums.
Coordinate compression is not part of this exercise.

## Input

The first line contains `minimumCoordinate`, `maximumCoordinate`, and
`operationCount`. The coordinate span is at most `200000`.

Each following line is one operation:

- `add coordinate delta`: add `delta` to one coordinate.
- `set coordinate value`: replace the value at one coordinate.
- `get coordinate`: print the value at one coordinate.
- `prefix coordinate`: print the sum from `minimumCoordinate` through
  `coordinate`, inclusive.
- `range left right`: print the inclusive sum from `left` through `right`.
- `bounds`: print `minimumCoordinate maximumCoordinate`.

Print `OUT_OF_RANGE` when a coordinate is outside the declared interval or a
range has `left > right`. Successful updates print nothing. All values start at
zero.

## Restrictions

- Use explicit array storage with a direct coordinate offset.
- Do not use coordinate compression, `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and sum query must run in `O(log S)` time, where `S` is the
  coordinate span.
- Storage must be `O(S)`.
