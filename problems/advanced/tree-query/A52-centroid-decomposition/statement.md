# Centroid Decomposition

Build a deterministic centroid decomposition: start each component from the supplied DFS entry and choose the first node reached by repeatedly entering a child component larger than half. Input is `n q`, then edges. Queries are `centroid_parent v`, `centroid_level v`, `activate v`, `deactivate v`, and `nearest v` (tree distance to an active node or `NONE`).
