# Du Sieve for Totient and Mobius Prefix Sums

Compute prefix sums of the Euler totient `phi` and the Möbius function `mu` for
`n` as large as `10^9`, where a full sieve to `n` does not fit in memory. Use
the **Du sieve (杜教篩)**: linear-sieve a prefix up to a threshold, then evaluate
larger arguments by a memoised recurrence with divisor blocking, giving
`O(n^(2/3))` per query. M-series, admitted by ADR-0006.

Let `Sphi(n) = sum_{i=1}^{n} phi(i)` and `Smu(n) = sum_{i=1}^{n} mu(i)`.

## Recurrences

From `sum_{d=1}^{n} Sphi(floor(n/d)) = n(n+1)/2` and
`sum_{d=1}^{n} Smu(floor(n/d)) = 1`:

- `Sphi(n) = n(n+1)/2 - sum_{d=2}^{n} Sphi(floor(n/d))`.
- `Smu(n)  = 1        - sum_{d=2}^{n} Smu(floor(n/d))`.

The inner sum is evaluated with divisor blocking over `d`, and small arguments
(`n <= threshold`) are answered from the precomputed linear-sieve prefix sums.

## Input

The first line contains an integer `N` (`1 <= N <= 20`). Each of the next `N`
lines is one query with `1 <= n <= 1000000000`:

- `phi_prefix n` — print `Sphi(n)`.
- `mu_prefix n` — print `Smu(n)`.

## Output

One line per query: a single integer (`Sphi(n)` is positive and fits in a
signed 64-bit integer; `Smu(n)` may be negative).

## Example

Input:

```
4
phi_prefix 5
mu_prefix 10
phi_prefix 100
mu_prefix 100
```

Output:

```
10
-1
3044
1
```

`Sphi(5) = 1+1+2+2+4 = 10`; `Smu(10) = -1`; `Sphi(100) = 3044`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- A linear or `O(n)` sweep per query is too slow and a sieve to `10^9` exceeds
  the memory limit; use the sublinear recurrence with memoisation.
- `n(n+1)/2` fits in a signed 64-bit integer for `n <= 10^9`.
