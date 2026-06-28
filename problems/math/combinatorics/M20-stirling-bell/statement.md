# Stirling Numbers and Bell Numbers

Precompute three classical labeled counting sequences modulo
`MOD = 1000000007`.

- Unsigned Stirling numbers of the first kind count permutations of `n` items
  with exactly `k` cycles:
  `c(n,k) = c(n-1,k-1) + (n-1)c(n-1,k)`.
- Stirling numbers of the second kind count partitions of `n` labeled items into
  `k` nonempty unlabeled blocks:
  `S(n,k) = S(n-1,k-1) + kS(n-1,k)`.
- `Bell(n) = sum_k S(n,k)`.

The shared base is `c(0,0) = S(0,0) = Bell(0) = 1`.

## Input

The first line contains `Q` (`1 <= Q <= 16`). Each following line is one query:

- `stirling1 n k`
- `stirling2 n k`
- `bell n`

with `0 <= n <= 2000` and `k >= 0`.

## Output

Print the requested value modulo `MOD`; print zero when `k > n`.

## Example

Input:

```
4
stirling1 4 2
stirling2 4 2
bell 4
bell 0
```

Output:

```
11
7
15
1
```

## Restrictions

- The first-kind multiplier is `n-1`, while the second-kind multiplier is `k`.
- Preserve the empty base `(0,0) = 1`; otherwise every recurrence entry remains
  zero.
- Store residues in 32-bit cells if needed to stay within the memory limit, but
  use 64-bit multiplication for recurrence products.
