# Undirected Adjacency Matrix

Implement a simple undirected graph using a square adjacency matrix.

Input starts with `vertexCount operationCount`. Operations are `add u v`,
`remove u v`, `has u v`, `degree u`, `neighbors u`, `row u`, `edge_count`,
and `clear`.

A duplicate edge prints `DUPLICATE`; removing a missing edge prints
`NOT_FOUND`; a successful removal prints `REMOVED`. Self-loops print
`SELF_LOOP`, and invalid vertices print `OUT_OF_RANGE`. Neighbors are printed
in increasing order, `row` prints zeroes and ones, and empty views print
`EMPTY`.

Do not use standard containers.
