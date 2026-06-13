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

- Active priority: expand tree traversal and heap representations.
- Current phase: second CH5-1 foundation slice.
- Current owner / handoff state: `TASK.CURRICULUM.008` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add traversal views, level-order representation, heap construction, and
  heap removal traces while keeping the curriculum data-structure-only.

## Last Validation Snapshot

- Last C++ validation: 2026-06-13.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 139 Judge integration tests passed. F27 through F30 each passed
  10,000 differential operations with seed `20260613`.
- Known one-based indexing, missing parent links, ID-neighbor threads, and
  missing heap sift-down mistakes produced reproducible `WA` verdicts.
- Known failing checks: none.
