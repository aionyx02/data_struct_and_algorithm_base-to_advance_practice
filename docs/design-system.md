---
type: design_system
status: active
priority: p1
updated: 2026-06-13
context_policy: retrieve_when_planning
owner: project
---

# Design System

## Status

The Web phase has not started. These values are provisional constraints, not a commitment to a
framework or component library.

## Visual Direction

- Calm technical workspace focused on problem statements, code results, and learning progress.
- Prefer clarity, compact data tables, readable code, and visible verdict states over decoration.
- Color must not be the only signal for verdict or progress.

## Provisional Layout Tokens

| Token | Value |
|---|---|
| Page max width | 1440 px |
| Section spacing | 24 px |
| Card radius | 8 px |
| Input height | 40 px |

## Typography

| Use | Style |
|---|---|
| Page title | 28 px, semibold |
| Section title | 20 px, semibold |
| Body text | 16 px, regular |
| Helper text | 14 px, regular |
| Code and metrics | Readable monospace with tabular numbers where useful |

## Components

| Component | Rule |
|---|---|
| Button | Clear verb label, visible focus, disabled reason when unavailable |
| Card | Group one problem, result, or progress concept |
| Modal | Avoid for primary workflows; preserve focus when required |
| Form field | Persistent label, nearby validation, keyboard operable |
| Navigation | Reflect curriculum stage and preserve current problem context |
| Verdict badge | Include text and icon; never rely on color alone |

## Change Policy

Choose the actual framework, component library, and final tokens through a Web-phase ADR.
