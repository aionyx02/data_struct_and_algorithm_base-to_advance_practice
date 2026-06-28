# EGF Permutations with Restricted Cycles

Count permutations whose cycle lengths belong to a permitted set, modulo
`MOD = 1000000007`. The exponential generating function is

`exp(sum_{l in L} x^l/l)`.

For a recurrence, distinguish the label containing `1`. If its cycle has length
`l`, choose the other `l-1` labels, order that cycle in `(l-1)!` ways, and solve
the remaining labels:

`dp[n] = sum_{l in L, l<=n} C(n-1,l-1) (l-1)! dp[n-l]`.

Set `dp[0] = 1`.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is:

`cycles n m l1 ... lm`

where `0 <= n <= 2000`, `0 <= m <= 50`, and the `m` permitted lengths are
distinct integers in `1..n`. Across the input, `sum(n*m) <= 2000000`.

## Output

Print the number of valid permutations modulo `MOD`.

## Example

Input:

```
4
cycles 3 1 3
cycles 4 2 1 3
cycles 5 2 1 2
cycles 0 0
```

Output:

```
2
9
26
1
```

## Restrictions

- The cyclic order contributes `(l-1)!`; choosing only the member set is not
  enough.
- Choose `l-1` companions for one distinguished label, not all `l` labels.
- Preserve the empty permutation count `dp[0]=1`.
