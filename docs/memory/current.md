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
- Current phase: Stage A5 Balanced/Randomized/Meldable Trees is complete.
  Order-statistic AVL and red-black trees (`A36`-`A37`), explicit and implicit
  Treaps (`A38`-`A39`), Splay Tree (`A40`), Leftist and Skew Heaps (`A41`-`A42`),
  Binomial Heap (`A43`), and a duplicate-aware order-statistic tree (`A44`) are
  implemented. Stages A1-A4 (`A01`-`A35`) are complete.
- Current owner / handoff state: no active task. `TASK.CURRICULUM.019`
  completed with `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next active task, likely Stage A6 Tree Query Structures or a
  backlog item.

## Last Validation Snapshot

- Last C++ validation: 2026-06-21.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 601 Judge integration tests passed (~349 s; `A36`-`A44` each add
  1 catalog, +3 Judge, +3 stress). Each of `A36`-`A44` passed all four fixed
  tests; a 100-case differential sweep (seed 99) returned `AC` for all 9 correct
  fixtures and `WA` for all 18 known-wrong fixtures.
- Judge runtime self-check (2026-06-21): `algo list` enumerates 112 problems
  (`F01`-`F68` and `A01`-`A44`);
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known-wrong fixtures across the F-series and Stage A still produce reproducible
  non-AC verdicts. Stage A5 stress catches inclusive rank/split boundaries,
  missing AVL/Treap/Splay rebalancing, stale deletion, implicit-Treap range-end
  mistakes, incorrect Leftist NPL, missing Skew swaps, meld source ownership,
  reversed Binomial root degrees, and duplicate erase/order-statistic mistakes.
- Known failing checks: none.
