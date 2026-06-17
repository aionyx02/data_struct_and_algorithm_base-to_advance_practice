---
type: working_memory
status: active
priority: p0
updated: 2026-06-17
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
- Current phase: Stage A3 Static Range Structures is complete. The Sparse Table
  range-query tree (`A21`), the Disjoint Sparse Table range sum (`A22`), the
  Square Root Decomposition range sum (`A23`), block decomposition with lazy
  tags (`A24`), Merge Sort Tree range count (`A25`), Wavelet Tree range
  kth/count (`A26`), Wavelet Matrix range kth/count (`A27`), and bitset set
  representation (`A28`) are implemented. Stage A2 (`A09`-`A20`) and Stage A1
  (`A01`-`A08`) are complete.
- Current owner / handoff state: no active task. `TASK.CURRICULUM.017`
  completed with `shawn` on 2026-06-17.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next active task, likely Stage A4 Union-Find variants or a backlog
  item.

## Last Validation Snapshot

- Last C++ validation: 2026-06-17.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 489 Judge integration tests passed (232.26 s; `A19`-`A28` each add
  1 catalog, +3 Judge, +3 stress). Each of `A19`-`A28` passed all four fixed
  tests and 100 differential cases; their known-wrong fixtures receive `WA`.
- Judge runtime self-check (2026-06-17): `algo list` enumerates 96 problems
  (`F01`-`F68` and `A01`-`A28`);
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known-wrong fixtures across F-series, Fenwick, Segment Tree, Sparse Table,
  Disjoint Sparse Table, Square Root Decomposition, block-lazy decomposition,
  Merge Sort Tree, Wavelet Tree, Wavelet Matrix, and bitset set representation
  still produce reproducible non-AC verdicts. Current A3 stress catches
  endpoint, overlap, stale cache, lazy-tag, inclusive-bound, child-rank,
  zero-offset, boundary-mask, and cached-total mistakes.
- Known failing checks: none.
