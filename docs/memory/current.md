---
type: working_memory
status: active
priority: p0
updated: 2026-06-13
context_policy: always_retrievable
owner: project
---

# Current Project Memory

## Current Strategy

- Keep the curriculum limited to data structures: 68 foundational and 64 advanced problems.
- Build the CLI Judge before the Web interface; both must share one Judge core.
- Expand the catalog in small prerequisite-ordered chapters.

## Current Focus

- Active priority: implement list transformations and ownership-focused exercises.
- Current phase: third foundation curriculum chapter.
- Current owner / handoff state: `TASK.CURRICULUM.003` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Define stable IDs for reversal, concatenation or splice, split, and node-reuse exercises.

## Last Validation Snapshot

- Last C++ validation: 2026-06-13.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 43 Judge integration tests passed. F06 through F10 each passed 10,000
  differential operations with seed `20260613`, for 50,000 new operations total.
- Known faulty stale-tail and missing-rotate submissions produced reproducible WA.
- Known failing checks: none.
