# Disjoint Sparse Table Range Sum

Answer range sum queries over a static array in `O(1)` with a Disjoint Sparse
Table. A plain Sparse Table only works for idempotent operations because its two
covering blocks overlap; sum is **not** idempotent, so the overlap would be
double-counted.

A Disjoint Sparse Table avoids this. At level `h` the array is split into
segments of length `2^(h+1)`; inside each segment it stores, for every position,
the aggregate from that position to the segment midpoint (a suffix on the left
half, a prefix on the right half). A query `[left, right]` with `left < right`
uses `h = highestBit(left XOR right)`: the segment at that level splits exactly
between `left` and `right`, so `table[h][left]` and `table[h][right]` cover two
**disjoint** halves whose combination is exact. A single-element query returns
the element itself.

The array never changes, so there are no update operations.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `sum left right`: print the sum over the inclusive range.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
reversed endpoints (`left > right`).

## Restrictions

- Build a Disjoint Sparse Table whose two covering halves are disjoint, and
  answer each `sum` query in `O(1)`.
- Handle the single-element query (`left == right`) without doubling the value.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, or `std::list`.
- The table build is `O(n log n)` time and space; every query is `O(1)`.
