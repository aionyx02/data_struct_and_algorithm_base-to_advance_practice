# AVL Insertion Tree

Implement an AVL tree with unique integer keys, explicit parent links, stored
heights, and automatic LL, RR, LR, and RL rebalancing.

Input starts with `operationCount`. Operations are `insert key`, `contains key`,
`root`, `parent key`, `height key`, `balance key`, `preorder`, `inorder`,
`size`, and `clear`.

A duplicate insert prints `DUPLICATE`; successful insertion prints nothing.
Missing node queries print `NOT_FOUND`; the root parent is `NONE`; empty tree
views print `EMPTY`. A leaf has height `1`, and balance is
`height(left) - height(right)`.

Do not use standard containers.
