---
type: task_template
status: template
priority: p2
updated: 2026-06-13
context_policy: on_demand
owner: project
---

# TASK.NNN - Task Title

## Goal

Describe the observable result that should be true when the task is complete.

## Non-Goals

- List behavior or refactors that are intentionally excluded.

## Scope

### In Scope

- List one concrete deliverable per item.

### Out of Scope

- List adjacent work that should remain unchanged.

## Relevant Docs

- Link only the documents required for this task.

## Implementation Plan

1. Inspect the existing boundary and tests.
2. Implement the smallest end-to-end change.
3. Verify behavior and update the matching state documents.

## Acceptance Criteria

- [ ] Observable behavior and failure paths are covered.
- [ ] Relevant validation commands pass.

## Validation Commands

```powershell
npm run docs:refresh
```

Add CMake, CTest, CLI, or benchmark commands required by the task.

## Risks

| Risk | Mitigation |
|---|---|
| Scope or contract drift | Keep changes behind the documented module boundary and add contract tests |

## Handoff Notes

- Record only information needed by the next contributor; detailed narrative belongs in a session log.
