# Concatenate Singly Linked Lists

Maintain two singly linked lists, `A` and `B`.

## Operations

The first line contains `operationCount`.

- `push_a x`, `push_b x`: append to one list.
- `concat_ab`: move every node from `B` to the back of `A`.
- `concat_ba`: move every node from `A` to the back of `B`.
- `pop_a`, `pop_b`: remove and print the front.
- `front_a`, `back_a`, `size_a`, `empty_a`
- `front_b`, `back_b`, `size_b`, `empty_b`
- `clear_a`, `clear_b`

An empty end operation prints `EMPTY`. Concatenation transfers ownership and
leaves the source list empty.

## Restrictions

- Do not use standard sequence containers.
- Concatenation must relink nodes in `O(1)` time without copying values.
- The source list must remain independently reusable after transfer.
