# LCA Query Structure

Preprocess a tree rooted at 1 for logarithmic LCA queries. Input is `n q`, then edges. Support `lca a b`, edge-count `distance a b`, `kth_on_path a b k` with zero-based k from a (`OUT_OF_RANGE` when invalid), and `is_ancestor a b`. Use Euler intervals and binary lifting rather than walking a path per query.
