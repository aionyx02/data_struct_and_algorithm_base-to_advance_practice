# Woodbury Low-Rank Determinant

For an invertible matrix `A`, answer independent low-rank determinant queries:

`det(A + U V^T) mod MOD`

where `MOD = 1000000007`, `U` is an `n x k` matrix, and `V` is an `n x k`
matrix.

Sylvester's determinant identity gives the Woodbury-style reduction:

`det(A + U V^T) = det(A) * det(I_k + V^T A^-1 U)`

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`woodbury n k m`

where `1 <= n <= 80`, `1 <= k <= 8`, and `1 <= m <= 100`. The next `n` lines
contain the invertible base matrix `A`. For each query, the next `n` lines
contain `U` with `k` values per row, followed by the next `n` lines containing
`V` with `k` values per row.

## Output

Print one line for each query: `det(A + U V^T) mod MOD`. Queries in the same
batch are independent; always use the original matrix `A`.

## Example

Input:

```
1
woodbury 2 1 2
2 0
0 3
1
0
1
0
0
1
0
1
```

Output:

```
9
8
```

## Restrictions

- `std::pow` is forbidden.
- Use `V^T A^-1 U`; `U^T A^-1 V` is wrong for non-symmetric matrices.
- Compute the determinant of the full `k x k` core; multiplying only its
  diagonal misses cross-column interactions.
