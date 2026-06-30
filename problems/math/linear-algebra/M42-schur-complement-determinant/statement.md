# Schur Complement Determinant

For an invertible matrix `A`, answer independent block determinant queries:

`det([ A  B ; C  D ]) mod MOD`

where `MOD = 1000000007`, `A` is `n x n`, `B` is `n x k`, `C` is `k x n`,
and `D` is `k x k`.

The Schur complement identity gives:

`det([ A  B ; C  D ]) = det(A) * det(D - C A^-1 B)`

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`schur n k m`

where `1 <= n <= 80`, `1 <= k <= 8`, and `1 <= m <= 100`. The next `n` lines
contain the invertible matrix `A`. For each query, the next `n` lines contain
`B` with `k` values per row, the next `k` lines contain `C` with `n` values per
row, and the next `k` lines contain `D`.

## Output

Print one line for each query. Queries in the same batch are independent; always
use the original matrix `A`.

## Example

Input:

```
1
schur 2 1 2
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
27
22
```

## Restrictions

- `std::pow` is forbidden.
- Use `D - C A^-1 B`; the sign is part of the identity.
- Do not transpose the rectangular product into `B^T A^-1 C^T`.
