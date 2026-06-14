# Amortized Growth Ledger

Implement a dynamic array and expose the copy cost behind amortized growth.
Input starts with `initialCapacity operationCount`.

Operations are `push value`, `pop`, `reserve capacity`, `size`, `capacity`,
`total_copies`, `values`, and `clear`. Capacity doubles when `push` is full.
Successful `push` prints the number of existing elements copied by that push.
Growing `reserve` prints the number copied; a non-growing reserve prints
`UNCHANGED`. `clear` preserves capacity and cumulative copy count.

Do not use standard containers.
