# Node Pool Chained Hash Table

Implement separate chaining with a fixed node pool. Each bucket chain remains
sorted by key, and insertion allocates the smallest free node slot.

Input starts with `bucketCount nodeCapacity operationCount`. Operations are
`insert key`, `erase key`, `contains key`, `slot key`, `bucket index`,
`bucket_slots index`, `bucket_size index`, `size`, `free_slots`, and `clear`.

`insert` and successful `erase` print the allocated or released slot.
Duplicates print `DUPLICATE`; a full pool prints `FULL`; missing keys print
`NOT_FOUND`; invalid bucket indices print `OUT_OF_RANGE`. Empty bucket views
print `EMPTY`.

Do not use standard containers.
