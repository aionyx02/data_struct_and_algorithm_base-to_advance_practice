# Inorder Threaded Binary Tree

Build a static double-threaded binary tree. Every missing left child becomes a
thread to the inorder predecessor, and every missing right child becomes a
thread to the inorder successor.

Input starts with `nodeCount rootId operationCount`. Each of the next
`nodeCount` lines is `id value leftId rightId`; identifiers are unique integers
from `0` through `nodeCount - 1`, and `-1` denotes a missing child.

Operations are `value id`, `predecessor id`, `successor id`, `left_link id`,
`right_link id`, and `inorder`.

Link queries print `CHILD id`, `THREAD id`, or `NONE`. A missing node prints
`NOT_FOUND`; a missing predecessor or successor prints `NONE`. `inorder` prints
identifiers separated by one space, or `EMPTY` for an empty tree.

Do not use standard containers.
