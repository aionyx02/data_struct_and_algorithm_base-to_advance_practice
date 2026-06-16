---
type: task_index
status: active
priority: p0
updated: 2026-06-16
context_policy: always_retrievable
owner: project
---

# Active Tasks

## Active Queue

### TASK.CURRICULUM.016 - Build Segment Tree Foundations

- Status: doing
- Priority: P0
- Owner: shawn
- Progress: ten of twelve Stage A2 problems are implemented: recursive and
  iterative range sum, minimum/maximum and GCD monoids, plus isolated range-add
  and range-assign lazy propagation, mixed lazy tag composition, and
  positional search, dynamic sparse Segment Tree, and persistent versions
  (`A09`-`A18`).
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add twelve exercises for the Stage A2 Segment Tree family.
  - [x] Begin with recursive and iterative point-update range-query trees.
  - [x] Cover sum, minimum, maximum, and GCD monoids.
  - [x] Add isolated range-add and range-assign lazy propagation.
  - [x] Add mixed lazy propagation composition order.
  - [x] Add positional search.
  - [x] Add dynamic sparse Segment Tree.
  - [x] Add persistent Segment Tree.
  - [ ] Add order-statistic and Beats variants.
  - [ ] Preserve observable update/query contracts and stated complexity limits.
- Validation:
  - [x] Known correct submissions for `A09`-`A18` pass deterministic and seeded
        stress tests.
  - [x] Known wrong endpoint, stale-parent, invalid identity, and no-push lazy
        mistakes for `A09`-`A14` receive non-AC verdicts.
  - [x] Known mixed-tag composition mistakes for `A15` receive non-AC verdicts.
  - [x] Known positional-search boundary mistakes for `A16` receive non-AC verdicts.
  - [x] Known dynamic allocation mistakes for `A17` receive non-AC verdicts.
  - [x] Known persistence mistakes for `A18` receive non-AC verdicts.
  - [ ] Known order-statistic and Beats mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
