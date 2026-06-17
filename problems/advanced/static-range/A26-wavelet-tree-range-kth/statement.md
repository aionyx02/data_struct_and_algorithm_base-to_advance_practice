# Wavelet Tree Range Kth

Answer order-statistic queries over a static array with a Wavelet Tree. The
structure recursively partitions values by the middle of the compressed value
domain. Each node stores prefix counts for how many elements of its sequence
move into the left child, which lets a range be translated into the child
sequence in `O(1)` at each level.

The array never changes, so there are no update operations.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `kth left right k`: print the 1-based `k`-th smallest value in the inclusive
  range.
- `count_le left right value`: print how many values in the inclusive range are
  less than or equal to `value`.
- `freq left right value`: print how many values in the inclusive range are
  exactly equal to `value`.
- `get index`: print one value.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, when a range has
reversed endpoints (`left > right`), or when `k` is outside
`[1, right - left + 1]`.

## Restrictions

- Compress the values and build a Wavelet Tree over the compressed value domain.
- Each node must store prefix counts that translate an inclusive range into the
  left or right child sequence.
- When descending right for `kth`, subtract the number of left-child values in
  the current range from `k`.
- `count_le` and `freq` must handle duplicate values and values outside the
  compressed domain.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- Build time and space are `O(n log sigma)`, where `sigma` is the number of
  distinct values; every query runs in `O(log sigma)`.
