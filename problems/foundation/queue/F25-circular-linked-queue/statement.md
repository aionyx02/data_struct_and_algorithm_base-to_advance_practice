# Circular Linked Queue

Implement a queue as a circular singly linked list represented by one tail
pointer. When nonempty, `tail->next` is the front node.

Input starts with `operationCount`. Operations are `enqueue value`, `dequeue`,
`front`, `back`, `rotate`, `size`, `empty`, and `clear`.

`rotate` moves the current front node to the back in `O(1)` and prints nothing.
On an empty queue, `dequeue`, `front`, `back`, and `rotate` print `EMPTY`.
Do not use standard containers.
