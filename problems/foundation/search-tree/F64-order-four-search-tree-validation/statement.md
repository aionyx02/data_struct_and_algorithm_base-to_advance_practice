# Order Four Search Tree Validation

Validate a fixed order-four multiway search tree. Each node stores one to
three sorted keys and either zero children or exactly `keyCount + 1` children.

Input starts with `nodeCount rootId`. Each record is:
`id keyCount key0 key1 key2 child0 child1 child2 child3`.
Unused key and child fields contain `-1`.

Print `VALID`, `INVALID_LINK`, `CYCLE`, `DISCONNECTED`, `INVALID_OCCUPANCY`,
`INVALID_KEYS`, or `INVALID_RANGE`. The root may contain one to three keys;
non-root nodes must also contain at least one key. Child subtrees must respect
the separator key ranges.

Do not use standard containers.
