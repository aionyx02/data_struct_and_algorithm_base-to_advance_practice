---
type: working_memory
status: active
priority: p0
updated: 2026-06-16
context_policy: always_retrievable
owner: project
---

# Current Project Memory

## Current Strategy

- Keep the curriculum limited to data structures: 68 foundational and 64 advanced problems.
- Build the CLI Judge before the Web interface; both must share one Judge core.
- Expand the catalog in small prerequisite-ordered chapters.

## Current Focus

- Active priority: build out the high-level data-structure curriculum.
- Current phase: Stage A2 Segment Tree family is complete at all twelve problems
  (`A09`-`A20`): recursive/iterative range sum, minimum/maximum, GCD monoids,
  isolated range-add and range-assign lazy propagation, mixed lazy propagation,
  positional search, dynamic sparse and persistent Segment Trees, the
  order-statistic Segment Tree, and Segment Tree Beats.
- Current owner / handoff state: `TASK.CURRICULUM.016` is complete; no task is in
  progress. Stage A1 remains complete at eight problems.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Stage A2 is closed. Begin Stage A3 Static Range Structures (Sparse Table,
  Disjoint Sparse Table, Sqrt Decomposition, block decomposition, Merge Sort
  Tree, Wavelet Tree/Matrix, bitset set) per `docs/learning-roadmap.md` when the
  maintainer opens the next curriculum task.

## Last Validation Snapshot

- Last C++ validation: 2026-06-16.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 433 Judge integration tests passed (212.66 s; previous 419 + 7 for
  `A19` and +7 for `A20`, each 1 catalog, +3 Judge, +3 stress). `A19` and `A20`
  each passed all four fixed tests and 100 differential cases (20,000 operations)
  with seed `20260614`; their wrong fixtures (`A19`: k-th descend that forgets to
  subtract the left count, inclusive `<=` rank; `A20`: a `chmin` that skips the
  Beats recursion, and one that scales the sum by the whole node size) receive
  `WA` on the first generated case.
- Known off-by-one range, endpoint-only difference, single-tree range-query,
  upper-bound lower-bound walk, off-by-one rank, reversed coordinate mapping,
  wrong 2D update direction, point-only sparse storage, exclusive Segment Tree
  endpoints, stale parent aggregates, and invalid min/max or GCD identities
  produced reproducible `WA` verdicts; range-add and range-assign lazy variants
  that skip pushing pending tags also fail; mixed lazy variants with reversed
  assignment/add push order fail; positional searches that ignore their
  left/right boundary fail; dynamic trees that eagerly allocate both children
  fail; persistent trees that mutate old versions in place fail; order-statistic
  trees with a wrong k-th descend branch or inclusive rank fail; Segment Tree
  Beats variants that skip the second-max recursion or scale by the whole node
  size fail. Earlier F-series mistakes still fail.
- Known failing checks: none.
