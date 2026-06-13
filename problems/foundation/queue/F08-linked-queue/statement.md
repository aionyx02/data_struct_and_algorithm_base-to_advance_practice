# Linked Queue

Implement an unbounded FIFO queue using linked nodes and explicit front/back pointers.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `enqueue x`
- `dequeue`: remove and print the front value.
- `front`, `back`, `size`, `empty`
- `clear`: delete every node.

Print `EMPTY` when `dequeue`, `front`, or `back` is called on an empty queue.

## Restrictions

- Do not use `std::deque`, `std::forward_list`, `std::list`, `std::queue`, or `std::vector`.
- Maintain both ends; every operation except `clear` must run in `O(1)` time.
- When the last node is removed, both end pointers must represent an empty queue.
- Release every allocated node.
