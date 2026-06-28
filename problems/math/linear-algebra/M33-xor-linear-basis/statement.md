# XOR Linear Basis Queries

Build a linear basis over `GF(2)` for each list of non-negative integers.

For every query, report the rank, the maximum subset XOR, and whether the target
value can be represented as a subset XOR.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each query starts with:

`basis n target`

where `1 <= n <= 200`, `0 <= target < 2^60`. The next line contains `n` values,
each in `[0, 2^60)`.

## Output

Print:

`rank max_xor YES`

or

`rank max_xor NO`

for each query.

## Example

Input:

```
2
basis 3 3
1 2 3
basis 2 3
5 10
```

Output:

```
2 3 YES
2 15 NO
```

## Restrictions

- `std::pow` is forbidden.
- Dependent vectors must not increase the rank.
- The target `0` is always representable by the empty subset.
- To maximize XOR, greedily improve the current answer from high bits to low bits.
