# Sparse Fenwick Tree

Maintain point values in a huge coordinate universe `[0, U)` without allocating
an array of size `U`. Store only touched Fenwick nodes in
`std::unordered_map<long long, long long>`.

## Input

The first line contains `U` and `operationCount`. `U` is between `1` and
`1000000000000`, inclusive.

Each following line is one operation:

- `add coordinate delta`: add `delta` to one coordinate.
- `set coordinate value`: replace one coordinate's value.
- `get coordinate`: print one coordinate's value.
- `prefix coordinate`: print the sum from coordinate `0` through `coordinate`,
  inclusive.
- `range left right`: print the inclusive sum from `left` through `right`.
- `nodes`: print the number of stored Fenwick nodes whose aggregate is nonzero.

Print `OUT_OF_RANGE` when a coordinate is outside `[0, U)` or a range has
`left > right`. Successful updates print nothing. All values start at zero.
When an update makes a stored Fenwick node's aggregate zero, erase that node.

## Restrictions

- Use `std::unordered_map` for sparse Fenwick nodes. Do not allocate storage
  proportional to `U`.
- Do not use `std::vector`, `std::map`, `std::set`, or `std::list`.
- Every update and sum query must take `O(log U)` expected time.
- With `K` nonzero point coordinates, storage must be `O(K log U)`.
