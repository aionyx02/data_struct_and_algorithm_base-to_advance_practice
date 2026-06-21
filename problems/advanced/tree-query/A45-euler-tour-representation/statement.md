# Euler Tour Representation

Build a rooted-tree Euler order with zero-based inclusive `tin` and `tout`. Input is `n q`, then `n-1` undirected edges; root is 1 and neighbors are visited in increasing order. Queries are `tin v`, `tout v`, `depth v`, `is_ancestor a b`, `subtree v` (preorder interval), and `euler`. Lists are space-separated. Maintain the ancestor interval invariant `tin[a] <= tin[b] <= tout[a]`.
