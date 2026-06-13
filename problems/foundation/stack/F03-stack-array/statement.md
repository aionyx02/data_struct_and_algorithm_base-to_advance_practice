# Fixed Capacity Array Stack

Implement a fixed-capacity stack using an array.

## Input

The first line contains `capacity` and `operationCount`.

Each following line is one operation:

- `push x`: push `x`; print `FULL` if the stack is full.
- `pop`: remove and print the top value; print `EMPTY` if the stack is empty.
- `top`: print the top value; print `EMPTY` if the stack is empty.
- `size`: print the current number of values.
- `empty`: print `true` or `false`.

## Restrictions

- Do not use `std::stack`.
- Do not use `std::vector`.
- Each operation must run in `O(1)` time.
- Storage must be `O(capacity)`.
