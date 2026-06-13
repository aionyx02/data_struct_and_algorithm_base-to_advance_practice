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

- Active priority: build the Graph ADT representation family.
- Current phase: CH6 storage and edge-operation slice.
- Current owner / handoff state: `TASK.CURRICULUM.011` is ready for `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Add five Graph ADT exercises covering adjacency matrix/list,
  directed and undirected edges, degree, and neighbor views.

## Last Validation Snapshot

- Last C++ validation: 2026-06-13.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and `ctest --preset dev`.
- Result: all 219 Judge integration tests passed. F41 through F46 each passed
  10,000 differential operations with seed `20260613`.
- Known one-hop root, missing sibling, first-root-only traversal, non-root
  union, reversed weighting, and missing compression mistakes produced
  reproducible `WA` verdicts.
- Known failing checks: none.
