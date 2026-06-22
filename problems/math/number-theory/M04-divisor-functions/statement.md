# Factorization and Divisor Functions

Factorize an integer by trial division and report the three classic
multiplicative functions built from its factorization: the number of divisors
`d(n)`, the sum of divisors `sigma(n)`, and Euler's totient `phi(n)`. M-series,
admitted by ADR-0006.

## Input

The first line contains an integer `Q` (`1 <= Q <= 40`). Each of the next `Q`
lines is one query with `1 <= n <= 10^12`:

- `factorize n` — print the prime factorization as space-separated `p^e` terms
  in ascending prime order, e.g. `2^2 3^1`. For `n = 1` print `1`.
- `divisors n` — print `d(n)`, the number of positive divisors.
- `sigma n` — print `sigma(n)`, the sum of positive divisors.
- `phi n` — print `phi(n)`, the count of integers in `[1, n]` coprime to `n`.

`sigma(n)` fits in a signed 64-bit integer for all `n` in range.

## Output

One line per query: the factorization string, or the requested integer value.

## Example

Input:

```
4
factorize 12
divisors 12
sigma 12
phi 12
```

Output:

```
2^2 3^1
6
28
4
```

`12 = 2^2 * 3`, which has `6` divisors summing to `28`, and `phi(12) = 4`
(the integers `1, 5, 7, 11`).

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- For a prime power `p^e`, remember `d` multiplies `(e + 1)` and `phi`
  multiplies `p^(e-1) * (p - 1)`; do not drop the exponent.
- Print factorization terms in ascending prime order with explicit exponents,
  even when the exponent is `1`.
