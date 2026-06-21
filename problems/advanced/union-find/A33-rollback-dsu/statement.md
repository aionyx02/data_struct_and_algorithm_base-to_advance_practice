# Rollback Disjoint Set

Maintain a disjoint-set forest that supports undoing unions back to a saved
checkpoint. Because rollback must restore the exact previous structure, unions
are by size and paths are **never** compressed: path compression would rewrite
pointers that the undo log does not record.

## Input

The first line contains `elementCount operationCount`. Each following line is
one operation:

- `unite a b`: union by size without compression (attach the smaller root under
  the larger; on a tie keep the root of `a`). Record an undo entry. Print the
  surviving root, or `ALREADY` if `a` and `b` already share a root.
- `find x`: print the root of `x` (no compression).
- `connected a b`: print `true` or `false`.
- `component_size x`: print the size of the component containing `x`.
- `components`: print the current number of disjoint sets.
- `checkpoint`: save the current state and print the number of open checkpoints
  after saving.
- `rollback`: undo every union performed since the most recent checkpoint, drop
  that checkpoint, and print how many unions were undone. If there is no open
  checkpoint, print `NONE`.
- `snapshot`: print the raw parent array as space-separated values, or `EMPTY`
  when `elementCount` is `0`.

Any operation whose element identifier is outside `[0, elementCount)` prints
`OUT_OF_RANGE`.

## Restrictions

- Do not compress paths anywhere; rollback depends on the structure being
  reconstructible purely from the union undo log.
- `rollback` must undo all unions since the last checkpoint, not just the most
  recent one, and must restore both parents and component sizes.
- Checkpoints nest: a `rollback` returns to the latest checkpoint and removes
  it, exposing any earlier checkpoint for the next `rollback`.
- Do not use `std::map`, `std::unordered_map`, `std::set`, `std::vector`,
  `std::list`, or `std::stack`.
