---
type: release_policy
status: active
priority: p2
updated: 2026-06-13
context_policy: retrieve_when_planning
owner: project
---

# Release And Deployment

## Current Model

The project is local-first and has no deployed service. A release is a repository state that another
supported development environment can configure, build, test, and use reproducibly.

## Release Checklist

- [ ] Documentation guards pass.
- [ ] C++ build and CTest pass when C++ targets exist.
- [ ] Problem metadata and progress schema changes are documented.
- [ ] Known platform limitations are documented.
- [ ] Generated build, identity, temporary, and progress-private files are not committed.

## Validation Commands

```powershell
npm run docs:ready
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

## Rollback

- Revert the scoped release change without rewriting shared history.
- Restore the previous metadata schema and migrate local progress when a schema changed.
- Keep the prior CLI contract available until stored problem packages are migrated.
