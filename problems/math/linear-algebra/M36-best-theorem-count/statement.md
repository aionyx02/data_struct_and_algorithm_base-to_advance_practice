# BEST Theorem Euler Circuit Count

For a directed Eulerian multigraph with labeled edges, count Euler circuits
modulo `MOD = 1000000007` using the BEST theorem. The starting vertex is fixed,
and one outgoing edge from that vertex is considered fixed as the first edge.

If the graph is not balanced or has no arborescence into the root, print `0`.
Otherwise the answer is:

`t(root) * product_v (outdeg(v)-1)!`

where `t(root)` is the number of directed arborescences oriented into `root`.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`best n m root`

where `1 <= n <= 45`. The next `m` lines contain a directed edge `u v`.
Parallel edges are distinct labeled edges.

## Output

Print the BEST-theorem count for each graph.

## Example

Input:

```
2
best 3 3 1
1 2
2 3
3 1
best 2 4 1
1 2
1 2
2 1
2 1
```

Output:

```
1
2
```

## Restrictions

- `std::pow` is forbidden.
- Check `indeg(v) == outdeg(v)` for every vertex before applying BEST.
- Use arborescences oriented into the root.
- Include the factorial product; the arborescence count alone is incomplete.
