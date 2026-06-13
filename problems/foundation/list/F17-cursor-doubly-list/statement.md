# Cursor Doubly Linked List

Implement a fixed-capacity doubly linked list using array indices as cursors
instead of raw pointers. Maintain a free list of unused slots.

## Input

The first line contains `capacity` and `operationCount`.

- `push_front x`, `push_back x`
- `pop_front`, `pop_back`
- `insert i x`, `erase i`, `get i`
- `front`, `back`, `size`, `empty`
- `free`: print available slots.
- `clear`: return all active slots to the free list.

Print `FULL` when insertion has no free slot, `OUT_OF_RANGE` for invalid
indices, and `EMPTY` for empty end operations.

## Restrictions

- Do not use standard sequence containers.
- Allocate exactly `capacity` cursor nodes before processing operations.
- Store links as integer indices with a documented null cursor.
- Acquire and release cursor nodes in `O(1)` time.
