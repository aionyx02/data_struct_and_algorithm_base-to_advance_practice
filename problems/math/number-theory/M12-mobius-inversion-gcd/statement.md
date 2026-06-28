# Mobius Inversion: Coprime and GCD Sum

Use Möbius inversion together with **2D divisor blocking** to evaluate two
classic double sums modulo `1000000007`. Both reduce to a sum over a single
index `d` whose `floor(n/d)` and `floor(m/d)` are piecewise constant, so each
query runs in `O(sqrt n + sqrt m)`. M-series, admitted by ADR-0006.

Let `LIMIT = 1000000` and `MOD = 1000000007`.

## Identities

- Coprime pair count:
  `coprime(n, m) = sum_{i=1}^{n} sum_{j=1}^{m} [gcd(i, j) = 1]
                 = sum_{d=1}^{min(n,m)} mu(d) * floor(n/d) * floor(m/d)`.
- GCD sum (via `gcd = sum_{d | gcd} phi(d)`):
  `gcdsum(n, m) = sum_{i=1}^{n} sum_{j=1}^{m} gcd(i, j)
              = sum_{d=1}^{min(n,m)} phi(d) * floor(n/d) * floor(m/d)`.

Precompute `mu` and `phi` with a linear sieve and their prefix sums; then in
each block `[l, r]` where both quotients are constant, advance
`r = min(floor(n / floor(n/l)), floor(m / floor(m/l)))`.

## Input

The first line contains an integer `N` (`1 <= N <= 30`). Each of the next `N`
lines is one query with `1 <= n, m <= LIMIT`:

- `coprime n m` — print `coprime(n, m) mod MOD`.
- `gcd_sum n m` — print `gcdsum(n, m) mod MOD`.

## Output

One line per query: a single integer in `[0, MOD)`.

## Example

Input:

```
4
coprime 3 3
gcd_sum 2 2
coprime 5 5
gcd_sum 10 10
```

Output:

```
7
5
19
189
```

For `n = m = 3` there are `7` coprime ordered pairs. For `n = m = 2`,
`gcd(1,1)+gcd(1,2)+gcd(2,1)+gcd(2,2) = 1+1+1+2 = 5`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- All arithmetic must be reduced modulo `MOD`; the running products overflow a
  64-bit integer if you forget to reduce them.
- The block step must take the **minimum** over both `n` and `m`, otherwise a
  quotient changes inside a block.
