# Undirected Sorted Adjacency List

Implement a fixed-capacity simple undirected graph using sorted adjacency
lists.

Input starts with `vertexCount edgeCapacity operationCount`. Operations are
`add u v`, `remove u v`, `has u v`, `degree v`, `neighbors v`, `edge_count`,
`capacity`, and `clear`.

At capacity, a new edge prints `FULL`. Duplicate edges print `DUPLICATE`.
Successful removals print `REMOVED`; missing edges print `NOT_FOUND`.
Self-loops print `SELF_LOOP`, invalid vertices print `OUT_OF_RANGE`, and
neighbors must remain in increasing order.

Do not use standard containers.
