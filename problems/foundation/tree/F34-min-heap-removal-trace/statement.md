# Binary Min Heap Removal Trace

Build a min heap with Floyd's bottom-up construction, then expose each removal
transition.

Input starts with `count operationCount`, followed by `count` integers.
Operations are `pop`, `top`, `array`, `at index`, `size`, `empty`, and `clear`.

`pop` prints the removed minimum. `array` prints the current internal array, so
the replacement-root and sift-down result are observable after every removal.
Empty `pop`, `top`, and `array` print `EMPTY`; invalid indices print
`OUT_OF_RANGE`.

Do not use standard containers or heap facilities.
