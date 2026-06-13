# Three-Dimensional Row-Major Array

Implement a three-dimensional integer array on one contiguous allocation.

Input starts with `d1 d2 d3 operationCount`. All cells initially contain zero.
Operations are `set i j k value`, `get i j k`, `index i j k`, `coord linear`,
`fill value`, and `dims`.

`index` uses `(i * d2 + j) * d3 + k`. `coord` prints `i j k`. Invalid
coordinates or offsets print `OUT_OF_RANGE`. Do not use `std::vector` or
`std::array`.
