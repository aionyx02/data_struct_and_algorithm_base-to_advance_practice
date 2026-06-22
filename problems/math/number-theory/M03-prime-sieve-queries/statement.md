# Prime Sieve Queries

Build a sieve over `[2, 1000000]` once, then answer four kinds of queries in
constant time. A linear (Euler) sieve gives you the smallest prime factor of
every number for free, which `spf` relies on. M-series, admitted by ADR-0006.

Let `LIMIT = 1000000`.

## Input

The first line contains an integer `N` (`1 <= N <= 40`). Each of the next `N`
lines is one query, with every argument in `[0, LIMIT]` unless noted:

- `is_prime x` — print `yes` if `x` is prime, otherwise `no`. `0` and `1` are
  not prime.
- `count a b` — print the number of primes `p` with `a <= p <= b`.
- `nth k` — print the `k`-th prime (`k >= 1`, `nth 1 = 2`), or `NONE` if `k`
  exceeds the number of primes `<= LIMIT`.
- `spf x` — print the smallest prime factor of `x` (`x >= 2`), or `NONE` when
  `x < 2`.

## Output

One line per query: `yes`/`no`, an integer count, the requested prime, the
smallest prime factor, or `NONE`.

## Example

Input:

```
5
is_prime 1
count 2 10
nth 25
spf 84
spf 1000000
```

Output:

```
no
4
97
2
2
```

The primes in `[2, 10]` are `2, 3, 5, 7`; the 25th prime is `97`; the smallest
prime factor of both `84` and `1000000` is `2`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- `count` is inclusive of both endpoints.
- Treat `0` and `1` as non-prime everywhere (primality, counting, and `nth`).
