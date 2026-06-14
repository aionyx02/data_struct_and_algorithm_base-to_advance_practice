# Switchable Stack Representation

Implement one stack ADT that can use either an array or linked-node
representation without changing its observable LIFO behavior.

Input starts with `capacity operationCount`; representation starts as `array`.
Operations are `push value`, `pop`, `top`, `size`, `representation`,
`switch array`, `switch linked`, `values`, and `clear`.

Switching representation prints the number of live values moved; switching to
the current representation prints `UNCHANGED`. `values` prints bottom-to-top.
At capacity print `FULL`; empty pop/top/views print `EMPTY`.

Do not use standard containers.
