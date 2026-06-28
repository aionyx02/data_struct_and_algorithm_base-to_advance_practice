# Modular Matrix Determinant

Compute the determinant of each square matrix modulo `MOD = 1000000007`.

Use Gaussian elimination over the finite field. When a pivot row is swapped into
place, the determinant sign changes; when a column has no pivot, the determinant
is zero.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each query starts with:

`det n`

where `1 <= n <= 80`. The next `n` lines contain the matrix entries in
`[0, MOD)`.

## Output

Print one determinant modulo `MOD` per query.

## Example

Input:

```
3
det 2
1 2
3 4
det 2
0 2
3 4
det 3
1 2 3
2 4 6
7 8 9
```

Output:

```
1000000005
1000000001
0
```

## Restrictions

- `std::pow` is forbidden; modular inverses must use integer fast power.
- Do not divide with integer `/`; multiply by the modular inverse of the pivot.
- Swapping rows changes the determinant sign.
- A missing pivot makes the determinant zero.
