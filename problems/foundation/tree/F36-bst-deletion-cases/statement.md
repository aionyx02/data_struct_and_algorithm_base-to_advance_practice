# Binary Search Tree Deletion Cases

Implement a linked binary search tree with unique integer keys and explicit
parent links. Support deletion of leaves, nodes with one child, and nodes with
two children.

Input starts with `operationCount`. Operations are:

- `insert key` and `erase key`
- `contains key`, `parent key`, `root`, `inorder`, `size`, and `clear`

A duplicate insert prints `DUPLICATE`. A successful erase prints the erased
key; a missing key prints `NOT_FOUND`. A missing parent lookup prints
`NOT_FOUND`, while the root has parent `NONE`. `inorder` prints sorted keys
separated by one space, or `EMPTY`.

Do not use standard containers.
