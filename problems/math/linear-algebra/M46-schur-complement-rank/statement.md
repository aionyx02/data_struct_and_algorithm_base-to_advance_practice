# Schur Complement Block Rank

For an invertible matrix `A`, compute the rank of each block matrix:

`[ A  B ; C  D ]`

where `MOD = 1000000007`, `A` is `n x n`, `B` is `n x k`, `C` is `k x n`,
and `D` is `k x k`.

Let `S = D - C A^-1 B`. Because `A` is invertible,
`rank([ A B ; C D ]) = n + rank(S)`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`blockrank n k m`

where `1 <= n <= 80`, `1 <= k <= 80`, and `1 <= m <= 100`. The next `n`
lines contain the invertible matrix `A`. For each query, the next `n` lines
contain `B`, the next `k` lines contain `C`, and the next `k` lines contain
`D`.

## Output

For each query, print one line with the rank of the full `(n + k) x (n + k)`
block matrix. Queries in the same batch are independent; always use the
original matrix `A`.

## Example

Input:

```
3
blockrank 1 1 2
1
2
3
6
2
3
7
blockrank 2 1 1
1 2
0 1
1
3
4 5
1000000002
blockrank 2 2 1
1 0
0 1
1 0
0 1
1 2
3 4
1 2
3 4
```

Output:

```
1
2
2
2
```

## Restrictions

- `std::pow` is forbidden.
- Use `S = D - C A^-1 B`; the sign matters.
- Do not transpose the rectangular product into `B^T A^-1 C^T`.
