# Inclusion-Exclusion Counting

Use inclusion-exclusion to answer two classic labeled counting queries modulo
`MOD = 1000000007`.

## Identities

- Onto functions from an `n`-element labeled set to a `k`-element labeled set:

  `onto(n,k) = sum_{i=0}^{k} (-1)^i C(k,i) (k-i)^n`.

- Derangements of `n` labeled elements:

  `D(n) = n! * sum_{i=0}^{n} (-1)^i / i!`.

Use modular inverses for the factorial denominator. By convention, `0^0 = 1`,
there is one empty function from the empty set to itself, and `D(0) = 1`.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is one query:

- `onto n k`, where `0 <= n <= 10^18` and `0 <= k <= 100000`.
- `derangement n`, where `0 <= n <= 1000000`.

## Output

Print the requested count modulo `MOD` for each query.

## Example

Input:

```
4
onto 0 0
onto 3 2
onto 4 3
derangement 4
```

Output:

```
1
6
36
9
```

## Restrictions

- `std::pow` is forbidden; use binary modular exponentiation.
- The sign is determined by the number `i` of excluded targets or fixed points,
  not by the number that remains.
- Preserve the empty cases `onto(0,0) = 1` and `D(0) = 1`.
- Directly enumerating functions or permutations is far too slow at the limits.
