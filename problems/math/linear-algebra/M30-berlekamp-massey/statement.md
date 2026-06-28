# Berlekamp-Massey Sequence Prediction

Given enough terms of a sequence over `MOD = 1000000007`, infer its shortest
linear recurrence with Berlekamp-Massey and compute a distant term with
Kitamasa-style polynomial reduction.

Every query is generated from some linear recurrence and includes enough prefix
terms to determine the minimal recurrence.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query is:

`predict s n a0 a1 ... a(s-1)`

where `1 <= s <= 160`, `0 <= n <= 10^18`, and every term is in `[0, MOD)`.

## Output

Print `a(n) mod MOD` for each query.

## Example

Input:

```
1
predict 12 50 0 1 1 2 3 5 8 13 21 34 55 89
```

Output:

```
586268941
```

## Restrictions

- `std::pow` is forbidden.
- If `n < s`, return the supplied prefix term directly.
- Berlekamp-Massey returns coefficients for
  `a(i) = c1*a(i-1) + ... + ck*a(i-k)`; preserve that order in reduction.
- Handle the all-zero sequence as a zero-order recurrence with answer `0`.
