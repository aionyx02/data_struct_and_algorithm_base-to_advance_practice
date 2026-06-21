# Splay Tree Access

Maintain unique keys in a Splay Tree with subtree sizes. The first line is
`operationCount`. `insert key` splays the inserted node and prints only
`DUPLICATE` on failure. `access key` prints `FOUND` or `MISS` and splays the
found node, or the last node visited on a miss. `contains key` follows the same
splaying rule and prints a boolean. `erase key` prints `REMOVED` or
`NOT_FOUND`.

Other operations are `root`, `rank key` (strictly smaller), `kth k`
(1-indexed, and the returned node is splayed), `size`, `inorder`, and `clear`.
Invalid `kth` prints `OUT_OF_RANGE`; an empty root or traversal prints `EMPTY`.
Implement zig, zig-zig, and zig-zag rotations without an ordered container.
