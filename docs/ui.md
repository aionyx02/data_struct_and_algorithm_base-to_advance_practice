---
type: ui_spec
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_when_planning
owner: project
---

# UI Architecture

## Status

The first UI is active after the CLI reached 132 stable problems. ADR-0004
selects a dependency-free local Node.js adapter with native HTML, CSS, and ES
modules; ADR-0005 adds resizable panes and compile-only feedback. Submission
and result history remain planned.

## UI Goals

- Make curriculum position, constraints, verdict evidence, and review status easy to understand.
- Display Judge reports without recomputing verdicts.
- Keep code, tests, and failure evidence accessible by keyboard.
- Support Traditional Chinese problem content and English technical identifiers.

## Planned Areas

| Page / Area | Responsibility | Notes |
|---|---|---|
| Problem catalog | Filter by stage, topic, difficulty, and review state | Implemented read-only |
| Problem workspace | Show statement, resizable browser-local draft, compile diagnostics, and CLI command | Compile-only feedback implemented |
| Result report | Present verdict, failed layer, timing, operations, and seed | Data comes from Judge API |
| Progress dashboard | Show per-problem attempts, verdict, best time, and scheduled review | Initial signals implemented |
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

ADR-0004 covers the initial no-framework editor, styling, HTTP transport, and
loopback exposure model. Create another ADR before adding a framework, remote
exposure, repository writes, or a browser-triggered Judge process. ADR-0005
covers only syntax compilation and explicitly does not authorize execution.
