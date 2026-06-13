# Binary Search Tree Structural Validation

Inspect a static linked-tree representation and decide whether it is one valid
strict binary search tree.

Input starts with `nodeCount rootId operationCount`. Each node row is
`id key parentId leftId rightId`. Identifiers are `0` through
`nodeCount - 1`; `-1` means no link.

The representation is valid only when the root has no parent, every node is
reachable exactly once, every child points back to its parent, and every key
satisfies the full ancestor range. Operations are `valid`, `root`, `inorder`,
`size`, and `key id`, `parent id`, `left id`, `right id`.

`inorder` prints identifiers or `INVALID`; an empty valid tree prints `EMPTY`.
Missing identifiers print `NOT_FOUND`, missing links print `NONE`.

Do not use standard containers.
