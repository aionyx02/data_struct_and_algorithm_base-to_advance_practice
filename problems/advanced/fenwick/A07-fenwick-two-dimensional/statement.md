# Two-Dimensional Fenwick Tree

Maintain a zero-initialized `rowCount` by `columnCount` grid with point updates
and rectangular sum queries. Store a two-dimensional Fenwick tree in explicit
array storage.

## Input

The first line contains `rowCount`, `columnCount`, and `operationCount`.
Rows are in `[0, rowCount)` and columns are in `[0, columnCount)`.

Each following line is one operation:

- `add row column delta`: add `delta` to one cell.
- `set row column value`: replace one cell's value.
- `get row column`: print one cell's value.
- `prefix row column`: print the sum of the rectangle from `(0, 0)` through
  `(row, column)`, inclusive.
- `rectangle top left bottom right`: print the inclusive rectangular sum.
- `shape`: print `rowCount columnCount`.

Print `OUT_OF_RANGE` when a point lies outside the grid, or when a rectangle
has invalid bounds or reversed endpoints. Successful updates print nothing.

## Restrictions

- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and sum query must run in `O(log R log C)` time.
- Storage must be `O(R * C)`.
