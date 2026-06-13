# Split Singly Linked Lists

Maintain two singly linked lists, `A` and `B`.

## Operations

- `push_a x`, `push_b x`
- `split_a i`: when `B` is empty, detach suffix `A[i..]` into `B`.
- `split_b i`: when `A` is empty, detach suffix `B[i..]` into `A`.
- `front_a`, `back_a`, `size_a`, `empty_a`
- `front_b`, `back_b`, `size_b`, `empty_b`
- `clear_a`, `clear_b`

Valid split indices are from zero through the source size. Print
`OUT_OF_RANGE` for an invalid index and `TARGET_NOT_EMPTY` when the destination
already owns nodes.

## Restrictions

- Do not use standard sequence containers.
- Split by changing links; do not copy node values.
- Preserve both lists' head, tail, and size invariants.
