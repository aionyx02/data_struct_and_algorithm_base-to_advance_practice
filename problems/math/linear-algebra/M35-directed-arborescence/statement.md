# Directed Rooted Arborescence Count

Count directed spanning arborescences oriented into a chosen root modulo
`MOD = 1000000007`.

In this problem every non-root vertex chooses exactly one outgoing edge, and
following chosen edges must eventually reach the root. Use the directed
Matrix-Tree theorem with the out-degree Laplacian:

- `L[u][u] += w`
- `L[u][v] -= w` for each edge `u -> v` with weight `w`

Delete the root row and root column, then take the determinant.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`arborescence n m root`

where `1 <= n <= 45`. The next `m` lines contain `u v w` for a directed edge
`u -> v`.

## Output

Print the rooted in-arborescence count for each graph.

## Example

Input:

```
2
arborescence 3 3 1
2 1 1
3 1 1
3 2 1
arborescence 3 2 1
1 2 1
1 3 1
```

Output:

```
2
0
```

## Restrictions

- `std::pow` is forbidden.
- Use the out-degree Laplacian for trees oriented into the root.
- Delete the requested root, not always vertex `1`.
- Edge weights represent parallel choices and must be preserved.
