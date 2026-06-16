# Sparse Table Range Query

Answer range minimum and range maximum queries over a static array. Build a
Sparse Table in `O(n log n)`: level `k` stores the answer for every block of
length `2^k`. Because minimum and maximum are idempotent, each query is answered
in `O(1)` by combining the two (possibly overlapping) blocks of length `2^k`
that exactly cover the range, where `k = floor(log2(right - left + 1))`.

The array never changes, so there are no update operations.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `min left right`: print the minimum over the inclusive range.
- `max left right`: print the maximum over the inclusive range.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
reversed endpoints (`left > right`).

## Restrictions

- Build a Sparse Table and answer each `min`/`max` query in `O(1)` using two
  overlapping power-of-two blocks.
- The two blocks must be `[left, left + 2^k - 1]` and
  `[right - 2^k + 1, right]`; both must stay inside the queried range.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- The table build is `O(n log n)` time and space; every query is `O(1)`.
