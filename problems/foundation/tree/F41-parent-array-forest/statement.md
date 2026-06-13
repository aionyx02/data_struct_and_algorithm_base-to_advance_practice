# Parent Array Forest

Store a static rooted forest using one parent array.

Input starts with `nodeCount operationCount`, followed by `nodeCount` parent
identifiers. Nodes are `0` through `nodeCount - 1`; `-1` marks a root. The
input is guaranteed to describe a valid forest.

Operations are `parent id`, `root id`, `depth id`, `children id`, `roots`, and
`size`. Children and roots print in increasing identifier order. Empty lists
print `EMPTY`, a root parent prints `NONE`, and invalid identifiers print
`OUT_OF_RANGE`.

Do not use standard containers.
