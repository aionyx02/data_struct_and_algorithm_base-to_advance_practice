# Fixed Array ADT

Implement the basic operations of a fixed-size array.

## Input

The first line contains `n` and `operationCount`.

The second line contains `n` integers. It is empty when `n` is zero.

Each following line is one operation:

- `get i`: print the value at zero-based index `i`.
- `set i x`: replace the value at index `i` with `x`.
- `swap i j`: exchange the values at indices `i` and `j`.
- `front`: print the first value.
- `back`: print the last value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is invalid. Print `EMPTY` for `front` or
`back` when `n` is zero. Successful `set` and `swap` operations print nothing.

## Restrictions

- Do not use `std::array`, `std::deque`, or `std::vector`.
- Every operation must run in `O(1)` time.
- Storage must be `O(n)`.
