---
type: adr
status: accepted
priority: p1
updated: 2026-06-21
context_policy: on_demand
owner: project
---

# ADR-0004: Local Read-Only Web Adapter

## Status

Accepted

## Context

The CLI now covers 132 stable problems, progress persistence, and scheduled
reviews, so the Web-phase trigger has been met. The first UI must expose those
existing contracts without creating a second Judge or requiring a framework,
database, or remote service before the interaction model is proven.

## Decision

Build the first Web slice with Node.js 20 standard-library HTTP APIs and native
HTML, CSS, and ES modules. The server binds to `127.0.0.1`, serves only an
explicit static asset allowlist, and exposes read-only catalog, problem detail,
progress, and review endpoints backed by repository files.

The workspace uses an original dense split-pane layout inspired by programming
practice sites: searchable problem rail, statement pane, and local draft/status
pane. Browser drafts may use `localStorage`; repository mutation, submission,
and verdict decisions remain outside this slice. A later adapter must call the
shared Judge boundary rather than reimplement judging in JavaScript.

## Consequences

### Positive

- Zero new runtime dependencies or lockfile churn.
- A real 132-problem UI can be tested before committing to a component stack.
- Read-only exposure keeps the local execution trust boundary unchanged.

### Negative

- Native rendering utilities are less ergonomic than a mature framework.
- File-backed API reads are intentionally local and single-user.

### Neutral / Tradeoffs

- A later framework can replace the frontend without changing endpoint shapes.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| React + Vite | Mature ecosystem and fast component development | Adds dependencies before the UI contract is stable | Defer until component complexity proves the need |
| C++ HTTP server | Direct access to domain objects | Adds HTTP parsing/security scope to the Judge binary | Keep transport out of the Judge executable |
| Static mock data | Fastest visual prototype | Not connected to the real catalog or progress | The requested frontend should be useful immediately |

## Security Review

- Trust boundary impact: loopback-only read access to catalog and progress data.
- Sensitive data impact: no source files, environment variables, or diagnostics
  are exposed.
- Permission impact: no repository write endpoint exists.
- Failure mode: unknown routes and malformed repository JSON fail closed.

## Resource Impact

- Memory impact: the 132-entry catalog is cached in one local Node process.
- CPU impact: negligible filtering and Markdown rendering.
- I/O impact: progress is reread per API request so Judge writes remain visible.

## Rollback Plan

Remove the `web/` assets and local server script. CLI Judge and progress files
remain unchanged.
