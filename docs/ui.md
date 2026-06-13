---
type: ui_spec
status: active
priority: p1
updated: 2026-06-13
context_policy: retrieve_when_planning
owner: project
---

# UI Architecture

## Status

The UI is deferred until the CLI Judge supports at least 25 stable problems. This document records
boundaries so early backend work does not assume a specific frontend.

## UI Goals

- Make curriculum position, constraints, verdict evidence, and review status easy to understand.
- Display Judge reports without recomputing verdicts.
- Keep code, tests, and failure evidence accessible by keyboard.
- Support Traditional Chinese problem content and English technical identifiers.

## Planned Areas

| Page / Area | Responsibility | Notes |
|---|---|---|
| Problem catalog | Filter by stage, topic, difficulty, and review state | Read-only first |
| Problem workspace | Show statement, limits, starter code, and submission controls | Local-only |
| Result report | Present verdict, failed layer, timing, operations, and seed | Data comes from Judge API |
| Progress dashboard | Show completion, weak topics, and scheduled reviews | No social features initially |
| Assessment view | Run a 90/120-minute data-structure assessment | Not a full ICPC scoreboard |

## Component Boundary Rules

- UI components do not decide verdicts, complexity classes, or review schedules.
- Shared components belong in the frontend shared directory selected during Web initialization.
- Page-specific components stay near the page.
- API adapters map stable application contracts to view models.

## State Rules

| State Type | Preferred Location |
|---|---|
| Local interaction state | Component |
| Form and editor state | Workspace feature |
| Judge and progress data | Query/data layer |
| Global application state | Only for cross-page session needs |

## Decision Triggers

Create an ADR before selecting the Web framework, editor, styling system, state library, HTTP transport,
or local service exposure model.
