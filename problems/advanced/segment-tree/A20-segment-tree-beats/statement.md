# Segment Tree Beats

Maintain an array under range "chmin" updates (`a[i] = min(a[i], x)` over a
range) together with range sum and range maximum queries. A plain lazy Segment
Tree cannot express `chmin`, so each node also tracks the strict second maximum
and the count of elements equal to the maximum. A `chmin` with bound `x`:

- does nothing when `x >= nodeMax`;
- when `secondMax < x < nodeMax`, lowers only the maximum elements, so the sum
  drops by `(nodeMax - x) * maxCount` and the node maximum becomes `x`;
- otherwise recurses into both children (the Beats step) and re-pulls.

This recursion is what keeps the structure correct and amortized efficient.

## Input

The first line contains `n` and `operationCount`. The second line contains `n`
initial integer values.

Each following line is one operation:

- `chmin left right x`: replace each value in the inclusive range with its
  minimum against `x`. Prints nothing on success.
- `sum left right`: print the sum over the inclusive range.
- `max left right`: print the maximum over the inclusive range.
- `size`: print `n`.

Print `OUT_OF_RANGE` when an index is outside `[0, n)`, or when a range has
reversed endpoints (`left > right`).

## Restrictions

- Use a Segment Tree Beats node that stores the maximum, the strict second
  maximum, the count of maximum elements, and the sum.
- A `chmin` must skip subtrees whose maximum is already at most `x`, apply the
  tag in `O(1)` only when its bound is strictly above the second maximum, and
  otherwise recurse into both children.
- Scale the sum update by the count of maximum elements, not by the whole node
  size.
- Use explicit array storage. Do not use `std::vector`, `std::map`,
  `std::unordered_map`, `std::set`, `std::multiset`, or `std::list`.
- Queries and updates must respect their `left` and `right` boundary.
- Storage must be `O(n)`.
