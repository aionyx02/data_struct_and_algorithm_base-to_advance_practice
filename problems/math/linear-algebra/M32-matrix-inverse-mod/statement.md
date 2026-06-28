# Matrix Inverse Mod Prime

Compute the inverse of each matrix over `MOD = 1000000007`, or report that the
matrix is singular.

Apply Gauss-Jordan elimination to `[A | I]`. A query is invertible only when
every column gets a pivot.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`inverse n`

where `1 <= n <= 45`. The next `n` lines contain the matrix entries in
`[0, MOD)`.

## Output

For each query, print either `SINGULAR` or `n` lines containing `A^{-1}`. Print
matrix rows directly one after another; there is no blank line between queries.

## Example

Input:

```
2
inverse 2
1 2
3 4
inverse 2
1 2
2 4
```

Output:

```
1000000005 1
500000005 500000003
SINGULAR
```

## Restrictions

- `std::pow` is forbidden.
- Row swaps are required when the current pivot position is zero.
- Normalize the pivot row before eliminating other rows.
- A missing pivot means `SINGULAR`, not a partially inverted matrix.
