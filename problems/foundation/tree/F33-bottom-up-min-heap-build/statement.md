# Bottom-Up Binary Min Heap Build

Build a binary min heap using Floyd's bottom-up construction, sifting from the
last internal node down to the root.

Input starts with `count operationCount`, followed by `count` integers.
Operations are `array`, `at index`, `top`, `parent index`, `left index`,
`right index`, `size`, and `valid`.

`array` prints the exact heap array produced by bottom-up construction.
Relative queries print an index or `NONE`; invalid source indices print
`OUT_OF_RANGE`. `valid` prints whether every parent is no greater than its
children. Empty `array` and `top` print `EMPTY`.

Do not use standard containers or heap facilities.
