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
- Current phase: Stage A2 Segment Tree family; recursive/iterative range sum,
  minimum/maximum, GCD monoids, isolated range-add lazy propagation, and
  isolated and mixed lazy propagation, positional search, dynamic sparse and
  persistent Segment Trees, and the order-statistic Segment Tree (`A09`-`A19`)
  are implemented.
- Current owner / handoff state: `TASK.CURRICULUM.016` is active with `shawn`;
  Stage A1 remains complete at eight problems.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add Segment Tree Beats (`A20`, D5 unlock) to close the twelve-problem Stage A2
  family and finish `TASK.CURRICULUM.016`.

## Last Validation Snapshot

- Last C++ validation: 2026-06-16.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 426 Judge integration tests passed (199.88 s; previous 419 + 1
  catalog, +3 Judge, +3 stress for `A19`). `A19` passed all four fixed tests and
  100 differential cases (20,000 operations) with seed `20260614`; both `A19`
  wrong fixtures (k-th descend that forgets to subtract the left count, and an
  inclusive `<=` rank) receive `WA` on the first generated case.
- Known off-by-one range, endpoint-only difference, single-tree range-query,
  upper-bound lower-bound walk, off-by-one rank, reversed coordinate mapping,
  wrong 2D update direction, point-only sparse storage, exclusive Segment Tree
  endpoints, stale parent aggregates, and invalid min/max or GCD identities
  produced reproducible `WA` verdicts; range-add and range-assign lazy variants
  that skip pushing pending tags also fail; mixed lazy variants with reversed
  assignment/add push order fail; positional searches that ignore their
  left/right boundary fail; dynamic trees that eagerly allocate both children
  fail; persistent trees that mutate old versions in place fail; order-statistic
  trees with a wrong k-th descend branch or inclusive rank fail. Earlier
  F-series mistakes still fail.
- Known failing checks: none.
