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

- The 132-problem data-structure catalog (68 foundational + 64 advanced) is complete.
- Scope expanded to algorithms (G-series) and mathematics (M-series) per ADR-0006
  (accepted 2026-06-22); reuse the same Judge core and differential-stress pattern.
- Promote exact-match-friendly families first (number theory, counting DP);
  defer non-deterministic output (geometry float, construction, interactive) to
  follow-up ADRs.
- Expand the catalog in small prerequisite-ordered chapters.

## Current Focus

- Active priority: build the product layer on the completed 132-problem catalog.
- Current phase: the read-only Web workspace is complete. `npm run web` serves
  the 132-problem catalog, statements, progress, review state, and browser-local
  drafts through a loopback-only native Web UI. Desktop panes are resizable,
  and a bounded syntax-only compile check returns local compiler diagnostics.
- Current owner / handoff state: no active task. `TASK.WEB.002` completed with
  `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Stages M1 (M01-M06) and M2 (M07-M09: discrete log, primitive root, quadratic
  residue) are complete. Continue the math/algorithm curriculum with the next
  `math-roadmap.md` stage (M3 multiplicative-function sieves) or start the
  G-series algorithm track; both fall under `TASK.CURRICULUM.020`.

## Last Validation Snapshot

- Last C++ validation: 2026-06-22.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: 806 CTest tests, including the nine M-series number-theory problems
  (M01-M09) catalog/judge/stress coverage and the statically linked progress
  repository round-trip. Two full runs each surfaced one different heavy DS test
  (A54 link-cut-tree; rehashing-linear-probing stress) TLE-flaking under parallel
  load; both pass in isolation. Pre-existing load sensitivity, untouched by
  M-series; all M-series and targeted runs are deterministically green.
- Judge runtime self-check: `algo list` enumerates all 141 problems (132 DS +
  M01-M09 number theory). Correct submissions verdict `AC`, known-wrong `WA`
  on both fixed tests and differential stress.
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Progress schema 2 reads schema 1, migrates on write, and rejects malformed
  JSON, unknown fields, duplicate IDs, invalid counts, and unsupported versions.
  Writes remain atomic; no source code or diagnostics are persisted.
- Web validation: 16/16 Node tests passed; localhost browser checks covered the
  real catalog, mouse/keyboard resizing, persisted widths, successful and failed
  real compiler feedback, mobile behavior, accessible labels, and a clean console.
- Known failing checks: none.
