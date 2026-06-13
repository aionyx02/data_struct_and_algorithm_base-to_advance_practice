# Binary Search Tree Range View

Implement an integer binary search tree with unique keys.

Input starts with `operationCount`. Operations are:

- `insert key`
- `range low high` and `count low high`
- `lower key` for the smallest stored key greater than or equal to `key`
- `upper key` for the smallest stored key strictly greater than `key`
- `size` and `clear`

Range endpoints are inclusive. `range` prints matching keys in ascending order
or `EMPTY`; `count` prints the number of matches. Missing bound results print
`NONE`. A duplicate insert prints `DUPLICATE`.

Do not use standard containers.
