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

### TASK.CURRICULUM.012 - Build Hash Table Foundations

- Status: todo
- Priority: P0
- Owner: shawn
- Related docs:
  - `docs/learning-roadmap.md`
  - `docs/judge-requirements.md`
- Acceptance criteria:
  - [ ] Add seven exercises to complete the CH8 Hashing allocation.
  - [ ] Cover direct bucket mapping and collision observability.
  - [ ] Cover linear probing, quadratic probing, and double hashing.
  - [ ] Cover deletion tombstones and separate chaining.
  - [ ] Make load factor and rehash transitions observable.
  - [ ] Preserve explicit average/worst-case complexity and storage contracts.
- Validation:
  - [ ] Known correct submissions pass deterministic and seeded stress tests.
  - [ ] Known collision overwrite, broken probe, tombstone, and stale rehash
        mistakes receive non-AC verdicts.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
