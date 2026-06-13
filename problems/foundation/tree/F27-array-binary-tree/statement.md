# Array Binary Tree

Store a binary tree in a fixed zero-based array. For slot `i`, the parent is
`(i - 1) / 2`, the left child is `2 * i + 1`, and the right child is
`2 * i + 2`.

Input starts with `capacity operationCount`. Operations are `set i value`,
`erase i`, `get i`, `parent_index i`, `left_index i`, `right_index i`,
`occupied`, and `clear`.

Creating a non-root slot whose parent is empty prints `NO_PARENT`. Erasing an
occupied slot with an occupied child prints `HAS_CHILDREN`; otherwise erase
prints the removed value. Invalid indices print `OUT_OF_RANGE`. Queries on an
empty slot print `EMPTY`; a missing relative prints `NONE`.

Do not use standard containers.
