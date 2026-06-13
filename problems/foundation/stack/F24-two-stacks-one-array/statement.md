# Two Stacks In One Array

Store two independent stacks in one fixed contiguous array. The left stack
grows from the beginning and the right stack grows from the end.

Input starts with `capacity operationCount`. Operations are `push_left value`,
`push_right value`, `pop_left`, `pop_right`, `top_left`, `top_right`,
`size_left`, `size_right`, `free`, and `clear`.

Push prints `FULL` only when the two stack tops would collide. Pop and top print
`EMPTY` for an empty side. Both stacks must share all unused capacity. Do not
use standard container adaptors or containers.
