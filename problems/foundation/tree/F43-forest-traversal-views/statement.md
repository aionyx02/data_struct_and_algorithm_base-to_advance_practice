# Forest Traversal Views

Build ordered traversal views from a parent-array forest. Roots and siblings are
visited in increasing identifier order.

Input starts with `nodeCount operationCount`, followed by the parent array.
`-1` marks a root and the input is guaranteed valid.

Operations are `preorder`, `postorder`, `levelorder`, `tree_preorder rootId`,
`tree_postorder rootId`, `depth id`, `height id`, and `roots`. Height is the
number of nodes on the longest downward path, so a leaf has height `1`.
Invalid identifiers print `OUT_OF_RANGE`; empty views print `EMPTY`.

Do not use standard containers.
