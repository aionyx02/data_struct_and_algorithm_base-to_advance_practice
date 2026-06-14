# AVL Single Rotation Trace

For each case, repair a three-node AVL imbalance using one single rotation.

Input starts with `caseCount`. Each case is `LL root child grandchild` or
`RR root child grandchild`. The keys follow the named shape and are distinct.

Print `newRoot | preorder | heights`, where heights are listed in preorder and
a leaf has height `1`. Preserve BST ordering by reconnecting the three nodes.

Do not use standard containers.
