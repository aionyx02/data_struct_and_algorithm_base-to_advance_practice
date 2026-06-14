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

- Active priority: begin the high-level data-structure curriculum.
- Current phase: Stage A1 Fenwick Tree family.
- Current owner / handoff state: `TASK.CURRICULUM.015` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add the first Stage A1 exercises for Fenwick point updates, prefix queries,
  and range queries before expanding into advanced variants.

## Last Validation Snapshot

- Last C++ validation: 2026-06-14.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 329 Judge integration tests passed. F59 through F68 each passed
  10,000 differential operations with seed `20260614`.
- Known rotation, stale height, invalid occupancy, red-black invariant,
  shift-accounting, representation-reversal, and copy-ledger mistakes produced
  reproducible `WA` verdicts.
- Known failing checks: none.
