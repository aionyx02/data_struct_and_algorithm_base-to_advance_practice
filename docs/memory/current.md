---
type: working_memory
status: active
priority: p0
updated: 2026-06-28
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
  the 171-problem catalog, statements, progress, review state, and browser-local
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

- Stages M1 (M01-M06), M2 (M07-M09), M3 (M10-M15), the M4 core
  (M16-M21: Lucas / exLucas, inclusion-exclusion, Catalan / Narayana / ballot,
  Stirling / Bell, and integer partitions), and the M5 core (M22-M27: rational
  OGF recurrence coefficients, Lagrange inversion, Newton identities, restricted
  cycle EGFs, formal composition, and FPS exp / log), plus the M6 linear-algebra
  core (M28-M39: modular Gaussian elimination, matrix power vector application,
  Berlekamp-Massey sequence prediction, determinant, matrix inverse, XOR linear
  basis, undirected Matrix-Tree, directed arborescences, BEST theorem counts,
  characteristic polynomials, trace powers, and determinant-lemma queries) are
  complete. Continue M6 with further linear-algebra applications if useful, or
  start the G-series algorithm track; both fall under `TASK.CURRICULUM.020`.

## Last Validation Snapshot

- Last C++ validation: 2026-06-28.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`,
  targeted M37-M39 CTest, full CTest, isolated reruns of transient DS failures,
  and `npm test`.
- Result: targeted M37-M39 CTest passed 21/21, including characteristic
  polynomial, Cayley-Hamilton trace power, and determinant-lemma fixed/judge/
  stress coverage. Full CTest now enumerates 1016 tests and reached 1011/1016
  because existing DS tests failed once under full-suite load; all five passed
  when rerun in isolation.
- Judge runtime self-check: catalog tests enumerate all 171 problems (132 DS +
  M01-M39 mathematics). Correct submissions verdict `AC`, known-wrong `WA`
  on both fixed tests and differential stress. M28-M39 stress oracles cover
  modular row reduction, matrix exponentiation, linear recurrence prediction,
  determinants, inverses, XOR linear basis behavior, Matrix-Tree cofactors,
  directed Laplacian orientation, BEST theorem factorial factors, characteristic
  polynomial coefficient order/sign, trace-power edge cases, and independent
  rank-one determinant updates independently of the optimized reference fixtures.
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
