# Linked Binary Tree

Implement a binary tree whose nodes contain a unique integer identifier, a
value, and explicit parent, left, and right pointers.

Input starts with `operationCount`. Operations are:

- `root id value`
- `attach_left parentId childId value`
- `attach_right parentId childId value`
- `value id`, `parent id`, `left id`, `right id`
- `erase_leaf id`, `size`, and `clear`

Creating a root in a nonempty tree or attaching to a used child position prints
`OCCUPIED`. Reusing an identifier prints `DUPLICATE`. A missing identifier
prints `NOT_FOUND`. `erase_leaf` prints `NOT_LEAF` for an internal node;
otherwise it prints the removed value. Missing links print `NONE`.

Do not use standard containers.
