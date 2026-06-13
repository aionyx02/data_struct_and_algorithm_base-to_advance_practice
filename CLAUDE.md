---
type: agent_bootstrap
status: active
priority: p0
updated: 2026-06-13
context_policy: always_retrievable
owner: project
---

# CLAUDE.md

> Auto-loaded at session start. Detailed governance, ADR, and documentation-routing rules are in `docs/CLAUDE.md`.

## Session Start

1. Run `npm run team:status` when Node scripts are available.
2. If no local identity is set, ask the human to run `npm run team:whoami -- <member-id>` before starting task work.
3. Read `docs/team/members.md` for registered members.
4. Read `docs/index.md` for documentation routing.
5. Read `docs/memory/current.md` for current strategy, constraints, and next step.
6. Read `docs/tasks/active.md` for the active queue and only work on tasks owned by the current identity unless explicitly reassigned.
7. Retrieve additional documents by intent. For planning, implementation, refactor, or architecture work, read `docs/engineering-principles.md`. Do not recursively load all docs.

## Session Close

Before final response, handoff, or commit:

1. Update only the smallest matching state document.
2. Put detailed execution notes, debugging narrative, and command-output history in `docs/memory/sessions/YYYY-MM-DD.md`.
   - For team workflows, use per-member session files created by `npm run docs:new-session`.
3. Keep `docs/memory/current.md` and `docs/tasks/active.md` as current-state indexes only.
4. Put completed-task detail in the session log using `## COMPLETED: TASK_ID - summary`.
5. Run `npm run docs:refresh` when Node scripts are available.

## Project Overview

- Product: Data Structures Practice
- Primary goal: Build a progressive C++20 data-structure curriculum with a local Judge and a later Web practice interface.
- Main users: Shawn and AI coding collaborators working in this repository.
- Supported platforms: Windows 11, CLion, CMake, and MinGW g++; CI documentation checks run on GitHub Actions.

## Git Workflow Rules

```text
main <- stable project state
feature/<name> <- scoped work branches when needed
```

- Do not merge without explicit developer confirmation.
- Create feature branches from `main` unless the repository policy changes.
- Show diffs and pass relevant checks before merge.
- Never rewrite shared history without explicit approval.

## Build / Test Commands

Replace commands to match this project.

```bash
npm install
npm run lint
npm run security:scan
npm test
npm run team:guard
npm run docs:refresh
npm run docs:ready

cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Documentation Entry Points

- `docs/index.md` - documentation router
- `docs/project.md` - stable project facts
- `docs/team/members.md` - fake team registry and task owner IDs
- `docs/memory/current.md` - short working memory
- `docs/tasks/active.md` - active work only
- `docs/engineering-principles.md` - coding style, planning priorities, and decoupled architecture rules
- `docs/CLAUDE.md` - governance and ADR rules
