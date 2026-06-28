# Cayley-Hamilton Trace Power

For each matrix `A` and exponent `t`, print `trace(A^t)` modulo
`MOD = 1000000007`.

The identity power is used, so `A^0 = I` and `trace(A^0) = n`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`trace n t`

where `1 <= n <= 90` and `0 <= t <= 10^18`. The next `n` lines contain `A`.

## Output

Print one line per query: `trace(A^t) mod MOD`.

## Example

Input:

```
3
trace 2 0
1 2
3 4
trace 2 1
1 2
3 4
trace 2 2
1 2
3 4
```

Output:

```
2
5
29
```

## Restrictions

- `std::pow` is forbidden.
- Handle `t = 0` as the identity matrix.
- Do not assume only diagonal entries contribute when the matrix is not diagonal.
