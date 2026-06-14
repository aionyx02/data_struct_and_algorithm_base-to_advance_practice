---
type: task_index
status: active
priority: p0
updated: 2026-06-14
context_policy: always_retrievable
owner: project
---

# Active Tasks

## Active Queue

### TASK.CURRICULUM.015 - Build Fenwick Tree Foundations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add eight exercises for the Stage A1 Fenwick Tree family.
  - [ ] Begin with point update and prefix/range query using explicit array storage.
  - [ ] Add range update variants, prefix lower bound, and frequency order statistics.
  - [ ] Add coordinate-indexed, two-dimensional, and sparse Fenwick representations.
  - [ ] Keep coordinate compression and unrelated search algorithms outside this batch.
  - [ ] Preserve observable update/query contracts and stated complexity limits.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known wrong index direction, off-by-one range, stale dual-tree,
        lower-bound, and sparse-storage mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
