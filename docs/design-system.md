---
type: design_system
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_when_planning
owner: project
---

# Design System

## Status

The first Web workspace implements these tokens with native CSS. The direction
is intentionally compact and IDE-like, with an original split-pane composition
inspired by programming practice workspaces rather than a branded clone.

## Visual Direction

- Calm technical workspace focused on problem statements, code results, and learning progress.
- Prefer clarity, compact data tables, readable code, and visible verdict states over decoration.
- Color must not be the only signal for verdict or progress.

## Provisional Layout Tokens

| Token | Value |
|---|---|
| Desktop layout | 260-300 px rail, flexible statement, 330-390 px draft |
| Top bar | 54 px |
| Section spacing | 14-30 px according to density |
| Card radius | 7-8 px |
| Accent | Amber on charcoal; green for accepted state |

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
| Pane separator | Mouse/touch drag plus arrow keys; bounded widths and visible focus |
| Compile result | Text status plus bounded monospace diagnostics; never color alone |

## Change Policy

Choose the actual framework, component library, and final tokens through a Web-phase ADR.
