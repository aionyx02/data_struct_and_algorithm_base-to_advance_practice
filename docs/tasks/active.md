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

### TASK.CURRICULUM.008 - Expand Tree Traversal And Heap Views

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add traversal views, level-order representation, and heap construction/removal traces.
  - [ ] Keep traversal work scoped to core tree ADT observation.
  - [ ] Make visit order, queue state, and heap shape transitions observable.
  - [ ] Preserve explicit complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known visit-order and heap-transition mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
