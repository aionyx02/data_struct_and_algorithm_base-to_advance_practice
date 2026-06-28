# Rational OGF Linear Recurrence Coefficients

A sequence with a constant-coefficient linear recurrence has a rational ordinary
generating function. Compute a distant coefficient modulo `MOD = 1000000007`.

For order `k`, the input gives initial values `a0..a(k-1)` and coefficients
`c1..ck` such that

`a(n) = c1*a(n-1) + c2*a(n-2) + ... + ck*a(n-k)`.

Equivalently, the denominator of its OGF is
`1-c1*x-c2*x^2-...-ck*x^k`. Compute `x^n` modulo the characteristic relation

`x^k = c1*x^(k-1) + ... + ck`

with binary exponentiation of degree-`k` polynomials, then dot the resulting
coefficients with the initial vector.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each following line is:

`recurrence k n a0 ... a(k-1) c1 ... ck`

where `1 <= k <= 20`, `0 <= n <= 10^18`, and every value is in `[0, MOD)`.

## Output

Print `a(n) mod MOD` for each query.

## Example

Input:

```
3
recurrence 2 10 0 1 1 1
recurrence 3 1 5 7 11 2 3 4
recurrence 1 5 3 2
```

Output:

```
55
7
96
```

## Restrictions

- `std::pow` is forbidden.
- Keep `c1..ck` in recurrence order when reducing high polynomial degrees.
- `n < k` must return the corresponding initial value without an index shift.
- Iterating from `k` to `n` is impossible for the largest query.
