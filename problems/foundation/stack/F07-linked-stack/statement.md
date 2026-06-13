# Linked Stack

Implement an unbounded LIFO stack using dynamically allocated linked nodes.

## Input

The first line contains `operationCount`. Each following line is one operation:

- `push x`
- `pop`: remove and print the top value.
- `top`: print the top value.
- `size`, `empty`
- `clear`: delete every node.

Print `EMPTY` when `pop` or `top` is called on an empty stack.

## Restrictions

- Do not use `std::deque`, `std::forward_list`, `std::list`, `std::stack`, or `std::vector`.
- Every operation except `clear` must run in `O(1)` time.
- Release every allocated node.
