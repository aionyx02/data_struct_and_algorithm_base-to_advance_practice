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
- Progress: six of twelve Stage A2 problems are implemented: recursive and
  iterative range sum, minimum/maximum and GCD monoids, plus isolated range-add
  and range-assign lazy propagation (`A09`-`A14`).
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add twelve exercises for the Stage A2 Segment Tree family.
  - [x] Begin with recursive and iterative point-update range-query trees.
  - [x] Cover sum, minimum, maximum, and GCD monoids.
  - [x] Add isolated range-add and range-assign lazy propagation.
  - [ ] Add mixed lazy propagation composition order.
  - [ ] Add positional search, dynamic, persistent, order-statistic, and Beats variants.
  - [ ] Preserve observable update/query contracts and stated complexity limits.
- Validation:
  - [x] Known correct submissions for `A09`-`A14` pass deterministic and seeded
        stress tests.
  - [x] Known wrong endpoint, stale-parent, invalid identity, and no-push lazy
        mistakes for `A09`-`A14` receive non-AC verdicts.
  - [ ] Known mixed-tag, persistence, pruning, and Beats
        mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
