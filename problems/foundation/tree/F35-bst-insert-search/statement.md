# Binary Search Tree Insert And Search

Implement a linked binary search tree with unique integer keys, integer values,
and explicit parent links.

Input starts with `operationCount`. Operations are:

- `insert key value`
- `contains key`, `value key`, and `parent key`
- `root`, `size`, and `clear`

A successful `insert` prints nothing. A duplicate key prints `DUPLICATE`.
`contains` prints `true` or `false`. Missing `value` and `parent` lookups print
`NOT_FOUND`; the root has parent `NONE`. `root` prints the root key or `EMPTY`.

Do not use standard containers.
