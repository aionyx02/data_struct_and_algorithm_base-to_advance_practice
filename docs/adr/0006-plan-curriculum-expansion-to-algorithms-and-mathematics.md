---
type: adr
status: accepted
priority: p1
updated: 2026-06-22
context_policy: on_demand
owner: project
---

# ADR-0006: Plan curriculum expansion to algorithms and mathematics

## Status

Accepted (2026-06-22, by developer shawn). The scope gate is lifted: algorithm
and mathematics families may now be implemented as local Judge problems.
Non-deterministic-output families (geometry float tolerance, construction
special-judge, interactive runner, heuristic scorer) still require their own
follow-up implementation ADR before implementation, per the Decision below.

## Context

The project is documented as data-structures-only. `docs/project.md` non-goals
state that "general algorithm training is outside the current scope" and that
"the current phase does not include dynamic programming, number theory, geometry,
or a full ICPC curriculum." `docs/project.md` §Current Strategy keeps the
curriculum "limited to data structures **until the user explicitly expands
scope**."

The 132-problem data-structure curriculum (F-series + A-series) and the
read-only Web workspace are now complete. The developer has explicitly asked to
plan a full competitive-programming curriculum: algorithms from foundation to
international-contest (ICPC World Finals / IOI) level, plus a complete number
theory and mathematics track, and to design these so the topics can later become
local Judge problems (not only an external study roadmap).

This is the explicit scope-expansion event that `docs/project.md` anticipated.
Two existing roadmap docs are relevant: `docs/algorithm-roadmap.md` (an external
study roadmap that today caps at ICPC regional level and adds no Judge problems)
and `docs/learning-roadmap.md` (the data-structure curriculum that the Judge
implements). Expanding the Judge to general algorithms and math touches problem
catalog scope, generator/oracle strategy, and the difficulty model, so it
warrants a recorded decision before any implementation.

## Decision

Adopt, **at planning level only**, an expansion of the learning scope beyond data
structures to include general algorithms and mathematics, captured in:

- `docs/algorithm-roadmap.md` — extended with an international tier (world-finals
  / IOI) above the existing regional ceiling.
- `docs/math-roadmap.md` (new) — a dedicated number-theory and mathematics track,
  foundation to advanced.

Both roadmaps remain **study-first**: practice continues on external judges
(Codeforces, AtCoder, Library Checker, etc.) and **no local Judge problem is
added under this ADR**. The roadmaps additionally annotate which topics are
candidates to become local Judge problems in the future.

Promoting any algorithm or math topic into the local Judge (new problem family,
generator/oracle, CMake/CTest wiring) requires this ADR to be moved to
`accepted` by the developer, and may require a follow-up implementation ADR for
specific contract or architecture changes (e.g. a new oracle strategy for
non-deterministic outputs, floating-point geometry tolerance, or interactive
problems). Until then `docs/project.md` non-goals stay in force.

## Consequences

### Positive

- Records the deliberate move past the documented DS-only boundary with an audit
  trail, instead of silently contradicting `docs/project.md`.
- Gives a single planning reference for foundation→international algorithm and
  math study, reusing the established stage/difficulty/acceptance format.
- Lets study begin immediately on external judges with zero code or
  architecture risk.

### Negative

- Widens the long-term surface the project may eventually need to maintain
  (algorithm + math problem families, new oracle strategies).
- Some topics (geometry, interactive, heuristic/output-only) do not fit the
  current exact-match differential Judge and would each need their own design.

### Neutral / Tradeoffs

- The roadmaps are documentation; they impose no build or runtime cost until a
  topic is promoted to the Judge under a separate accepted decision.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| Plan + implement Judge problems now in one step | Fastest to playable | Bundles a large scope change with unreviewed oracle/contract work | Violates ADR-first and smallest-safe-change principles |
| Keep DS-only, refuse expansion | No scope creep | Ignores the developer's explicit request | Scope expansion was explicitly requested |
| Plan as external study roadmap only, never Judge | Zero project risk | Forecloses the requested "toward Judge" direction | Developer chose to design toward future Judge problems |

## Security Review

- Trust boundary impact: none at planning stage. Future geometry/number-theory
  Judge problems keep the existing local-only, project-root-bounded execution
  model; interactive problems (if ever added) would need a new boundary review.
- Sensitive data impact: none; roadmaps contain no secrets.
- Permission impact: none.
- Failure mode: a roadmap topic is promoted to the Judge without its own ADR —
  mitigated by the explicit gate in this Decision.

## Resource Impact

- Memory impact: none (documentation only).
- CPU impact: none until topics are promoted; future math/geometry generators
  may need `__int128`, FFT-sized inputs, or floating-point tolerance budgets,
  to be sized per problem.
- I/O impact: none.

## Rollback Plan

- Revert the two roadmap docs and this ADR; `docs/project.md` non-goals already
  describe the prior DS-only state, so no state migration is required. No code
  ships under this ADR, so rollback is a documentation revert only.
