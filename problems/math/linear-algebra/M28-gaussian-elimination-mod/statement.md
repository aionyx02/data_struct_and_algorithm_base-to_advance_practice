# Gaussian Elimination Mod Prime

Solve linear systems over `MOD = 1000000007`. For each system, classify whether
it has no solution, a unique solution, or infinitely many solutions.

Use row swaps, modular inverses, and rank comparison over the finite field. All
input values are already in `[0, MOD)`.

## Input

The first line contains `Q` (`1 <= Q <= 12`). Each query starts with:

`system n m`

where `1 <= n, m <= 60`. The next `n` lines contain `m` coefficients followed
by the right-hand side.

## Output

Print one line per query:

- `NO` if the system is inconsistent.
- `INF rank` if the system has free variables.
- `UNIQUE x1 x2 ... xm` if the solution is unique.

## Example

Input:

```
3
system 2 2
1 1 5
2 3 13
system 2 2
1 1 1
2 2 3
system 2 3
1 2 3 4
2 4 6 8
```

Output:

```
UNIQUE 2 3
NO
INF 1
```

## Restrictions

- `std::pow` is forbidden; compute modular inverses with integer fast power.
- A valid pivot can appear below the current row, so row swapping is required.
- A zero coefficient row with a non-zero right-hand side is inconsistent.
- `rank < m` means infinitely many solutions, not a unique zero-filled answer.
