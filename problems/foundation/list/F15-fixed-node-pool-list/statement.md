# Fixed Node Pool List

Implement a singly linked list backed by a fixed-capacity node pool and a free
list. Erased nodes must become available for later insertions.

## Input

The first line contains `capacity` and `operationCount`.

- `insert i x`: insert before index `i`; print `FULL` when no node is free.
- `erase i`: remove and print index `i`.
- `get i`, `front`, `back`, `size`, `empty`
- `free`: print the number of available pool nodes.
- `clear`: release all active nodes back to the pool.

Print `OUT_OF_RANGE` for invalid indexed operations and `EMPTY` for empty end
queries.

## Restrictions

- Do not use standard sequence containers.
- Allocate storage for exactly `capacity` nodes before processing operations.
- Node acquire and release must be `O(1)`.
- Reuse released nodes; total successful insertions may exceed capacity.
