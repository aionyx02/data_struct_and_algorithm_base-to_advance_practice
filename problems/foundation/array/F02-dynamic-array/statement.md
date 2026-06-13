# Manual Dynamic Array

Implement a dynamic array whose capacity doubles whenever a push would exceed
the current capacity. The initial capacity is always positive. Do not shrink
the capacity after `pop` or `clear`.

## Input

The first line contains `initialCapacity` and `operationCount`.

Each following line is one operation:

- `push x`: append `x`.
- `pop`: remove and print the last value; print `EMPTY` when no value exists.
- `get i`: print the value at zero-based index `i`.
- `set i x`: replace the value at index `i`.
- `size`: print the number of stored values.
- `capacity`: print the current capacity.
- `clear`: remove every value without changing the capacity.

Print `OUT_OF_RANGE` when `get` or `set` receives an invalid index. A successful
`push`, `set`, or `clear` operation prints nothing.

## Restrictions

- Do not use `std::array`, `std::deque`, or `std::vector`.
- Random access must run in `O(1)` time.
- `push` must run in amortized `O(1)` time.
- Storage must be `O(capacity)`.
