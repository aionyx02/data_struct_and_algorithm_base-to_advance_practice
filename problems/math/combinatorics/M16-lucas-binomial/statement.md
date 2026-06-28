# Lucas Theorem for Huge Binomials

Compute `C(n, k) mod p` when `n` and `k` may be as large as `10^18`, but `p`
is a small prime. Precomputing factorials up to `n` is impossible; apply
**Lucas' theorem** digit by digit in base `p`.

Write `n = n0 + n1*p + ...` and `k = k0 + k1*p + ...`. Then

`C(n, k) = product C(ni, ki) (mod p)`.

If any digit has `ki > ni`, the answer is zero. Precompute factorial and inverse
factorial tables for `0..p-1`; cache them when queries reuse the same prime.

## Input

The first line contains `Q` (`1 <= Q <= 20`). Each following line is:

`lucas n k p`

where `0 <= n, k <= 10^18` and `p` is a prime with `2 <= p <= 100000`.
If `k > n`, print zero.

## Output

Print `C(n, k) mod p` for each query.

## Example

Input:

```
4
lucas 5 2 7
lucas 7 1 7
lucas 17 7 7
lucas 10 3 13
```

Output:

```
3
0
2
3
```

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and `<numeric>`.
- Process every base-`p` digit; using only `n mod p` and `k mod p` is not Lucas.
- A single digit with `ki > ni` makes the entire product zero.
- Use modular exponentiation to construct inverse-factorial tables.
