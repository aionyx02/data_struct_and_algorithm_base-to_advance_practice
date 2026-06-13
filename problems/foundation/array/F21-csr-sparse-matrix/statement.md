# Compressed Sparse Row Matrix

Build a static CSR representation from row-major, unique, nonzero triplets.

Input starts with `rows columns nonzeroCount operationCount`, followed by
`nonzeroCount` lines containing `row column value`. Operations are:

- `get r c`
- `row_nnz r`
- `row_sum r`
- `row_start r`
- `term i`
- `nonzero`

`row_start r` prints the offset stored in the CSR row-offset array and accepts
`0 <= r <= rows`. `term i` prints `column value` from the compressed arrays.
Invalid coordinates, rows, offsets, or term indices print `OUT_OF_RANGE`.

Use `O(rows + nonzeroCount)` storage. Do not use maps or `std::vector`.
