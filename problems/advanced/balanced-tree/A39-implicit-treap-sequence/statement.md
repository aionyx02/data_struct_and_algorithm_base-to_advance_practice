# Implicit Treap Sequence

Implement a mutable sequence with an implicit Treap. Store subtree size, sum,
and a lazy reversal flag; positions are zero-based and ranges are inclusive.

The first line contains `operationCount`. Operations: `push_back value`,
`insert position value`, `erase position` (prints the removed value), `reverse
left right`, `range_sum left right`, `get position`, `size`, `print`, and
`clear`. Invalid positions or ranges print `OUT_OF_RANGE`; successful insertion
and reversal print nothing. An empty sequence prints `EMPTY`.

Split by subtree size and push lazy reversal before descending. Do not use a
library sequence container as the maintained representation.
