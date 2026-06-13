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

### TASK.CURRICULUM.009 - Complete BST Foundations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add six BST exercises to complete the 14-problem CH5-1 allocation.
  - [ ] Cover insert/search, deletion cases, predecessor/successor, range views, and validation.
  - [ ] Make parent links and replacement-node transitions observable.
  - [ ] Preserve explicit complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known duplicate, deletion, and ordering mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
