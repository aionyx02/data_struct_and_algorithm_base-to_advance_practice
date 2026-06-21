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
- Current phase: Stage A7 String Index Structures is complete. Trie, compressed
  Radix Tree, XOR Trie, Aho-Corasick, Suffix Array/LCP, and Suffix Automaton
  (`A55`-`A60`) are implemented. Stages A1-A6 (`A01`-`A54`) are complete.
- Current owner / handoff state: no active task. `TASK.CURRICULUM.021`
  completed with `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next active task, likely Stage A8 Persistent And Specialized
  Structures or a backlog item.

## Last Validation Snapshot

- Last C++ validation: 2026-06-21.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 713 Judge integration tests passed (~431 s; `A55`-`A60` each add
  1 catalog, +3 Judge, +3 stress). Each of `A55`-`A60` passed all four fixed
  tests; a 100-case differential sweep (seed 99) returned `AC` for all 6
  correct fixtures and `WA` for all 12 known-wrong fixtures.
- Judge runtime self-check (2026-06-21): `algo list` enumerates 128 problems
  (`F01`-`F68` and `A01`-`A60`);
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Known-wrong fixtures across the F-series and Stage A still produce reproducible
  non-AC verdicts. Stage A7 stress catches prefix-vs-exact confusion, duplicate
  erase mistakes, Radix partial-edge matching, XOR key/value confusion,
  overlapping and failure-link match loss, suffix-index base/RMQ errors, and
  Suffix Automaton empty-substring or suffix-only lookup mistakes.
- Known failing checks: none.
