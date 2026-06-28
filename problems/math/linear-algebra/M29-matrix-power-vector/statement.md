# Matrix Power Vector Application

Compute `A^e * v` modulo `MOD = 1000000007` for each query.

The exponent can be very large, so repeated multiplication by `A` is not
enough. Use binary exponentiation on square matrices and keep the vector as a
column vector.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each query starts with:

`apply n e`

where `1 <= n <= 25` and `0 <= e <= 10^18`. The next `n` lines contain the
matrix `A`, followed by one line containing vector `v`.

## Output

For each query, print the `n` values of `A^e * v`.

## Example

Input:

```
2
apply 2 5
1 1
1 0
1 0
apply 2 0
2 3
4 5
7 11
```

Output:

```
8 5
7 11
```

## Restrictions

- `std::pow` is forbidden.
- `e = 0` must return the original vector.
- Matrix multiplication is not commutative; preserve multiplication order.
- Treat `v` as a column vector, so the answer row is `sum A[row][col] * v[col]`.
