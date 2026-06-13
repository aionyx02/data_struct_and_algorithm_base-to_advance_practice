# Level-Order Binary Tree View

Build a static linked binary tree and expose breadth-first structural views
using a manually implemented queue.

Input uses the same `nodeCount rootId operationCount` header and
`id value leftId rightId` node rows as F31.

Operations are `levelorder`, `level depth`, `depth id`, `width depth`,
`levels`, and `queue_peak`. The root is at depth `0`. `level` prints
identifiers on that depth or `EMPTY`. `width` prints the number of nodes at the
depth. `levels` is the number of nonempty levels. `queue_peak` is the maximum
number of nodes simultaneously held by a standard left-before-right BFS.
Missing identifiers print `NOT_FOUND`; negative depths print `OUT_OF_RANGE`.

Do not use standard containers or queue adaptors.
