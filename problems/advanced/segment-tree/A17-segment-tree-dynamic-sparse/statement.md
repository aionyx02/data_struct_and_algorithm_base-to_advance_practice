# Dynamic Sparse Segment Tree

Maintain point values in a huge coordinate universe `[0, U)` without allocating
an array or full Segment Tree over all coordinates. Values start at zero. Build
tree nodes only for update paths that actually change a coordinate value.

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
- `all`: print the sum over the whole universe.
- `nodes`: print the number of allocated Segment Tree nodes.
- `universe`: print `U`.

Print `OUT_OF_RANGE` when a coordinate is outside `[0, U)`, or when a range has
invalid or reversed endpoints. Successful updates print nothing.

## Allocation Rules

- The root node for `[0, U - 1]` exists after construction, so `nodes` starts at
  `1`.
- Allocate only the one missing child that an update descends into.
- Do not allocate nodes during `get`, `prefix`, `range`, `all`, `nodes`, or
  `universe`.
- `add coordinate 0` and `set coordinate current_value` are no-ops and must not
  allocate nodes.
- Once a node is allocated, keep it allocated even if later updates make its sum
  zero.

## Restrictions

- Use a dynamic Segment Tree over the coordinate universe.
- Do not allocate storage proportional to `U`.
- Do not use `std::vector`, `std::map`, `std::unordered_map`, `std::set`, or
  `std::list`.
- Every update and sum query must run in `O(log U)` time.
- With `K` coordinates changed by nonzero deltas, storage must be `O(K log U)`.
