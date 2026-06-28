# Linear Sieve of Multiplicative Functions

Run **one** linear (Euler) sieve over `[1, 1000000]` that simultaneously
computes four multiplicative functions, then answer queries in constant time.
The whole point is that a single sieve, carrying the exponent of each number's
smallest prime factor, yields all four functions together. M-series, admitted
by ADR-0006.

Let `LIMIT = 1000000`.

## Functions

For `n` with prime factorisation `n = p1^a1 ... pk^ak`:

- `phi(n)` — Euler's totient. `phi(1) = 1`; in the sieve, if `p | i` then
  `phi(i*p) = phi(i) * p`, otherwise `phi(i*p) = phi(i) * (p - 1)`.
- `mu(n)` — Möbius function (`0` if not squarefree, else `(-1)^k`).
- `d(n)` — number of divisors, `d(n) = prod (ai + 1)`.
- `sigma(n)` — sum of divisors, `sigma(n) = prod (p_i^{ai+1} - 1) / (p_i - 1)`.

## Input

The first line contains an integer `N` (`1 <= N <= 40`). Each of the next `N`
lines is one query of the form `op x` with `1 <= x <= LIMIT`:

- `phi x` — print `phi(x)`.
- `mu x` — print `mu(x)`.
- `d x` — print the number of divisors of `x`.
- `sigma x` — print the sum of divisors of `x`.

## Output

One line per query: a single integer.

## Example

Input:

```
6
phi 1
phi 4
mu 30
d 4
sigma 12
sigma 1
```

Output:

```
1
2
-1
3
28
1
```

`phi(4) = 2` (the integers `1, 3` are coprime to `4`); `mu(30) = -1`; `4` has
divisors `1, 2, 4` so `d(4) = 3`; `sigma(12) = 1+2+3+4+6+12 = 28`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- Use a single linear sieve; the `p | i` branch must update `d` and `sigma`
  through the smallest-prime-power bookkeeping, not as if `p` were new.
