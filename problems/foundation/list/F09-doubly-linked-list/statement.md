# Doubly Linked List

Implement a doubly linked list with explicit `head` and `tail` pointers.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `push_front x`, `push_back x`
- `pop_front`, `pop_back`: remove and print one end.
- `insert i x`: insert before index `i`; `i == size` appends.
- `erase i`: remove and print index `i`.
- `get i`, `front`, `back`, `size`, `empty`
- `clear`

Print `OUT_OF_RANGE` for an invalid indexed operation. Print `EMPTY` for an
empty end operation.

## Restrictions

- Do not use `std::deque`, `std::forward_list`, `std::list`, or `std::vector`.
- Maintain consistent `next` and `previous` links after every mutation.
- Traverse indexed operations from the nearer end.
- Release every allocated node.
