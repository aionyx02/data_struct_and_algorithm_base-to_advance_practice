# Two-Dimensional Row-Major Array

Implement a two-dimensional integer array on top of one contiguous allocation.

Input starts with `rows columns operationCount`. All cells initially contain
zero. Operations are `set r c value`, `get r c`, `index r c`, `coord linear`,
`fill value`, `rows`, and `columns`.

`index` prints the zero-based row-major offset. `coord` prints `row column`.
Invalid coordinates or linear offsets print `OUT_OF_RANGE`. Do not use
`std::vector` or `std::array`.
