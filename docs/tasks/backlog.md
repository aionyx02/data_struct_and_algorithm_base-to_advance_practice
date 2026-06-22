---
type: task_index
status: backlog
priority: p2
updated: 2026-06-21
context_policy: on_demand
owner: project
---

# Backlog

## Judge Foundation

- [ ] Define versioned problem metadata and Judge report schemas.
- [ ] Add deterministic, random, differential, invariant, and benchmark test layers.
- [ ] Investigate reliable Windows memory limiting.
- [x] Add atomic local progress persistence.
- [ ] Add 90/120-minute assessment mode.

## Curriculum

- [x] Implement the 68 foundational data-structure problems.
- [x] Implement the 64 advanced data-structure problems.
- [x] Add 7-day and 30-day spaced review scheduling.
- [ ] Add operation-count instrumentation for complexity evidence.

## Future Web Phase

- [x] Select a Web stack through an ADR when CLI coverage reaches 25 stable problems.
- [x] Build read-only problem and progress views first.
- [ ] Add local editing and submission only after the Judge API boundary is stable.

## Algorithm & Math Curriculum (gated by ADR-0006, proposed)

Planned in `docs/algorithm-roadmap.md` (G1–G15) and `docs/math-roadmap.md`
(M0–M12). Study starts now on external judges; the items below need ADR-0006
moved to `accepted` before any local Judge implementation.

- [ ] Accept ADR-0006 to open algorithm/math problem families in the local Judge.
- [ ] Promote first exact-match-friendly families (G6/M1–M4 number theory,
      G3/G10 DP counting) into Judge problems with differential oracles.
- [ ] Follow-up ADR for non-deterministic outputs: geometry float tolerance
      (G12/M11), special-judge for construction (G14), interactive runner (G14),
      heuristic scorer (G13).

## Parking Lot

- Full ICPC/IOI training is now planned (see above, gated by ADR-0006); until
  that ADR is accepted, `docs/project.md` non-goals keep general algorithms out
  of the local Judge.
