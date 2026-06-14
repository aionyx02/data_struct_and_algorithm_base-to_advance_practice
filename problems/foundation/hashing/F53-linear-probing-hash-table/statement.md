# Linear Probing Hash Table

Implement a fixed-capacity integer set using normalized modulo hashing and
linear probing. Deletion is not part of this exercise.

Input starts with `capacity operationCount`. Operations are `insert key`,
`contains key`, `slot key`, `probe_count key`, `table`, `size`, `capacity`,
and `clear`.

`insert` prints the allocated slot. Duplicate keys print `DUPLICATE`; an
unsuccessful insertion after probing the whole table prints `FULL`. `slot`
prints `NOT_FOUND` when absent. `probe_count` counts examined slots through
the found key, first empty slot, or full-table exhaustion. `table` prints one
token per slot, using `.` for empty slots.

Do not use standard containers.
