# BST Predecessor And Successor

Build a linked binary search tree with unique integer keys and parent links.

Input starts with `operationCount`. Operations are:

- `insert key`
- `predecessor key` and `successor key`
- `min`, `max`, `size`, and `clear`

A duplicate insert prints `DUPLICATE`. Neighbor queries for a missing key print
`NOT_FOUND`; an existing key without that neighbor prints `NONE`. `min` and
`max` print `EMPTY` when the tree is empty.

Do not use standard containers.
