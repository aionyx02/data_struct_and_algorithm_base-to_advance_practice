# AVL Structural Validation

Validate a serialized AVL tree. Input starts with `nodeCount rootId`, followed
by `nodeCount` records: `id key leftId rightId parentId storedHeight`. `-1`
means no link.

Print the first applicable result in this priority:

1. `INVALID_LINK` for an out-of-range link or root with a parent.
2. `CYCLE` when traversal revisits a node.
3. `DISCONNECTED` when a node is unreachable.
4. `INVALID_PARENT` for a broken backlink.
5. `INVALID_ORDER` for a global BST-bound violation.
6. `INVALID_HEIGHT` for an incorrect stored height.
7. `UNBALANCED` when a balance factor is outside `[-1, 1]`.
8. `VALID`.

Do not use standard containers.
