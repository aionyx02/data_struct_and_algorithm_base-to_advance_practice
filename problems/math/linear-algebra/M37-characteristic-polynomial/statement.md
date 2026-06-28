# Characteristic Polynomial

For each matrix `A`, print the coefficients of its characteristic polynomial:

`det(xI - A) = c_n x^n + c_{n-1} x^{n-1} + ... + c_0`

All arithmetic is modulo `MOD = 1000000007`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`char n`

where `1 <= n <= 80`. The next `n` lines contain the matrix `A`.

## Output

For each query, print `n + 1` coefficients from highest degree to constant term:

`c_n c_{n-1} ... c_0`

## Example

Input:

```
2
char 1
7
char 2
1 2
3 4
```

Output:

```
1 1000000000
1 1000000002 1000000005
```

## Restrictions

- `std::pow` is forbidden.
- The polynomial is `det(xI - A)`, not `det(A - xI)`.
- Print coefficients from highest degree to constant term.
