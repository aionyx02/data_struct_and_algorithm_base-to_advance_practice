# Mobius Function and Divisor-Block Sum

Build a linear (Euler) sieve over `[1, 1000000]` once to obtain the Möbius
function `mu` and its prefix sums, then answer three kinds of queries. The
`floor_sum` query is the divisor summatory function and must be answered with
**divisor blocking** (數論分塊) in `O(sqrt n)`, not a naive `O(n)` loop.
M-series, admitted by ADR-0006.

Let `LIMIT = 1000000`.

## Background

- `mu(1) = 1`. For `n > 1` with prime factorisation `n = p1^a1 ... pk^ak`,
  `mu(n) = 0` if any `ai >= 2` (not squarefree), otherwise `mu(n) = (-1)^k`.
- The divisor summatory function is `D(n) = sum_{i=1}^{n} floor(n / i)`. The
  values `floor(n / i)` take only `O(sqrt n)` distinct values, so equal values
  can be summed in contiguous blocks.

## Input

The first line contains an integer `N` (`1 <= N <= 40`). Each of the next `N`
lines is one query:

- `mu x` — print `mu(x)`, with `1 <= x <= LIMIT`.
- `mu_prefix n` — print `sum_{i=1}^{n} mu(i)`, with `1 <= n <= LIMIT`.
- `floor_sum n` — print `sum_{i=1}^{n} floor(n / i)`, with
  `1 <= n <= 1000000000`.

## Output

One line per query: a single integer (`mu(x)` is one of `-1`, `0`, `1`; the
two sums are non-negative for `floor_sum` and may be negative for `mu_prefix`).

## Example

Input:

```
6
mu 1
mu 4
mu 30
mu_prefix 10
floor_sum 1
floor_sum 6
```

Output:

```
1
0
-1
-1
1
14
```

`mu(4) = 0` because `4 = 2^2` is not squarefree; `mu(30) = mu(2*3*5) = -1`.
The prefix `sum_{i=1}^{10} mu(i)` is `-1`. For `n = 6`,
`floor(6/1)+...+floor(6/6) = 6+3+2+1+1+1 = 14`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, and the `<numeric>` header.
- `floor_sum` must use `O(sqrt n)` divisor blocking; `n` can be as large as
  `10^9`, which a per-`i` loop would not finish in time.
- Treat `mu` as `0` on every number that is not squarefree.
