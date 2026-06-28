# Integer Partitions and Distinct Parts

Count integer partitions modulo `MOD = 1000000007`.

- `partition n` asks for `p(n)`, the number of unordered sums of positive
  integers equal to `n`. Use Euler's generalized pentagonal recurrence:

  `p(n) = p(n-1) + p(n-2) - p(n-5) - p(n-7) + ...`.

  Generalized pentagonal indices are `k(3k-1)/2` and `k(3k+1)/2`; their signs
  occur in pairs `+,+,-,-,...`.

- `distinct n` asks for partitions whose parts are all different. Use a 0/1
  knapsack product `product(1+x^i)`, updating sums in descending order.

Define both counts at `n=0` as one.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is
`partition n` or `distinct n`, where `0 <= n <= 5000`.

## Output

Print the requested count modulo `MOD`.

## Example

Input:

```
6
partition 0
partition 5
partition 10
distinct 0
distinct 5
distinct 10
```

Output:

```
1
7
42
1
3
10
```

## Restrictions

- Pentagonal recurrence signs repeat in pairs; alternating every individual
  term is incorrect.
- Distinct-part knapsack must update in descending order. Ascending order allows
  the same part to be reused.
- Normalize negative recurrence sums modulo `MOD`.
