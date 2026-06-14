# Sequence Shift Cost Trace

Implement a fixed-capacity array sequence and expose the exact element shifts.
Input starts with `capacity operationCount`.

Operations are `insert index value`, `erase index`, `get index`, `size`,
`capacity`, `values`, and `clear`. Successful `insert` prints the number of
elements shifted right. Successful `erase` prints `removedValue shifts`.
Invalid indices print `OUT_OF_RANGE`; insertion at capacity prints `FULL`;
empty values print `EMPTY`.

Do not use standard containers.
