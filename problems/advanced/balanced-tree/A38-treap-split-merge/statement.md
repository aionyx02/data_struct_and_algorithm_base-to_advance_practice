# Treap Split and Merge

Maintain unique integer keys in a Treap: keys obey BST order and the explicit
integer priorities obey a max-heap. The first line is `operationCount`.

Operations are `insert key priority` (`DUPLICATE` on an existing key), `erase
key` (`REMOVED` or `NOT_FOUND`), `contains key`, `root` (root key or `EMPTY`),
`rank key`, `kth k` (1-indexed or `OUT_OF_RANGE`), `size`, `inorder`, `clear`,
and non-mutating `split pivot`. Split prints the sorted keys `< pivot`, then
` | `, then keys `>= pivot`; an empty side prints `EMPTY`. Re-merge the two
parts after printing. Priorities in valid input are distinct.

Implement insertion and deletion from the same split/merge primitives; do not
use an ordered library container.
