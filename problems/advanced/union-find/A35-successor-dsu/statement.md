# Successor Disjoint Set

Slots `0` through `elementCount - 1` start free. Use a disjoint-set forest whose
parent pointers point toward the next free slot to assign slots and answer
successor queries in near-constant amortized time. Treat index `elementCount` as
a sentinel meaning "no free slot".

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `assign x`: find the smallest free slot at or after `x`, mark it used, and
  print that slot. If there is no free slot at or after `x`, print `NONE`.
- `next x`: print the smallest free slot at or after `x` without changing
  anything, or `NONE` if none exists.
- `used x`: print `1` if slot `x` is already used, otherwise `0`.
- `remaining`: print how many slots are still free.
- `snapshot`: print the raw parent array for slots `[0, elementCount)` as
  space-separated values, or `EMPTY` when `elementCount` is `0`.

Any operation whose slot identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`.

## Restrictions

- `assign x` must mark the *found successor* slot as used, not slot `x` itself;
  assigning the same starting point repeatedly must hand out successive slots.
- The slot at index `elementCount - 1` is assignable; only the sentinel index
  `elementCount` means "no free slot".
- `find` should compress paths so repeated successor queries do not rescan the
  whole run of used slots.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`, or
  `std::list`.
