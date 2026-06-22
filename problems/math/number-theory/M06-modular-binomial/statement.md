# Modular Inverse Tables and Binomials

Precompute the three tables every modular-combinatorics solution needs — linear
modular inverses, factorials, and inverse factorials — then answer inverse,
factorial, and binomial-coefficient queries modulo the prime `p = 1000000007`.
M-series, admitted by ADR-0006.

Let `N = 1000000` and `p = 1000000007`.

## Input

The first line contains an integer `Q` (`1 <= Q <= 40`). Each of the next `Q`
lines is one query:

- `inv x` — print `x^{-1} mod p` for `1 <= x <= N`.
- `fact n` — print `n! mod p` for `0 <= n <= N`.
- `inv_fact n` — print `(n!)^{-1} mod p` for `0 <= n <= N`.
- `binom n r` — print `C(n, r) mod p` for `0 <= n <= N`, `r >= 0`. If `r > n`,
  print `0`.

## Output

One line per query: the requested value in `[0, p)`.

## Example

Input:

```
4
inv 2
fact 5
binom 5 2
binom 5 5
```

Output:

```
500000004
120
10
1
```

`2 * 500000004 = 1 (mod p)`, `5! = 120`, `C(5, 2) = 10`, and `C(5, 5) = 1`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and the `<numeric>`
  header.
- Build the inverse table with the linear recurrence
  `inv[i] = -(p / i) * inv[p % i] mod p`, normalized into `[0, p)`.
- `C(n, n) = 1` and `C(n, 0) = 1`; only `r > n` yields `0`.
