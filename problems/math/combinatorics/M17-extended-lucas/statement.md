# Extended Lucas for Composite Moduli

Compute `C(n, k) mod m` for huge `n` when `m` is not necessarily prime. Fermat
inverses and ordinary Lucas no longer apply. Factor

`m = product p_i^(q_i)`,

compute the binomial modulo every prime power, then combine the residues with
the Chinese remainder theorem.

For one prime power `P = p^q`:

1. Count the exponent of `p` in each factorial using Legendre's formula.
2. Recursively compute factorial products with multiples of `p` removed,
   grouped into complete blocks of length `P`.
3. Invert only values coprime to `P` with extended Euclid.
4. Restore the remaining factor `p^e`; if `e >= q`, the residue is zero.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each following line is:

`binom n k m`

where `0 <= n, k <= 10^18` and `1 <= m <= 100000`. If `k > n`, print zero.

## Output

Print `C(n, k) mod m` for each query. For `m = 1`, print `0`.

## Example

Input:

```
4
binom 8 4 8
binom 10 3 12
binom 20 10 72
binom 5 8 100
```

Output:

```
6
0
4
0
```

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and `<numeric>`.
- Preserve each complete prime power; replacing `p^q` with `p` loses essential
  multiplicity information.
- Fermat's `a^(mod-2)` inverse is invalid for composite `p^q`; use extended
  Euclid on the coprime factorial part.
- Use 128-bit intermediates while merging CRT residues.
