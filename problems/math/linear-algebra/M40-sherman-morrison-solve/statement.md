# Sherman-Morrison Rank-One Solve

For an invertible matrix `A`, answer independent linear-system queries after a
rank-one update:

`(A + u v^T) x = b`

All arithmetic is modulo `MOD = 1000000007`.

The Sherman-Morrison identity gives:

`(A + u v^T)^-1 b = A^-1 b - A^-1 u * (v^T A^-1 b) / (1 + v^T A^-1 u)`

Every query is guaranteed to have `1 + v^T A^-1 u != 0`, so the updated matrix
is invertible.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`solve n m`

where `1 <= n <= 80` and `1 <= m <= 200`. The next `n` lines contain an
invertible matrix `A`. For each query, one line contains the column vector `u`,
the next line contains the row vector `v`, and the next line contains `b`.

## Output

Print one line per query containing the `n` entries of `x` modulo `MOD`.
Queries in the same batch are independent; always use the original matrix `A`.

## Example

Input:

```
1
solve 2 2
2 0
0 3
1 0
1 0
5 6
0 1
0 1
4 9
```

Output:

```
666666673 2
2 250000004
```

## Restrictions

- `std::pow` is forbidden.
- Use the denominator `1 + v^T A^-1 u`; omitting it is wrong.
- Use `A^-1 u` and `v^T A^-1 b`; swapping `u` and `v` is wrong for
  non-symmetric matrices.
