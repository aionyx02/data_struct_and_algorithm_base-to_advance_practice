# Singly Linked List

Implement a singly linked list with explicit `head` and `tail` pointers.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `push_front x`, `push_back x`
- `insert i x`: insert before zero-based index `i`; `i == size` appends.
- `erase i`: remove and print index `i`.
- `get i`: print index `i`.
- `front`, `back`, `size`, `empty`
- `clear`: delete every node.

Print `OUT_OF_RANGE` for an invalid indexed operation. Print `EMPTY` when
`front` or `back` is called on an empty list. Successful mutating operations
except `erase` print nothing.

## Restrictions

- Do not use `std::deque`, `std::forward_list`, `std::list`, or `std::vector`.
- Maintain both `head` and `tail`; end insertion must be `O(1)`.
- Indexed operations may take `O(index)` time.
- Release every allocated node.
