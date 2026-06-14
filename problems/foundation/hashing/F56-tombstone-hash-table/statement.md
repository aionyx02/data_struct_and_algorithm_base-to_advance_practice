# Linear Probing With Tombstones

Extend linear probing with deletion tombstones. Lookup must continue through a
tombstone. Insertion remembers the first tombstone but must continue probing
to reject a duplicate key before reusing that slot.

Input starts with `capacity operationCount`. Operations are `insert key`,
`erase key`, `contains key`, `slot key`, `state index`, `table`, `size`,
`tombstones`, and `clear`.

`insert` prints its slot. Successful `erase` prints `REMOVED`; a missing key
prints `NOT_FOUND`. `state` prints `EMPTY`, `TOMBSTONE`, or `OCCUPIED key`;
invalid indices print `OUT_OF_RANGE`. `table` uses `.`, `#`, and integer keys
for empty, tombstone, and occupied slots.

Do not use standard containers.
