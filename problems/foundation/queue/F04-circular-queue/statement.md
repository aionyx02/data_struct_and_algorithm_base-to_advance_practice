# Fixed Capacity Circular Queue

Implement a fixed-capacity FIFO queue using a circular array.

## Input

The first line contains `capacity` and `operationCount`.

Each following line is one operation:

- `enqueue x`: add `x` at the back; print `FULL` if the queue is full.
- `dequeue`: remove and print the front value; print `EMPTY` if empty.
- `front`: print the front value; print `EMPTY` if empty.
- `back`: print the back value; print `EMPTY` if empty.
- `size`: print the number of stored values.
- `empty`: print `true` or `false`.

## Restrictions

- Do not use `std::deque`, `std::list`, `std::queue`, or `std::vector`.
- Reuse array slots after values are dequeued.
- Every operation must run in `O(1)` time.
- Storage must be `O(capacity)`.
