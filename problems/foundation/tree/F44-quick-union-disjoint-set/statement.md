# Quick Union Disjoint Set

Implement quick-union over elements `0` through `elementCount - 1`. Initially
every element is its own parent.

Input starts with `elementCount operationCount`. Operations are `unite a b`,
`find x`, `connected a b`, `parent x`, `component_size x`, `components`, and
`snapshot`.

`unite a b` attaches the root of `b` under the root of `a` and prints the
surviving root. If already connected it prints `ALREADY`. `snapshot` prints
the raw parent array. Invalid identifiers print `OUT_OF_RANGE`.

Do not use standard containers and do not apply weighting or path compression.
