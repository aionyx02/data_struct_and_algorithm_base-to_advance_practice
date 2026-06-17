# Wavelet Matrix Range Kth

Answer order-statistic queries over a static array with a Wavelet Matrix. First
compress the values. Then process the compressed ranks from the most significant
bit to the least significant bit. At each level, keep a prefix count of one-bits,
stable-partition zero-bit ranks before one-bit ranks, and remember the number of
zero-bit ranks at that level.

The array never changes, so there are no update operations.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `kth left right k`: print the 1-based `k`-th smallest value in the inclusive
  range.
- `count_lt left right value`: print how many values in the inclusive range are
  strictly less than `value`.
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

- Compress values and build a Wavelet Matrix over the compressed ranks.
- Every level must store prefix one-bit counts and a zero-count offset for the
  level's stable partition.
- When `kth` descends into the one-bit side, translate the range by adding that
  level's zero-count offset and subtract the zero-side count from `k`.
- `count_lt`, `count_le`, and `freq` must handle duplicate values and values
  outside the compressed domain.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- Build time and space are `O(n log sigma)`, where `sigma` is the number of
  distinct values; every query runs in `O(log sigma)`.
