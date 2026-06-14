# Quadratic Probing Hash Table

Implement a fixed-capacity integer set using probe position
`(hash(key) + i * i) mod capacity` for `i = 0..capacity-1`. Deletion is not
part of this exercise.

The operations and outputs are the same as `F53-linear-probing-hash-table`:
`insert`, `contains`, `slot`, `probe_count`, `table`, `size`, `capacity`, and
`clear`. An insertion prints `FULL` when its complete quadratic probe sequence
does not reach an empty slot, even if another table slot is empty.

Do not use standard containers.
