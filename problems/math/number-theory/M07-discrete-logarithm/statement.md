# Discrete Logarithm

Solve `a^x = b (mod p)` for the smallest non-negative exponent `x`, using the
baby-step giant-step (BSGS) meet-in-the-middle search. The modulus `p` is always
prime. M-series, admitted by ADR-0006.

## Input

The first line contains an integer `Q` (`1 <= Q <= 24`). Each of the next `Q`
lines is one query `log a b p` with `p` prime, `2 <= p <= 10^9`,
`1 <= a <= p-1`, and `0 <= b <= p-1`.

Print the smallest `x >= 0` with `a^x = b (mod p)`, or `NONE` if no such `x`
exists (`b` is not a power of `a` modulo `p`).

## Output

One line per query: the smallest exponent, or `NONE`.

## Example

Input:

```
4
log 2 3 5
log 2 1 5
log 2 0 5
log 3 4 7
```

Output:

```
3
0
NONE
4
```

`2^3 = 8 = 3 (mod 5)`; `2^0 = 1`; no power of `2` is `0` modulo a prime; and
`3^4 = 81 = 4 (mod 7)`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and the `<numeric>`
  header.
- Return the smallest non-negative exponent; `a^0 = 1`, so `b = 1` gives `0`.
- No power of `a` is `0` modulo a prime, so `b = 0` is always `NONE`.
