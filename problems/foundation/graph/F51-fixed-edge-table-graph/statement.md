# Fixed Capacity Edge Table Graph

Implement a simple undirected graph as a fixed-capacity table of edge records.
Each live edge has a unique integer identifier and occupies one reusable slot.

Input starts with `vertexCount edgeCapacity operationCount`. Operations are
`add edgeId u v`, `erase edgeId`, `slot edgeId`, `edge_at slot`,
`endpoints edgeId`, `degree v`, `neighbors v`, `edge_count`, `free_slots`, and
`clear`.

`add` prints its allocated slot and must choose the smallest free slot.
`erase` prints the released slot. Duplicate live identifiers or endpoint pairs
print `DUPLICATE`; a full table prints `FULL`. Missing identifiers print
`NOT_FOUND`; an empty valid slot prints `EMPTY`; invalid slots or vertices
print `OUT_OF_RANGE`; self-loops print `SELF_LOOP`.

Do not use standard containers.
