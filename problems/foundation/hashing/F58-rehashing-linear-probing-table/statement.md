# Rehashing Linear Probing Table

Implement a dynamic integer set using linear probing. Before inserting a new
key, if `(size + 1) / capacity` would exceed `0.5`, allocate a table of
`capacity * 2 + 1` slots and reinsert every live key using the new capacity.

Input starts with `initialCapacity operationCount`. Operations are `insert key`,
`contains key`, `slot key`, `capacity`, `size`, `load_percent`, `table`, and
`clear`.

`insert` prints the key's final slot after any rehash. Duplicates print
`DUPLICATE`; missing `slot` queries print `NOT_FOUND`. `load_percent` prints
integer floor `size * 100 / capacity`. `clear` restores the initial capacity.
`table` uses `.` for empty slots.

Do not use standard containers.
