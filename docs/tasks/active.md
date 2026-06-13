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

### TASK.CURRICULUM.011 - Build Graph ADT Representations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add five exercises to complete the CH6 Graph ADT allocation.
  - [ ] Cover adjacency matrix and adjacency list representations.
  - [ ] Cover directed and undirected edge insertion, removal, and lookup.
  - [ ] Make degree and ordered neighbor views observable.
  - [ ] Keep shortest paths, MST, topological sort, and other graph algorithms deferred.
  - [ ] Preserve explicit complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known asymmetric edge, duplicate edge, and stale degree mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
