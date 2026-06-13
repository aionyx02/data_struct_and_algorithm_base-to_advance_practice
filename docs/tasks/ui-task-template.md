---
type: task_template
status: template
priority: p2
updated: 2026-06-13
context_policy: on_demand
owner: project
---

# UI TASK.NNN - Task Title

## Goal

Describe what the learner should be able to view or do after this task.

## Scope

### In Scope

- Name the page, state, and interaction being changed.

### Out of Scope

- Do not change Judge rules or unrelated visual areas.

## Relevant Docs

- `docs/ui.md`
- `docs/html-guidelines.md`
- `docs/design-system.md`
- `docs/accessibility.md`
- Name the related ADR, or state that no ADR is required.

## Acceptance Criteria

- [ ] Semantic HTML is used correctly.
- [ ] Keyboard navigation and visible focus work.
- [ ] Responsive layout works.
- [ ] Loading, empty, error, and success states are handled.
- [ ] Judge behavior remains in the shared backend boundary.
- [ ] Relevant tests pass.

## Validation Commands

```powershell
npm run docs:refresh
npm run test --if-present
npm run build --if-present
```

## Risks

| Risk | Mitigation |
|---|---|
| UI duplicates Judge logic | Keep verdict and progress rules behind the application API |
