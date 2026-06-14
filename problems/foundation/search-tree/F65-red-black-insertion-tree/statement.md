# Red Black Insertion Tree

Implement a red-black tree with unique integer keys, parent links, standard
BST insertion, recoloring, and rotations.

Input starts with `operationCount`. Operations are `insert key`, `contains key`,
`root`, `color key`, `parent key`, `preorder`, `inorder`, `black_height`,
`validate`, `size`, and `clear`.

Successful insertion prints nothing; duplicates print `DUPLICATE`. Colors are
`RED` and `BLACK`. Missing node queries print `NOT_FOUND`; root parent is
`NONE`; empty views print `EMPTY`. `black_height` counts black nodes from the
root to a null leaf, including the null leaf. `validate` checks BST ordering,
root color, red adjacency, parent links, and equal black height.

Do not use standard containers.
