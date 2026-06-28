# Lagrange Inversion for Ordered Forests

Let the formal power series `T(x)` be defined implicitly by

`T(x) = x * (1 + T(x))^m`.

For each query, compute the coefficient of `x^n` in `T(x)^k` modulo
`MOD = 1000000007`. Lagrange inversion gives

`[x^n] T(x)^k = k/n * [t^(n-k)] (1+t)^(mn)
                  = k/n * C(mn, n-k)`.

This counts ordered forests of `k` components in the corresponding bounded
arity tree family.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is:

`forest m n k`

where `1 <= m <= 10`, `1 <= k <= n <= 200000`, and `m*n <= 2000000`.

## Output

Print `[x^n]T(x)^k mod MOD`.

## Example

Input:

```
4
forest 2 4 1
forest 2 4 2
forest 3 5 1
forest 1 6 3
```

Output:

```
14
14
273
10
```

## Restrictions

- `std::pow` is forbidden.
- Lagrange inversion for `T^k` contributes `k/n`, not merely `1/n`.
- Extract coefficient `n-k`, not `n`, from `(1+t)^(mn)`.
- Division by `n` uses a modular inverse.
