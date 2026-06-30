---
type: working_memory
status: active
priority: p0
updated: 2026-06-30
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

- Active priority: extend the algorithm and mathematics curriculum on the stable
  Judge and product layers.
- Current phase: the read-only Web workspace is complete. `npm run web` serves
  the 178-problem catalog, statements, progress, review state, and browser-local
  drafts through a loopback-only native Web UI. Desktop panes are resizable,
  and a bounded syntax-only compile check returns local compiler diagnostics.
- Current owner / handoff state: `TASK.CURRICULUM.020` is active with `shawn`.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- The G-series algorithm track is admitted by ADR-0006 but has not been
  implemented yet.

## Next Step

- M1-M5 are complete through M27. The M6 linear-algebra family is complete
  through M46, covering Gaussian elimination, matrix powers and recurrences,
  determinants/inverses/bases, Matrix-Tree and BEST counts, characteristic and
  trace identities, determinant/update lemmas, Woodbury, and Schur complement
  determinant/solve/inverse/rank variants. Continue M6 with further linear-algebra
  applications if useful, or start the G-series algorithm track; both fall under
  `TASK.CURRICULUM.020`.

## Last Validation Snapshot

- Last C++ validation: 2026-06-30.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`,
  targeted M40-M46 CTest, and `npm test`.
- Result: targeted M40-M46 CTest passed 49/49, covering catalog listing, fixed
  judge fixtures, and differential stress for correct and known-wrong update,
  Woodbury, and Schur complement submissions. Node tests passed 17/17.
- Judge runtime self-check: catalog tests enumerate all 178 problems (132 DS +
  M01-M46 mathematics). Correct submissions verdict `AC`, known-wrong `WA`
  on both fixed tests and differential stress. M28-M44 stress oracles cover the
  linear-algebra family independently of optimized reference fixtures, including
  row reduction, recurrences, graph determinants, characteristic/trace identities,
  rank-one and low-rank updates, Woodbury solves, and Schur complement
  determinant/solve/inverse/rank variants.
  Broken sources verdict `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Progress schema 2 reads schema 1, migrates on write, and rejects malformed
  JSON, unknown fields, duplicate IDs, invalid counts, and unsupported versions.
  Writes remain atomic; no source code or diagnostics are persisted.
- Web validation: 17/17 Node tests passed; localhost browser checks covered the
  domain-aware catalog and mathematics / number-theory filtering.
- Known deterministic failing checks: none. Existing full-suite DS transients
  remain possible and are tracked separately from the M-series work.
