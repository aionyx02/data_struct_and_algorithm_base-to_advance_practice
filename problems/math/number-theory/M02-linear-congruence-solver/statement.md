# Linear Congruence Solver

Apply the extended Euclidean algorithm to two classic tasks: solving a single
linear congruence `a*x = b (mod m)`, and combining two congruences with the
**general** Chinese Remainder Theorem (moduli need not be coprime). M-series,
admitted by ADR-0006.

## Input

The first line contains an integer `N` (`1 <= N <= 40`). Each of the next `N`
lines is one operation:

- `solve a b m` — solve `a*x = b (mod m)` for `x`. Let `g = gcd(a, m)`. If
  `g` does not divide `b`, print `NO SOLUTION`. Otherwise there are exactly `g`
  solutions modulo `m`; print two integers: the solution count `g` and the
  smallest non-negative solution.
- `crt r1 m1 r2 m2` — find the smallest non-negative `x` with `x = r1 (mod m1)`
  and `x = r2 (mod m2)`. If `gcd(m1, m2)` does not divide `r2 - r1`, print
  `NO SOLUTION`. Otherwise print `x` and the combined modulus `lcm(m1, m2)`.

Constraints: `0 <= a, b, r1, r2 <= 10^9` and `1 <= m, m1, m2 <= 10^9`. The
combined modulus can reach `10^18`, so accumulate products with a 128-bit type.

## Output

One line per operation: `NO SOLUTION`, or two space-separated non-negative
integers as described above.

## Example

Input:

```
3
solve 6 4 10
solve 2 1 4
crt 2 3 3 5
```

Output:

```
2 4
NO SOLUTION
8 15
```

`6*x = 4 (mod 10)` has `gcd(6, 10) = 2` solutions, the smallest being `4`
(`6*4 = 24 = 4 (mod 10)`). `2*x = 1 (mod 4)` is unsolvable because `2` is even.
`x = 8` is the unique value modulo `15` that is `2 (mod 3)` and `3 (mod 5)`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::lcm`, `std::__gcd`, and the `<numeric>`
  header. Implement the extended Euclidean algorithm yourself.
- Report the exact solution count for `solve`; do not assume a unique solution.
- Handle non-coprime moduli in `crt`; do not assume `gcd(m1, m2) = 1`.
- Normalize every printed value into `[0, modulus)`.
