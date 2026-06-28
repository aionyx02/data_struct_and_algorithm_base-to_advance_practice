# Min_25 Prime Counting and Prime Sums

Compute the number and sum of primes up to `n` without sieving every integer up
to `n`. This problem isolates the base layer of the **Min_25 / Zhouge (洲閣)
sieve**: keep one state for each distinct quotient `floor(n / i)`, initialize it
as a sum over integers `2..v`, then remove composites prime by prime.

Let `MOD = 1000000007`.

## Min_25 base recurrence

For every distinct `v = floor(n / i)`, initialize:

- `g0(v) = v - 1`, the number of integers in `[2, v]`.
- `g1(v) = (1 + ... + v) - 1 (mod MOD)`, their sum.

Process primes `p <= sqrt(n)` in increasing order. If `pc` and `ps` are the
count and sum of primes smaller than `p`, then for every `v >= p^2`:

- `g0(v) -= g0(floor(v/p)) - pc`.
- `g1(v) -= p * (g1(floor(v/p)) - ps) (mod MOD)`.

After all updates, `g0(n) = pi(n)` and `g1(n)` is the prime sum modulo `MOD`.
Map a quotient to its state in `O(1)` using separate indices for values at most
`sqrt(n)` and values represented by `n / v`.

## Input

The first line contains `N` (`1 <= N <= 12`). Each of the next `N` lines is one
query with `1 <= n <= 10000000000`:

- `pi n` — print the number of primes not exceeding `n`.
- `prime_sum n` — print the sum of primes not exceeding `n`, modulo `MOD`.

## Output

Print one integer per query. Prime counts are exact; prime sums are in
`[0, MOD)`.

## Example

Input:

```
4
pi 10
prime_sum 10
pi 100
prime_sum 100
```

Output:

```
4
17
25
1060
```

## Restrictions

- A sieve with `O(n)` memory or time cannot handle the largest input.
- Do not count `1`: both initial states describe the range `[2, v]`.
- The update condition is `v >= p^2`; skipping equality leaves prime squares
  incorrectly classified as primes.
- Use 128-bit intermediate arithmetic for `v(v+1)/2` before reducing modulo
  `MOD`.
