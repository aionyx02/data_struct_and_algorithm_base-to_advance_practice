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

### TASK.CURRICULUM.013 - Build Advanced Search Tree Foundations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add seven exercises to complete the CH10 Search Trees allocation.
  - [ ] Cover AVL single/double rotations, insertion, deletion, and validation.
  - [ ] Cover fixed-order multiway search-tree node representation.
  - [ ] Cover red-black insertion transitions and structural validation.
  - [ ] Keep dynamic-programming optimization and general search algorithms deferred.
  - [ ] Preserve explicit height, update, and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known wrong rotation, stale height, broken parent link, and red-black
        invariant mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
