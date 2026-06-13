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

- Active priority: complete the forest and Union-Find foundation family.
- Current phase: CH5-2 representation and disjoint-set slice.
- Current owner / handoff state: `TASK.CURRICULUM.010` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add six forest and Union-Find exercises covering parent arrays,
  child-sibling links, forest views, weighting, and path compression.

## Last Validation Snapshot

- Last C++ validation: 2026-06-13.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 189 Judge integration tests passed. F35 through F40 each passed
  10,000 differential operations with seed `20260613`.
- Known duplicate overwrite, predecessor replacement, parent-only neighbor,
  exclusive range, local-only validation, and key-copy replacement mistakes
  produced reproducible `WA` verdicts.
- Known failing checks: none.
