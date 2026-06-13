# Fixed Capacity Binary Min Heap

Implement a fixed-capacity binary min heap in a zero-based contiguous array.

Input starts with `capacity operationCount`. Operations are `push value`, `pop`,
`top`, `change index value`, `at index`, `size`, `empty`, and `clear`.

`pop` prints the removed minimum. Push into a full heap prints `FULL`. Pop and
top on an empty heap print `EMPTY`. Invalid `change` and `at` indices print
`OUT_OF_RANGE`. `change` must restore the heap after either increasing or
decreasing a value.

Do not use standard heap facilities or containers.
