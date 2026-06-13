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

### TASK.CURRICULUM.007 - Build Core Tree Representations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add array and linked binary-tree representations, a threaded tree, and a binary heap.
  - [ ] Keep the slice focused on data-structure representation and core ADT operations.
  - [ ] Make parent/child indexing, links, threads, and heap shape invariants observable.
  - [ ] Preserve explicit complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known index, link, thread, and heap-invariant mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
