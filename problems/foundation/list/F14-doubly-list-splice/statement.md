# Splice Doubly Linked Lists

Maintain doubly linked lists `A` and `B`.

## Operations

- `push_front_a x`, `push_back_a x`, and the corresponding `_b` forms.
- `splice_b_to_a i`: insert all nodes of `B` before index `i` in `A`.
- `splice_a_to_b i`: insert all nodes of `A` before index `i` in `B`.
- `pop_front_a`, `pop_back_a`, `pop_front_b`, `pop_back_b`
- `get_a i`, `get_b i`, `size_a`, `size_b`, `empty_a`, `empty_b`

Valid splice indices range from zero through destination size. Print
`OUT_OF_RANGE` for invalid indices and `EMPTY` for empty pops. Splicing empties
the source list.

## Restrictions

- Do not use standard sequence containers.
- Locate the insertion point from the nearer end.
- Transfer nodes by relinking; do not copy values.
- Preserve both forward and backward links.
