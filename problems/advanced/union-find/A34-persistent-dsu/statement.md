# Persistent Disjoint Set

Maintain a versioned disjoint-set forest. Version `0` is the initial state where
every element is its own set. Each successful union produces a new version while
leaving every earlier version queryable and unchanged.

## Input

The first line contains `elementCount operationCount`. Version identifiers start
at `0`; a new version id is the next integer each time one is created. Each
following line is one operation:

- `unite v a b`: starting from version `v`, union the sets of `a` and `b` by
  size (no compression). If `a` and `b` are already connected in version `v`,
  print `ALREADY` and create no new version. Otherwise create a new version that
  copies version `v` and applies the union, then print the new version id.
- `connected v a b`: print `true` or `false` for version `v`.
- `find v x`: print the root of `x` in version `v`.
- `component_size v x`: print the size of the component containing `x` in
  version `v`.
- `components v`: print the number of disjoint sets in version `v`.
- `snapshot v`: print version `v`'s raw parent array as space-separated values,
  or `EMPTY` when `elementCount` is `0`.

Print `OUT_OF_RANGE` when the version does not exist yet, or when an element
identifier is outside `[0, elementCount)`.

## Restrictions

- A new version must not modify any earlier version; querying an old version
  after later unions must return the old result.
- Always fork from the requested version `v`, not from the most recently created
  version.
- Union is by size and never compresses paths.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
