# Disjoint Set Amortized Cost

Maintain a disjoint-set forest over elements `0` through `elementCount - 1` and
make the cost of `find` observable. Unions are by rank and compress paths, so
the work spent walking to a root falls sharply once a path has been flattened.

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `unite a b`: union the sets of `a` and `b` by rank (attach the lower-rank
  root under the higher-rank root; on a tie keep the root of `a` and bump its
  rank). The two root lookups compress their paths. Print the surviving root,
  or `ALREADY` if `a` and `b` already share a root.
- `find x`: print the number of edges walked from `x` up to its root, measured
  **before** compression, then compress every node on that path directly onto
  the root. A node that is its own root costs `0`; a direct child costs `1`.
- `connected a b`: print `true` or `false`; the two root lookups compress.
- `parent x`: print the raw stored parent of `x` (no compression).
- `components`: print the current number of disjoint sets.
- `snapshot`: print the raw parent array as space-separated values, or `EMPTY`
  when `elementCount` is `0`.

Any operation whose element identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`.

## Restrictions

- `find` must report the edge count, not the node count: a direct child of the
  root costs `1`, not `2`.
- `find` must actually compress: querying the same node twice in a row must
  report a strictly cheaper (or equal, when already flat) cost the second time.
- The root lookups performed inside `unite` and `connected` also compress.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
