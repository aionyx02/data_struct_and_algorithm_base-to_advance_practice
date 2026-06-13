# BST Replacement Node Trace

Implement a binary search tree whose nodes have both a unique identifier and a
unique key. Deletion must physically transplant the inorder-successor node
instead of copying only its key.

Input starts with `operationCount`. Operations are:

- `insert id key` and `erase key`
- `find key`, `root`, `inorder`, `size`, and `clear`
- `key id`, `parent id`, `left id`, and `right id`

A duplicate live identifier or key prints `DUPLICATE`. A successful erase
prints the identifier of the removed target node. Missing keys or identifiers
print `NOT_FOUND`; missing links print `NONE`. `inorder` prints identifiers in
ascending key order, or `EMPTY`.

Do not use standard containers.
