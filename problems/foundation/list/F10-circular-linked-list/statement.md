# Circular Singly Linked List

Implement a circular singly linked list using a tail pointer. The logical front
is `tail->next`.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `push_front x`, `push_back x`
- `pop_front`: remove and print the front value.
- `front`, `back`, `size`, `empty`
- `rotate`: move the logical front to the back in `O(1)` time.
- `clear`

Print `EMPTY` when an empty end operation is requested. `rotate` does nothing
for zero or one node.

## Restrictions

- Do not use `std::deque`, `std::forward_list`, `std::list`, `std::queue`, or `std::vector`.
- For a non-empty list, the tail's next link must always point to the front.
- End operations and `rotate` must run in `O(1)` time.
- Release every allocated node without looping forever.
