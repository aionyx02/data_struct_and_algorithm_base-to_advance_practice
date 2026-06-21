---
type: working_memory
status: active
priority: p0
updated: 2026-06-21
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
- Current phase: Stage A4 Union-Find Variants is complete. Union by rank with
  path compression (`A29`), disjoint-set amortized cost (`A30`),
  potential/weighted DSU (`A31`), parity DSU (`A32`), rollback DSU (`A33`),
  persistent DSU (`A34`), and successor DSU (`A35`) are implemented. Stage A3
  (`A21`-`A28`), Stage A2 (`A09`-`A20`), and Stage A1 (`A01`-`A08`) are
  complete.
- Current owner / handoff state: no active task. `TASK.CURRICULUM.018`
  completed with `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next active task, likely Stage A5 Balanced/Randomized/Meldable
  Trees or a backlog item.

## Last Validation Snapshot

- Last C++ validation: 2026-06-21.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 538 Judge integration tests passed (~250 s; `A29`-`A35` each add
  1 catalog, +3 Judge, +3 stress). Each of `A29`-`A35` passed all four fixed
  tests; a 100-case differential sweep (seed 99) returned `AC` for all 7 correct
  fixtures and `WA` for all 14 known-wrong fixtures.
- Judge runtime self-check (2026-06-21): `algo list` enumerates 103 problems
  (`F01`-`F68` and `A01`-`A35`);
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known-wrong fixtures across the F-series and Stage A still produce reproducible
  non-AC verdicts. Stage A4 stress catches size-vs-rank confusion, missing path
  compression, node-vs-edge cost off-by-one, weighted/parity compression
  accumulation errors, merge-offset mistakes, rollback-with-compression,
  partial-rollback, persistence base mutation, wrong fork version, and
  successor marks-x / sentinel off-by-one mistakes.
- Known failing checks: none.
