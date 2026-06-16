# Persistent Segment Tree Versions

Maintain an array with versioned point updates and inclusive range-sum queries.
Version `0` is the initial array. Every successful update creates one new
version by copying only the nodes on the update path; all older versions must
remain queryable and unchanged.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `set baseVersion index value`: create a new version from `baseVersion` after
  replacing one value, then print the new version ID.
- `add baseVersion index delta`: create a new version from `baseVersion` after
  adding `delta` to one value, then print the new version ID.
- `get version index`: print one value from a version.
- `range version left right`: print the inclusive range sum from a version.
- `all version`: print the sum of the entire array in a version.
- `versions`: print the number of existing versions.
- `size`: print `n`.

Print `OUT_OF_RANGE` when a version ID is outside `[0, versions)`, an index is
outside `[0, n)`, or a range has invalid or reversed endpoints. Failed updates
do not create versions.

## Restrictions

- Use path copying. Do not mutate nodes that are reachable from older versions.
- Store one root per version.
- Use explicit array or node-pool storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- Every update and query must run in `O(log n)` time.
- If `V` versions exist, storage must be `O(n + V log n)`.
