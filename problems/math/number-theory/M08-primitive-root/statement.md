# Multiplicative Order and Primitive Root

Work inside the multiplicative group modulo a prime `p`: compute multiplicative
orders, find the smallest primitive root, count primitive roots, and test
whether an element generates the whole group. M-series, admitted by ADR-0006.

The key tool is the factorization of `p - 1`: the order of `a` divides `p - 1`,
and `a` is a primitive root iff `a^{(p-1)/q} != 1` for every prime `q | p - 1`.

## Input

The first line contains an integer `Q` (`1 <= Q <= 24`). Each of the next `Q`
lines is one query with `p` prime, `2 <= p <= 10^9`, and `1 <= a <= p-1`:

- `order a p` — print the multiplicative order of `a` modulo `p` (the smallest
  `k > 0` with `a^k = 1 (mod p)`).
- `primitive_root p` — print the smallest primitive root of `p`.
- `num_primitive_roots p` — print how many primitive roots `p` has, which is
  `phi(p - 1)`.
- `is_primitive a p` — print `yes` if `a` is a primitive root modulo `p`,
  otherwise `no`.

## Output

One line per query: an order, a primitive root, a count, or `yes`/`no`.

## Example

Input:

```
4
order 2 7
primitive_root 7
num_primitive_roots 7
is_primitive 2 7
```

Output:

```
3
3
2
no
```

`2` has order `3` modulo `7` (`2, 4, 1`), so it is not a primitive root; the
smallest primitive root is `3`, and `7` has `phi(6) = 2` primitive roots.

## Restrictions

- Forbidden symbols: `std::gcd`, `std::__gcd`, `std::pow`, and the `<numeric>`
  header.
- `a^{p-1} = 1` holds for every `a` by Fermat; a primitive root additionally
  needs order exactly `p - 1`, so check `a^{(p-1)/q}` for each prime `q | p-1`.
