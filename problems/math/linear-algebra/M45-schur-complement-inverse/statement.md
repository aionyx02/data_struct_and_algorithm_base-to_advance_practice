# Schur Complement Block Inverse

For an invertible matrix `A`, print the inverse of each block matrix:

`[ A  B ; C  D ]^-1`

where `MOD = 1000000007`, `A` is `n x n`, `B` is `n x k`, `C` is `k x n`,
and `D` is `k x k`.

Let `S = D - C A^-1 B`. Every query is guaranteed to have an invertible `S`.
You may compute the inverse from the block formula or by solving one Schur
complement system per basis vector.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`blockinverse n k m`

where `1 <= n <= 80`, `1 <= k <= 8`, and `1 <= m <= 100`. The next `n` lines
contain the invertible matrix `A`. For each query, the next `n` lines contain
`B`, the next `k` lines contain `C`, and the next `k` lines contain `D`.

## Output

For each query, print `n + k` lines containing the inverse matrix rows modulo
`MOD`. Queries in the same batch are independent; always use the original
matrix `A`.

## Example

Input:

```
1
blockinverse 2 1 2
2 0
0 3
1
0
1 0
5
0
1
0 1
4
```

Output:

```
555555560 0 888888895
0 333333336 0
888888895 0 222222224
500000004 0 0
0 272727275 181818183
0 181818183 454545458
```

## Restrictions

- `std::pow` is forbidden.
- Use `S = D - C A^-1 B`; the sign matters.
- Do not transpose the rectangular product into `B^T A^-1 C^T`.
