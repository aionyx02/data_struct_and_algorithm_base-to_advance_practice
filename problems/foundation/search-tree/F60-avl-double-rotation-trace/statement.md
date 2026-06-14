# AVL Double Rotation Trace

For each case, repair a three-node AVL imbalance using a double rotation.

Input starts with `caseCount`. Each case is `LR root child grandchild` or
`RL root child grandchild`. The keys follow the named shape and are distinct.

Print `rotation | newRoot | preorder | heights`. Rotation is `LEFT_RIGHT` or
`RIGHT_LEFT`; leaf height is `1`.

Do not use standard containers.
