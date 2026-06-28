# Powerful Number Sieve

A positive integer is **powerful** when every prime in its factorization has
exponent at least two. Thus `1`, `4`, `8`, `9`, and `72 = 2^3 * 3^2` are
powerful, while `2`, `12`, and `18` are not.

Every powerful integer has a unique representation

`x = a^2 * b^3`, where `a >= 1` and `b` is square-free.

Use this representation as a Powerful Number sieve. Let `MOD = 1000000007`.
For each square-free `b <= cuberoot(n)`, set
`A = floor(sqrt(n / b^3))`. This `b` contributes `A` numbers and

`b^3 * (1^2 + 2^2 + ... + A^2)`

to their sum. Recall `1^2 + ... + A^2 = A(A+1)(2A+1)/6`.

## Input

The first line contains `N` (`1 <= N <= 16`). Each of the next `N` lines is one
query with `1 <= n <= 1000000000000000000`:

- `count n` — print the number of powerful integers not exceeding `n`.
- `sum n` — print their sum modulo `MOD`.

## Output

Print one integer per query. Counts are exact; sums are in `[0, MOD)`.

## Example

Input:

```
4
count 3
count 10
sum 10
sum 64
```

Output:

```
1
4
22
271
```

The powerful numbers up to `10` are `1, 4, 8, 9`.

## Restrictions

- Include `1` (`a = b = 1`).
- Sieve square-free `b`; allowing a factor divisible by a prime square destroys
  uniqueness and double-counts values such as `64`.
- Correct floating-point root estimates with integer checks. Use 128-bit
  intermediates for squares and cubes near `10^18`.
- Enumerating all integers up to `n` is impossible; enumerate only
  `b <= cuberoot(n)`.
