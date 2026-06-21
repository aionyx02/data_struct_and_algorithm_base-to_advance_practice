# Binary Lifting Table

Build the binary-lifting ancestor table for a tree rooted at 1. Input is `n q`, followed by `n-1` edges. Queries are `ancestor v k` (the k-th ancestor, with k=0 returning v and `NONE` above the root), `depth v`, and `table v`. `table` prints `up[v][0..LOG-1]`, using 0 when an ancestor is absent; `LOG` is the smallest positive integer with `2^LOG > n`.
