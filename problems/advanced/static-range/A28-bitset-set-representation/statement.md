# Bitset Set Representation

Represent a fixed-universe integer set with packed machine words. Key `i` lives
in word `i / 64` and bit `i % 64`. Point updates change one bit, membership
queries read one bit, and range counts use masks on the two boundary words plus
whole-word popcounts in between.

The universe size never changes.

## Input

The first line contains `universeSize` and `operationCount`. The second line
contains `universeSize` integers, each `0` or `1`, describing the initial set
membership for keys `[0, universeSize)`.

Each following line is one operation:

- `set key`: insert `key`.
- `clear key`: remove `key`.
- `flip key`: toggle `key`.
- `contains key`: print `1` if `key` is present, otherwise `0`.
- `count left right`: print how many present keys lie in the inclusive range.
- `first_ge key`: print the smallest present key greater than or equal to
  `key`, or `NONE` if no such key exists.
- `total`: print the current set size.
- `size`: print `universeSize`.

Print `OUT_OF_RANGE` when a key is outside `[0, universeSize)`, when a range has
an endpoint outside `[0, universeSize)`, or when a range has reversed endpoints
(`left > right`). Successful updates print nothing.

## Restrictions

- Store membership in explicit 64-bit words, not one integer per key.
- Range counts must mask the partial left and right boundary words so bits
  outside the requested range are not counted.
- Maintain any cached total count consistently across `set`, `clear`, and
  `flip`.
- `first_ge` must ignore set bits before the requested key in the first scanned
  word and must never return a key outside the universe.
- Do not use `std::bitset`, `std::vector`, `std::map`, `std::unordered_map`,
  `std::set`, `std::multiset`, or `std::list`.
