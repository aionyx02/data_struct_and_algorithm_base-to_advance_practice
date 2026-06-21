# Union By Rank With Path Compression

Maintain a disjoint-set forest over elements `0` through `elementCount - 1`.
Initially every element is its own root with rank `0`. Unions are decided by
**rank**, and every `find` applies full path compression. Rank is what drives
the union; it is not the component size.

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `unite a b`: union the sets of `a` and `b`. Attach the lower-rank root under
  the higher-rank root. When the two roots have equal rank, keep the root of
  `a` and increase its rank by one. Print the surviving root. If `a` and `b`
  are already in the same set, print `ALREADY`.
- `find x`: print the root of `x` after compressing its path.
- `connected a b`: print `true` if `a` and `b` share a root, otherwise `false`.
- `parent x`: print the raw stored parent of `x` (this query does not compress).
- `rank x`: print the rank stored at the root of `x`.
- `components`: print the current number of disjoint sets.
- `snapshot`: print the raw parent array as space-separated values, or `EMPTY`
  when `elementCount` is `0`.

Any operation whose element identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`. Successful unions print the surviving root; there is no other
silent output.

## Restrictions

- Decide unions by rank, not by component size. Equal-rank unions must keep the
  root of the left operand and bump its rank.
- Every `find` (including the lookups performed inside `unite` and `connected`)
  must apply full path compression, so later `parent` and `snapshot` queries
  observe the flattened structure.
- `rank x` reports the rank of the root, which only increases on equal-rank
  unions and is independent of the number of elements in the set.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
