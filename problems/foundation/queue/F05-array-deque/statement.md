# Fixed Capacity Array Deque

Implement a fixed-capacity double-ended queue using a circular array.

## Input

The first line contains `capacity` and `operationCount`.

Each following line is one operation:

- `push_front x`: add `x` at the front; print `FULL` if full.
- `push_back x`: add `x` at the back; print `FULL` if full.
- `pop_front`: remove and print the front value; print `EMPTY` if empty.
- `pop_back`: remove and print the back value; print `EMPTY` if empty.
- `front`: print the front value; print `EMPTY` if empty.
- `back`: print the back value; print `EMPTY` if empty.
- `size`: print the number of stored values.
- `empty`: print `true` or `false`.

## Restrictions

- Do not use `std::deque`, `std::list`, `std::queue`, or `std::vector`.
- Both ends must reuse slots through circular indexing.
- Every operation must run in `O(1)` time.
- Storage must be `O(capacity)`.
