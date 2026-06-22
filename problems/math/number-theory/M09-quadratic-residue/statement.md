# Quadratic Residue and Modular Square Root

Decide whether `a` is a quadratic residue modulo an odd prime `p`, and when it
is, compute a modular square root with **Tonelli-Shanks**. M-series, admitted by
ADR-0006.

## Input

The first line contains an integer `Q` (`1 <= Q <= 24`). Each of the next `Q`
lines is one query with `p` an odd prime, `3 <= p <= 10^9`, and
`0 <= a <= p-1`:

- `legendre a p` — print the Legendre symbol `(a/p)`: `0` if `p | a`, `1` if `a`
  is a non-zero quadratic residue, otherwise `-1`.
- `is_qr a p` — print `yes` if `a` is a quadratic residue modulo `p`
  (including `a = 0`), otherwise `no`.
- `sqrt a p` — print the smallest non-negative `x` with `x^2 = a (mod p)`, or
  `NONE` if `a` is a non-residue. For `a = 0` the answer is `0`.

## Output

One line per query: `-1`/`0`/`1`, `yes`/`no`, the smaller square root, or `NONE`.

## Example

Input:

```
4
legendre 2 7
legendre 3 7
sqrt 2 7
sqrt 3 7
```

Output:

```
1
-1
3
NONE
```

`2` is a residue modulo `7` (`3^2 = 2`, and the two roots `3, 4` give the
smaller `3`); `3` is a non-residue, so it has no square root.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and the `<numeric>`
  header.
- A non-zero residue has two roots `x` and `p - x`; print the smaller one.
- Handle `p % 4 == 3` with the direct formula and `p % 4 == 1` with the full
  Tonelli-Shanks loop.
