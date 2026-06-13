# Sentinel Doubly Linked List

Implement a doubly linked list with permanent dummy head and tail sentinel
nodes. Data nodes always remain between the sentinels.

## Operations

The first line contains `operationCount`.

- `push_front x`, `push_back x`
- `pop_front`, `pop_back`
- `insert i x`: insert before index `i`; `i == size` appends.
- `erase i`, `get i`
- `front`, `back`, `size`, `empty`, `clear`

Print `OUT_OF_RANGE` for invalid indexed operations and `EMPTY` for empty end
operations.

## Restrictions

- Do not use standard sequence containers.
- Keep both sentinel nodes alive until program shutdown.
- Link and unlink a known node without empty-list special cases.
- Traverse indexed operations from the nearer end.
