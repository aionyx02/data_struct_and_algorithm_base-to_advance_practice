# Woodbury Low-Rank Solve

For an invertible matrix `A`, answer independent low-rank updated linear-system
queries:

`(A + U V^T) x = b`

where `MOD = 1000000007`, `U` is an `n x k` matrix, and `V` is an `n x k`
matrix.

The Woodbury identity gives:

`(A + U V^T)^-1 b = A^-1 b - A^-1 U (I_k + V^T A^-1 U)^-1 V^T A^-1 b`

Every query is guaranteed to have an invertible `I_k + V^T A^-1 U`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`woodsolve n k m`

where `1 <= n <= 80`, `1 <= k <= 8`, and `1 <= m <= 100`. The next `n` lines
contain the invertible base matrix `A`. For each query, the next `n` lines
contain `U` with `k` values per row, the next `n` lines contain `V` with `k`
values per row, and the final line contains `b`.

## Output

Print one line per query containing the `n` entries of `x` modulo `MOD`.
Queries in the same batch are independent; always use the original matrix `A`.

## Example

Input:

```
1
woodsolve 2 1 2
2 0
0 3
1
0
1
0
5 6
0
1
0
1
4 9
```

Output:

```
666666673 2
2 250000004
```

## Restrictions

- `std::pow` is forbidden.
- Use the full inverse of `I_k + V^T A^-1 U`; using only its diagonal is wrong.
- Do not swap the low-rank factors into `U^T A^-1 V`.
