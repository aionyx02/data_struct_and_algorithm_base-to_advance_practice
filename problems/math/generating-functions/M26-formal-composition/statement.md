# Formal Power Series Composition

Given two truncated formal power series

`A(x) = a0 + a1*x + ... + an*x^n`

and

`B(x) = b1*x + ... + bn*x^n` with `B(0)=0`,

compute one coefficient of `A(B(x))` modulo `MOD = 1000000007`.

Build `B(x)^i` incrementally, starting from `B^0=1`, and truncate every
multiplication to the requested target degree.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each following line is:

`compose n k a0 ... an b0 ... bn`

where `0 <= k <= n <= 200`, `b0=0`, and all coefficients are in `[0,MOD)`.
Across the input, `sum(n^3) <= 10000000`.

## Output

Print `[x^k]A(B(x)) mod MOD`.

## Example

Input:

```
3
compose 3 0 5 7 11 13 0 1 0 0
compose 2 2 0 1 0 0 1 1
compose 2 2 1 2 3 0 2 1
```

Output:

```
5
1
14
```

## Restrictions

- Start with `B^0=1`; starting with `B` loses the constant contribution.
- Truncation must retain the target degree `k`, not stop at `k-1`.
- `B(0)=0` guarantees that the truncated composition is well-defined.
