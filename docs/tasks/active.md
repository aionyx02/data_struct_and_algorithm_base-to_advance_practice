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

### TASK.CURRICULUM.003 - Implement List Transformations And Node Management

- Status: todo
- Priority: P0
- Owner: shawn
- Started: not started
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Define stable IDs for list transformations and ownership-focused exercises.
  - [ ] Cover reversal, concatenation or splice, split, and node-reuse edge cases.
  - [ ] Keep the chapter focused on data-structure operations rather than general algorithms.
  - [ ] Require explicit complexity and ownership contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known cycle, lost-tail, or broken-backlink mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
