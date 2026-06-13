# Union Find Path Compression Trace

Implement union-find over an existing valid parent forest. Roots initially
point to themselves; component sizes must be derived from the input.

Input starts with `elementCount operationCount`, followed by the raw parent
array. Operations are `find x`, `connected a b`, `unite a b`, `parent x`,
`component_size x`, `components`, and `snapshot`.

Every `find`, `connected`, and `component_size` query applies full path
compression. `unite` first compresses both paths, then uses component size;
ties attach the root of `b` under the root of `a`. It prints the surviving root
or `ALREADY`. Invalid identifiers print `OUT_OF_RANGE`.

Do not use standard containers.
