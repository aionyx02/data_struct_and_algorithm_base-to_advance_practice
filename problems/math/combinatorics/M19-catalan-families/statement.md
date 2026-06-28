# Catalan, Narayana, and Ballot Paths

Answer three related counting queries modulo `MOD = 1000000007` using
factorials, modular inverses, and the reflection principle.

## Identities

- `catalan(n) = C(2n,n) / (n+1)`.
- `narayana(n,k) = C(n,k) C(n,k-1) / n` for `n >= 1`.
- For `a >= b`, the number of monotone paths from `(0,0)` to `(a,b)` that never
  enter `y > x` is `C(a+b,b) - C(a+b,b-1)`.

Define `catalan(0) = 1`, `narayana(0,0) = 1`, and other Narayana indices outside
`1..n` as zero.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is one query:

- `catalan n`, `0 <= n <= 1000000`.
- `narayana n k`, `0 <= n <= 1000000`, `k >= 0`.
- `ballot a b`, `a,b >= 0`, `a+b <= 2000000`.

## Output

Print the requested count modulo `MOD`.

## Example

Input:

```
4
catalan 4
narayana 4 2
ballot 2 1
ballot 1 2
```

Output:

```
14
6
2
0
```

## Restrictions

- `std::pow` is forbidden.
- Division is modular multiplication by an inverse, not integer division of an
  already-reduced residue.
- In the reflection formula subtract `C(a+b,b-1)`, not `C(a+b,b+1)`.
- Precompute only up to the largest index required by the input.
