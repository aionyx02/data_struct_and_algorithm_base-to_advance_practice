# Schur Complement Block Solve

For an invertible matrix `A`, solve independent block linear systems:

`[ A  B ; C  D ] [ x ; y ] = [ p ; q ]`

where `MOD = 1000000007`, `A` is `n x n`, `B` is `n x k`, `C` is `k x n`,
and `D` is `k x k`.

Let `S = D - C A^-1 B`. Then:

`y = S^-1 (q - C A^-1 p)`

`x = A^-1 (p - B y)`

Every query is guaranteed to have an invertible Schur complement `S`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`blocksolve n k m`

where `1 <= n <= 80`, `1 <= k <= 8`, and `1 <= m <= 100`. The next `n` lines
contain the invertible matrix `A`. For each query, the next `n` lines contain
`B`, the next `k` lines contain `C`, the next `k` lines contain `D`, then one
line contains `p` and one line contains `q`.

## Output

Print one line per query containing the `n + k` entries `x` followed by `y`.
Queries in the same batch are independent; always use the original matrix `A`.

## Example

Input:

```
1
blocksolve 2 1 2
2 0
0 3
1
0
1 0
5
5 6
7
0
1
0 1
4
4 9
8
```

Output:

```
2 2 1
2 909090918 272727276
```

## Restrictions

- `std::pow` is forbidden.
- Use `D - C A^-1 B` and `q - C A^-1 p`; the signs matter.
- Do not transpose the rectangular product into `B^T A^-1 C^T`.
