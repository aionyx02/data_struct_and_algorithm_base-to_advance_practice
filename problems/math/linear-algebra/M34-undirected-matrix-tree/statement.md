# Undirected Matrix-Tree Theorem

Count spanning trees in an undirected weighted multigraph modulo
`MOD = 1000000007`.

Each edge `(u, v, w)` contributes weight `w`; the weight of a spanning tree is
the product of its chosen edge weights. Build the Laplacian, delete any one row
and matching column, and compute the determinant of the cofactor.

## Input

The first line contains `Q` (`1 <= Q <= 10`). Each query starts with:

`tree n m`

where `1 <= n <= 45`. The next `m` lines contain `u v w` with one-indexed
vertices and `0 <= w < MOD`.

## Output

Print the weighted spanning-tree count for each graph.

## Example

Input:

```
2
tree 3 3
1 2 1
2 3 1
1 3 1
tree 4 2
1 2 1
2 3 1
```

Output:

```
3
0
```

## Restrictions

- `std::pow` is forbidden.
- Add each undirected edge to both symmetric off-diagonal entries.
- Preserve edge weights; treating every edge as weight `1` is incorrect.
- A graph with one vertex has exactly one empty spanning tree.
