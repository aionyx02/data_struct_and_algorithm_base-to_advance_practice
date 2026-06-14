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

- Active priority: build the Hash Table foundation family.
- Current phase: CH8 collision handling and table lifecycle.
- Current owner / handoff state: `TASK.CURRICULUM.012` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add seven Hash Table exercises covering bucket mapping, open addressing,
  tombstones, separate chaining, and observable rehash transitions.

## Last Validation Snapshot

- Last C++ validation: 2026-06-14.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 244 Judge integration tests passed. F47 through F51 each passed
  10,000 differential operations with seed `20260614`.
- Known asymmetric matrix, duplicate arc count, unsorted adjacency list,
  stale in-degree, and missing edge-slot reuse mistakes produced reproducible
  `WA` verdicts.
- Known failing checks: none.
