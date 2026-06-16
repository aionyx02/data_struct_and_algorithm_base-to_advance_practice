# Block Decomposition With Lazy Tags

Maintain an array under range add updates and range sum queries with block
decomposition and lazy tags. Partition the array into blocks of size about
`sqrt(n)`. Each block keeps a cached sum and a lazy add tag that applies to every
element of the block but has not yet been written into the individual cells.

A range add splits into at most two partially covered boundary blocks, whose
cells are updated directly, and the whole blocks in between, which only bump
their lazy tag and cached sum. A range sum reads the partial boundary cells
(adding their block's lazy tag) and the cached sums of the whole blocks between,
all in `O(sqrt n)`.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `add left right delta`: add `delta` to every value in the inclusive range.
- `sum left right`: print the sum over the inclusive range.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
reversed endpoints (`left > right`). Successful updates print nothing.

## Restrictions

- Partition the array into blocks of size about `sqrt(n)` with a cached sum and
  a lazy add tag per block.
- A range add must update the partial boundary blocks cell by cell and apply a
  lazy tag plus a cached-sum adjustment to each whole interior block.
- A range sum and a `get` must add the relevant block's lazy tag when reading
  individual cells, and use cached block sums for whole blocks.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Range updates and range queries run in `O(sqrt n)`; storage is `O(n)`.
