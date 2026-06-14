---
type: working_memory
status: active
priority: p0
updated: 2026-06-14
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
- Current phase: Stage A1 Fenwick Tree family; first three core problems
  (`A01`–`A03`) are implemented under `problems/advanced/fenwick/`.
- Current owner / handoff state: `TASK.CURRICULUM.015` stays with `shawn`;
  remaining five A1 variants are still todo.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add the next Stage A1 batch starting at prefix lower bound and frequency
  order statistics, then coordinate-indexed, two-dimensional, and sparse Fenwick
  representations to finish the eight-problem family.

## Last Validation Snapshot

- Last C++ validation: 2026-06-14.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 344 Judge integration tests passed. A01 through A03 each passed
  1,000 differential cases (200,000 operations) with seed `20260614`.
- Known off-by-one range, endpoint-only difference, and single-tree range-query
  mistakes produced reproducible `WA` verdicts; earlier F-series invariant
  mistakes still fail as before.
- Known failing checks: none.
