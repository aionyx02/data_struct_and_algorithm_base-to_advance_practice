# Linked Deque

Implement a deque with a doubly linked structure and explicit head and tail
pointers.

Input starts with `operationCount`. Operations are `push_front value`,
`push_back value`, `pop_front`, `pop_back`, `front`, `back`, `size`, `empty`,
and `clear`.

Pop prints the removed value. Pop, front, and back print `EMPTY` when the deque
is empty. Every operation except `clear` must be `O(1)`; `clear` is `O(n)`.
Do not use standard containers.
