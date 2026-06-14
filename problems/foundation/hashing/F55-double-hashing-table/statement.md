# Double Hashing Table

Implement a fixed-capacity integer set. The table capacity is prime. Use
`h1(key) = normalized_mod(key, capacity)` and
`h2(key) = 1 + normalized_mod(key, capacity - 1)`, then probe
`(h1(key) + i * h2(key)) mod capacity`.

The operations and outputs are the same as the previous probing exercises:
`insert`, `contains`, `slot`, `probe_count`, `table`, `size`, `capacity`, and
`clear`.

Do not use standard containers.
