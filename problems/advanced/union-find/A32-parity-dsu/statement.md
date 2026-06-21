# Parity Disjoint Set

Every element has one of two colors. Relations state whether two elements share
a color or differ. Maintain a disjoint-set forest that stores each node's parity
relative to its parent and reports the relative color of any two connected
elements, detecting contradictions.

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `relate a b same` or `relate a b diff`: assert that `a` and `b` have the same
  color (`same`) or different colors (`diff`).
  - If `a` and `b` are in different components, merge them so the assertion
    holds (attach the root of `b` under the root of `a`) and print `MERGED`.
  - If they are already connected and the assertion agrees with the known
    parity, print `CONSISTENT`.
  - If they are already connected but the assertion contradicts the known
    parity, print `CONFLICT` and ignore the assertion.
- `query a b`: if `a` and `b` are connected, print `same` or `diff`; otherwise
  print `unknown`.
- `connected a b`: print `true` or `false`.
- `find x`: print the root of `x` after compressing its path.
- `components`: print the current number of disjoint sets.
- `snapshot`: print the raw parent array as space-separated values, or `EMPTY`
  when `elementCount` is `0`.

Any operation whose element identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`.

## Restrictions

- Store the parity of each node relative to its parent as a single bit.
- `find` must xor the parities while compressing so a query after compression
  reports the same relation as before.
- A merge must combine the relative parities of `a` and `b` with the requested
  relation; do not assume `a` and `b` are their own roots.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
