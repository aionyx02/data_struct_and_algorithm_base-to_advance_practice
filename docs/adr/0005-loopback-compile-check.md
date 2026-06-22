---
type: adr
status: accepted
priority: p1
updated: 2026-06-21
context_policy: on_demand
owner: project
---

# ADR-0005: Loopback Compile Check

## Status

Accepted

## Context

The native Web workspace keeps C++ drafts in browser storage and currently
requires copying a CLI command before receiving any feedback. A one-click
compile check is useful while editing, but it must not become a second Judge,
execute learner binaries, or expose a remote code-processing service.

## Decision

Extend ADR-0004 with one loopback-only `POST /api/compile` endpoint. It accepts
a known problem ID and at most 128 KiB of C++ source, then starts the configured
local compiler without a shell using `-std=c++20 -fsyntax-only -x c++ -` and
streams the source over stdin. Only one compile runs at a time; the process has
a 20-second timeout and 64 KiB diagnostic cap.

The result is only `compiled` or `compile_error` evidence. It is not a Judge
verdict, does not run the produced program, does not write the source to disk,
and does not update progress. Full correctness remains behind the CLI Judge
until a shared write adapter is designed.

The same UI slice adds keyboard-accessible desktop pane separators. Sidebar and
draft widths are browser-local preferences with bounded minimums; narrow-screen
drawers remain unchanged.

## Consequences

### Positive

- Editing gets immediate syntax/type feedback with one click or `Ctrl+Enter`.
- No learner executable is produced or launched.
- Resizable panes support both statement-heavy and code-heavy workflows.

### Negative

- The Node adapter now owns one bounded compiler child-process path.
- Syntax success still says nothing about correctness or forbidden APIs.

### Neutral / Tradeoffs

- A future submission endpoint should replace this transport with a shared
  Judge application adapter, not expand the compile check into another Judge.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| Run `algo test` from the browser | Full verdict immediately | Executes code and mutates progress through a new write boundary | Requires a larger security and API design |
| Compile to a temporary executable | Closer to Judge compilation | Leaves an artifact and expands cleanup requirements | `-fsyntax-only` provides the requested feedback |
| Browser-only C++ parser | No local process | Would diverge from the configured compiler | Native compiler feedback is authoritative for syntax/type checks |

## Security Review

- Trust boundary impact: one POST route on `127.0.0.1`; no remote bind.
- Input bounds: JSON body and source length are capped.
- Process impact: compiler is spawned directly with fixed arguments and no shell.
- Output impact: diagnostics are bounded and returned as plain text.
- Persistence impact: no source, binary, verdict, or progress write.

## Resource Impact

- Memory impact: bounded request and diagnostic buffers.
- CPU impact: at most one local compiler process per request.
- I/O impact: compiler reads source from stdin; no temporary source or binary.

## Rollback Plan

Remove the compile route and UI result panel. Browser drafts, read-only APIs,
and CLI Judge remain unaffected.
