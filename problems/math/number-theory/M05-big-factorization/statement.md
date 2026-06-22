# Big Number Primality and Factorization

Trial division is too slow past `10^12`. Use **deterministic Miller-Rabin** for
primality and **Pollard's Rho** for factorization to handle `n` up to `10^18`.
M-series, admitted by ADR-0006.

## Input

The first line contains an integer `Q` (`1 <= Q <= 32`). Each of the next `Q`
lines is one query with `1 <= n <= 10^18`:

- `is_prime n` — print `yes` if `n` is prime, otherwise `no`.
- `max_factor n` — print the largest prime factor of `n`, or `NONE` if `n = 1`.
- `num_factors n` — print the number of **distinct** prime factors of `n`
  (`0` for `n = 1`).
- `factor_count n` — print the number of prime factors of `n` **with
  multiplicity** (`0` for `n = 1`).

## Output

One line per query: `yes`/`no`, a prime, a count, or `NONE`.

## Example

Input:

```
4
is_prime 2047
max_factor 600851475143
num_factors 12
factor_count 12
```

Output:

```
no
6857
2
3
```

`2047 = 23 * 89` is composite even though it is a base-2 strong pseudoprime, so
a single Miller-Rabin base is not enough. `12 = 2^2 * 3` has `2` distinct prime
factors but `3` counted with multiplicity.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- Use enough Miller-Rabin witnesses to be deterministic for `n <= 10^18`
  (the first 12 primes suffice).
- Multiply modulo `n` with a 128-bit type; `n` can approach `10^18`.
- Distinguish distinct factors (`num_factors`) from factors with multiplicity
  (`factor_count`).
