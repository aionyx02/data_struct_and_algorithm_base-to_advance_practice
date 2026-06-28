# Matrix Determinant Lemma Queries

For an invertible matrix `A`, answer independent rank-one update queries:

`det(A + u v^T) mod MOD`

where `MOD = 1000000007`.

The matrix determinant lemma gives:

`det(A + u v^T) = det(A) * (1 + v^T A^{-1} u)`

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each batch starts with:

`lemma n m`

where `1 <= n <= 80` and `1 <= m <= 200`. The next `n` lines contain an
invertible matrix `A`. For each of the `m` update queries, one line contains
the column vector `u`, and the next line contains the row vector `v`.

## Output

Print one line for each rank-one update query. Updates in the same batch are
independent; always use the original matrix `A`.

## Example

Input:

```
1
lemma 2 2
2 0
0 3
1 0
1 0
0 1
0 1
```

Output:

```
9
8
```

## Restrictions

- `std::pow` is forbidden.
- Use `v^T A^{-1} u`; reversing the two vectors is wrong for non-symmetric
  matrices.
- Do not apply updates cumulatively within a batch.
