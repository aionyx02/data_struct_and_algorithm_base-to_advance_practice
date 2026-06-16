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

### TASK.CURRICULUM.017 - Build Static Range Structures

- Status: doing
- Priority: P0
- Owner: shawn
- Progress: two of eight Stage A3 problems are implemented: the Sparse Table
  range-query tree (`A21`) and the Disjoint Sparse Table range sum (`A22`).
  `TASK.CURRICULUM.016` (Stage A2, `A09`-`A20`) completed on 2026-06-16.
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add eight exercises for the Stage A3 Static Range Structures family.
  - [x] Begin with the Sparse Table for idempotent range min/max.
  - [x] Add Disjoint Sparse Table.
  - [ ] Add Square Root Decomposition.
  - [ ] Add block decomposition with lazy tags.
  - [ ] Add Merge Sort Tree.
  - [ ] Add Wavelet Tree and Wavelet Matrix.
  - [ ] Add a bitset-based set representation.
  - [ ] Preserve observable query contracts and stated complexity limits.
- Validation:
  - [x] Known correct submission for `A21` passes deterministic and seeded
        stress tests.
  - [x] Known Sparse Table mistakes (dropping the right endpoint, using a
        single block) for `A21` receive non-AC verdicts.
  - [x] Known Disjoint Sparse Table mistakes (overlapping blocks that
        double-count sum, missing single-element case) for `A22` receive
        non-AC verdicts.
  - [ ] Later Stage A3 structures have correct and known-wrong fixtures.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
