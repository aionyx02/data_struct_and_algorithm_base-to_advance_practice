# Modular Arithmetic Toolkit

Implement the core modular-arithmetic primitives every competitive programmer
relies on: greatest common divisor, fast modular exponentiation, modular
multiplicative inverse, and least common multiple under a modulus. This is the
first problem of the **M-series** (number theory and mathematics), admitted to
the local Judge by ADR-0006.

You must implement the primitives yourself. Library helpers that trivialize the
exercise are forbidden (see Restrictions).

## Input

The first line contains an integer `N` (`1 <= N <= 48`): the number of
operations. Each of the next `N` lines is one operation:

- `gcd a b` — print `gcd(|a|, |b|)`. By convention `gcd(0, 0) = 0`.
- `pow a b m` — print `a^b mod m`, normalized to `[0, m)`. Here `b >= 0`,
  `m >= 1`, and `0^0 = 1`. When `m = 1` the answer is `0`.
- `inv a m` — print the modular inverse of `a` modulo `m` in `[0, m)`, or
  `NONE` when it does not exist (that is, when `gcd(a, m) != 1`). Here `m >= 2`.
- `lcm a b m` — print `lcm(|a|, |b|) mod m`, normalized to `[0, m)`. If either
  operand is `0` the least common multiple is `0`.

Constraints: `-2*10^9 <= a, b <= 2*10^9`, `1 <= m <= 2*10^9`, and for `pow`,
`0 <= b <= 10^18`. Intermediate products can exceed 64 bits, so multiply with a
128-bit accumulator (or an equivalent safe modular multiply).

## Output

One line per operation, in order: either an integer in `[0, m)` (or the exact
`gcd` value for `gcd`), or `NONE` for a non-invertible `inv` query.

## Example

Input:

```
4
gcd 24 36
pow 2 10 1000
inv 5 14
inv 4 14
```

Output:

```
12
24
3
NONE
```

`2^10 = 1024`, and `1024 mod 1000 = 24`. The inverse of `5` modulo `14` is `3`
because `5*3 = 15 = 1 (mod 14)`. `4` has no inverse modulo `14` because
`gcd(4, 14) = 2`.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::lcm`, `std::__gcd`, `std::pow`, and the
  `<numeric>` header. Implement `gcd`, fast exponentiation, and the extended
  Euclidean algorithm by hand.
- Reduce operands and normalize every result into `[0, m)`; never emit a
  negative representative.
- Do not reduce operands modulo `m` before computing a `gcd` or `lcm`: the
  modulus applies to the final value only.
