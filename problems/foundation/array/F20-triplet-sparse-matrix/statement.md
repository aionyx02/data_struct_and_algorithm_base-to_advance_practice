# Triplet Sparse Matrix

Store nonzero `(row, column, value)` triplets in row-major order.

Input starts with `rows columns capacity operationCount`. Operations are
`set r c value`, `get r c`, `term i`, `nonzero`, and `clear`. Setting zero
removes a triplet.

Invalid coordinates or term indices print `OUT_OF_RANGE`; inserting a new
nonzero into a full representation prints `FULL`. Do not use maps or vectors.
