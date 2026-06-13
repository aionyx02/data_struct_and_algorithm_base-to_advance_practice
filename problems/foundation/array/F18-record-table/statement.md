# Fixed Capacity Record Table

Store unique records `(id, value)` in a fixed array.

Input starts with `capacity operationCount`. Operations are `insert id value`,
`update id value`, `erase id`, `get id`, `contains id`, `size`, and `clear`.

Print `DUPLICATE`, `FULL`, or `NOT_FOUND` when applicable. `erase` prints the
removed value. Do not use associative containers or `std::vector`.
