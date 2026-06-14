# Directed Adjacency Matrix

Implement a simple directed graph using an adjacency matrix. Self-loops are not
allowed.

Input starts with `vertexCount operationCount`. Operations are `add from to`,
`remove from to`, `has from to`, `out_degree v`, `in_degree v`,
`out_neighbors v`, `in_neighbors v`, `row v`, `edge_count`, and `clear`.

Duplicate arcs print `DUPLICATE`; successful removals print `REMOVED`; missing
arcs print `NOT_FOUND`. Invalid vertices print `OUT_OF_RANGE`, and self-loops
print `SELF_LOOP`. Neighbor views are increasing and empty views print `EMPTY`.

Do not use standard containers.
