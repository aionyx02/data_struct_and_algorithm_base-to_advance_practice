# Binary Tree Traversal Views

Build a static linked binary tree and expose its three depth-first traversal
views.

Input starts with `nodeCount rootId operationCount`. Each of the next
`nodeCount` lines is `id value leftId rightId`; identifiers are unique integers
from `0` through `nodeCount - 1`, and `-1` denotes a missing child.

Operations are `preorder`, `inorder`, `postorder`, `height`, `size`, and
`value id`. Traversals print identifiers separated by one space. Empty
traversals print `EMPTY`; an empty tree has height `0`. Missing identifiers
print `NOT_FOUND`.

Do not use standard containers.
