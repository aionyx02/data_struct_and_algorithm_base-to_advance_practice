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

- Stage M1 number-theory foundations (M01-M06) is complete. Continue the
  math/algorithm curriculum with the next `math-roadmap.md` stage (M2 congruence
  theory: CRT chains, BSGS discrete log, Tonelli-Shanks) or start the G-series
  algorithm track; both fall under `TASK.CURRICULUM.020`.

## Last Validation Snapshot

- Last C++ validation: 2026-06-22.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 785 CTest tests passed (~476 s single-worker), including the six
  M-series number-theory problems (M01-M06) catalog/judge/stress coverage and
  the statically linked progress repository round-trip.
- Judge runtime self-check: `algo list` enumerates all 138 problems (132 DS +
  M01-M06 number theory). Correct submissions verdict `AC`, known-wrong `WA`
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
