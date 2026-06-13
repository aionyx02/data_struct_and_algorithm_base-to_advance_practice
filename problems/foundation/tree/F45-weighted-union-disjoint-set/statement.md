# Weighted Union Disjoint Set

Implement union-find with union by component size and no path compression.

Input starts with `elementCount operationCount`. Operations are `unite a b`,
`find x`, `connected a b`, `parent x`, `component_size x`, `components`, and
`snapshot`.

`unite` attaches the smaller root under the larger root and prints the
surviving root. On equal sizes, the root of `b` is attached under the root of
`a`. Already-connected elements print `ALREADY`. `snapshot` prints the raw
parent array. Invalid identifiers print `OUT_OF_RANGE`.

Do not use standard containers.
