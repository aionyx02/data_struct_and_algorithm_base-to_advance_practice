# Reverse A Singly Linked List

Extend a singly linked list with an in-place `reverse` operation.

## Operations

The first line contains `operationCount`.

- `push_front x`, `push_back x`
- `reverse`
- `get i`, `front`, `back`, `size`, `empty`
- `clear`

Print `OUT_OF_RANGE` for an invalid index and `EMPTY` for an empty end query.

## Restrictions

- Do not use standard sequence containers.
- Maintain valid `head` and `tail` pointers after every reversal.
- `reverse` must take `O(n)` time and `O(1)` extra space.
- End insertions remain `O(1)`.
