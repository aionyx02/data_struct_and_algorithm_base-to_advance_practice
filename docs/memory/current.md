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
  isolated range-assign lazy propagation (`A09`-`A14`) are implemented.
- Current owner / handoff state: `TASK.CURRICULUM.016` is active with `shawn`;
  Stage A1 remains complete at eight problems.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add mixed lazy propagation (`A15`) to verify range-add/range-assign tag
  composition order.

## Last Validation Snapshot

- Last C++ validation: 2026-06-16.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 399 Judge integration tests passed. A01 through A14 each passed
  1,000 differential cases (200,000 operations) with seed `20260614`.
- Known off-by-one range, endpoint-only difference, single-tree range-query,
  upper-bound lower-bound walk, off-by-one rank, reversed coordinate mapping,
  wrong 2D update direction, point-only sparse storage, exclusive Segment Tree
  endpoints, stale parent aggregates, and invalid min/max or GCD identities
  produced reproducible `WA` verdicts; range-add and range-assign lazy variants
  that skip pushing pending tags also fail. Earlier F-series mistakes still fail.
- Known failing checks: none.
