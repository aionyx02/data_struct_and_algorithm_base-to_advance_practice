# Formal Power Series Exponential and Logarithm

Compute truncated formal power series modulo `MOD = 1000000007`.

- `exp F` requires `f0=0` and returns `G=exp(F)` with `g0=1`. From
  `G'=F'G`:
  `n*g_n = sum_{k=1}^n k*f_k*g_(n-k)`.
- `log F` requires `f0=1` and returns `G=log(F)` with `g0=0`. From
  `F'=G'F`, solve each `g_n` after subtracting already-known convolution terms.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each following line is:

`exp n f0 ... fn`

or

`log n f0 ... fn`.

Here `0 <= n <= 1000`, coefficients lie in `[0,MOD)`, and
`sum(n^2) <= 4000000`.

## Output

For each query, print `g0 ... gn` on one line.

## Example

Input:

```
2
exp 3 0 1 1 0
log 3 1 1 0 0
```

Output:

```
1 1 500000005 166666669
0 1 500000003 333333336
```

## Restrictions

- `std::exp`, `std::log`, and `std::pow` are forbidden.
- The exponential recurrence includes the derivative factor `k`.
- The logarithm recurrence subtracts known lower-degree terms.
- Division by the degree uses a modular inverse.
