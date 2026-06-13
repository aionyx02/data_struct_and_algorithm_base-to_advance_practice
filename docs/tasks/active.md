---
type: task_index
status: active
priority: p0
updated: 2026-06-13
context_policy: always_retrievable
owner: project
---

# Active Tasks

## Active Queue

### TASK.CURRICULUM.004 - Complete List Representations

- Status: todo
- Priority: P0
- Owner: shawn
- Started: not started
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add sentinel-based and cursor-based linked-list representations.
  - [ ] Complete the ten-problem CH4 list allocation.
  - [ ] Compare pointer and array-index link representations explicitly.
  - [ ] Preserve lifecycle, boundary, and node-reuse coverage.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known sentinel or free-list mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
