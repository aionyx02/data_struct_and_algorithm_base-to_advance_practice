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
- Current phase: Stage A6 Tree Query Structures is complete. Euler Tour,
  Binary Lifting, LCA, HLD and HLD+Segment Tree (`A45`-`A49`), Small-to-Large
  and DSU on Tree (`A50`-`A51`), Centroid Decomposition (`A52`), Virtual Tree
  (`A53`), and Link-Cut Tree (`A54`) are implemented. Stages A1-A5
  (`A01`-`A44`) are complete.
- Current owner / handoff state: no active task. `TASK.CURRICULUM.020`
  completed with `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next active task, likely Stage A7 String Index Structures or a
  backlog item.

## Last Validation Snapshot

- Last C++ validation: 2026-06-21.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 671 Judge integration tests passed (~415 s; `A45`-`A54` each add
  1 catalog, +3 Judge, +3 stress). Each of `A45`-`A54` passed all four fixed
  tests; a 100-case differential sweep (seed 99) returned `AC` for all 10
  correct fixtures and `WA` for all 20 known-wrong fixtures.
- Judge runtime self-check (2026-06-21): `algo list` enumerates 122 problems
  (`F01`-`F68` and `A01`-`A54`);
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known-wrong fixtures across the F-series and Stage A still produce reproducible
  non-AC verdicts. Stage A6 stress catches Euler interval/order mistakes,
  ancestor-table off-by-one errors, incomplete LCA climbing, wrong heavy-child
  selection, dropped path/subtree endpoints, incomplete color-container merges,
  missing DSU-on-tree self contribution, wrong centroid selection/nearest
  choice, omitted virtual LCAs/duplicate handling, and Link-Cut path/cut errors.
- Known failing checks: none.
