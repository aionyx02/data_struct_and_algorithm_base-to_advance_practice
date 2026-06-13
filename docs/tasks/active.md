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

### TASK.CURRICULUM.010 - Complete Forest And Union-Find Foundations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add six exercises to complete the CH5-2 allocation.
  - [ ] Cover parent-array and child-sibling forest representations.
  - [ ] Cover basic union, weighting, and path compression.
  - [ ] Make root, depth, component size, and parent transitions observable.
  - [ ] Preserve explicit complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known stale-root, reversed-weight, and missing-compression mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
