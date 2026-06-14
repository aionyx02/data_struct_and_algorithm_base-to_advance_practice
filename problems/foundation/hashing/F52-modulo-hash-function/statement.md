# Normalized Modulo Hash Function

Implement the integer hash function `h(key) = key mod bucketCount`, normalized
to a bucket index in `[0, bucketCount)`, including for negative keys.

Input starts with `bucketCount operationCount`. Operations are `hash key`,
`same_bucket left right`, `sequence start count`, and `bucket_count`.

`same_bucket` prints `true` or `false`. `sequence` prints the bucket indices of
`count` consecutive keys beginning at `start`; a zero-length sequence prints
`EMPTY`.
