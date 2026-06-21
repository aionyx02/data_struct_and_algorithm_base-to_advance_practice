# Potential Weighted Disjoint Set

Each element `x` has an unknown integer potential. Relations fix the difference
between two potentials. Maintain a weighted disjoint-set forest that stores, for
every node, its potential relative to its parent, and answers difference queries
within a component.

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `relate a b w`: assert that `potential(b) - potential(a) = w`.
  - If `a` and `b` are in different components, merge them so the assertion
    holds (attach the root of `b` under the root of `a`) and print `MERGED`.
  - If they are already in the same component and the known difference equals
    `w`, print `CONSISTENT`.
  - If they are already in the same component but the known difference is not
    `w`, print `CONFLICT` and ignore the assertion.
- `diff a b`: if `a` and `b` are in the same component, print
  `potential(b) - potential(a)`; otherwise print `UNKNOWN`.
- `connected a b`: print `true` or `false`.
- `find x`: print the root of `x` after compressing its path.
- `components`: print the current number of disjoint sets.
- `snapshot`: print the raw parent array as space-separated values, or `EMPTY`
  when `elementCount` is `0`.

Any operation whose element identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`. The relation weight `w` can be negative.

## Restrictions

- Store each node's potential relative to its parent; do not store absolute
  potentials, which are undetermined.
- `find` must accumulate weights while compressing so that a query after
  compression reports the same difference as before.
- A merge must account for the current relative potentials of `a` and `b`, not
  just the raw weight `w`.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
