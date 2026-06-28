# Newton Identities from Power Sums

Given power sums `p_i = sum_j x_j^i` modulo `MOD = 1000000007`, reconstruct one
coefficient of a symmetric generating function.

- Elementary symmetric values satisfy
  `k*e_k = sum_{i=1}^k (-1)^(i-1) e_(k-i) p_i`.
- Complete homogeneous symmetric values satisfy
  `k*h_k = sum_{i=1}^k h_(k-i) p_i`.

In both cases the zeroth value is one. These are Newton's identities for
`product(1+x_j t)` and `product(1-x_j t)^(-1)`.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each following line is:

`elementary k p1 ... pk`

or

`complete k p1 ... pk`.

Here `0 <= k <= 2000`, each `p_i` is in `[0, MOD)`, and the sum of all `k` is at
most `4000`.

## Output

Print `e_k` or `h_k` modulo `MOD`.

## Example

For hidden variables `1,2`, the power sums are `p1=3`, `p2=5`.

Input:

```
4
elementary 0
elementary 2 3 5
complete 2 3 5
elementary 2 0 2
```

Output:

```
1
2
7
1000000006
```

The final query corresponds to hidden variables `-1,1`, whose product is `-1`.

## Restrictions

- Elementary identities alternate signs; complete homogeneous identities do
  not.
- Division by `k` is multiplication by `k^(-1) mod MOD`, not integer division
  of the normalized residue.
- Preserve the degree-zero value as one.
