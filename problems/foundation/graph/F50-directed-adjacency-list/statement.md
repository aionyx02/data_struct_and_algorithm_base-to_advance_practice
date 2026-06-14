# Directed Sorted Adjacency List

Implement a fixed-capacity simple directed graph using sorted outgoing
adjacency lists and explicit in-degree counters.

Input starts with `vertexCount edgeCapacity operationCount`. Operations are
`add from to`, `remove from to`, `has from to`, `out_degree v`, `in_degree v`,
`out_neighbors v`, `in_neighbors v`, `edge_count`, `capacity`, and `clear`.

At capacity, a new arc prints `FULL`. Duplicate arcs print `DUPLICATE`.
Successful removals print `REMOVED`; missing arcs print `NOT_FOUND`. Self-loops
print `SELF_LOOP`, invalid vertices print `OUT_OF_RANGE`, and neighbor views
must be increasing.

Do not use standard containers.
