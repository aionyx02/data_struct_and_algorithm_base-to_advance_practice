# Square Root Decomposition Range Sum

Maintain an array under point updates and range sum queries with Square Root
Decomposition. Partition the array into contiguous blocks of size about
`sqrt(n)` and cache each block's sum. A point update refreshes one element and
its block sum in `O(1)`. A range sum scans the partial cells of the two boundary
blocks directly and adds the cached sums of the whole blocks in between, for
`O(sqrt n)` per query.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set index value`: replace one value.
- `add index delta`: add `delta` to one value.
- `sum left right`: print the sum over the inclusive range.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
reversed endpoints (`left > right`). Successful updates print nothing.

## Restrictions

- Partition the array into blocks of size about `sqrt(n)` and keep a cached sum
  per block.
- Every `set` and `add` must refresh the affected block sum so later queries see
  the change.
- A range sum must read the two partial boundary blocks element by element and
  use cached block sums only for the whole blocks strictly between them; a
  query whose endpoints fall in the same block reads that block directly.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Updates run in `O(1)`, queries in `O(sqrt n)`, and storage is `O(n)`.
