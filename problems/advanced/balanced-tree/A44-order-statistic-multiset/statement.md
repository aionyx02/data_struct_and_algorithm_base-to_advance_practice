# Order Statistic Multiset

Implement an order-statistic tree with duplicate keys. Store a multiplicity and
total subtree element count in each node. Input begins with `operationCount`.

Operations: `insert key`, `erase_one key` (`REMOVED` or `NOT_FOUND`), `count
key`, `kth k` (1-indexed or `OUT_OF_RANGE`), `order_of_key key` (number of
elements strictly smaller), inclusive `range_count low high`, `lower_bound key`
and `upper_bound key` (`NONE` if absent), `size`, `print`, and `clear`. Printing
includes duplicates in sorted order, or `EMPTY`.

Use a hand-written balanced tree (for example a randomized Treap); do not use
an ordered library container.
