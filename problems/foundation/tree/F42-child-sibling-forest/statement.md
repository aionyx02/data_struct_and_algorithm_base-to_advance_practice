# Child Sibling Forest

Inspect a static forest stored with parent, first-child, and next-sibling links.

Input starts with `nodeCount rootCount operationCount`, followed by the ordered
root identifiers. Each node row is
`id parentId firstChildId nextSiblingId`; `-1` means no link. The representation
is guaranteed valid.

Operations are `parent id`, `first_child id`, `next_sibling id`, `children id`,
`root id`, `subtree_size id`, and `roots`. Child order follows the sibling
chain. Missing links print `NONE`, empty lists print `EMPTY`, and invalid
identifiers print `OUT_OF_RANGE`.

Do not use standard containers.
