---
type: adr
status: accepted
priority: p1
updated: 2026-06-13
context_policy: on_demand
owner: project
---

# ADR-0001: Local Judge Core And Interface Boundary

## Status

Accepted

## Context

The project will begin as a CLI learning tool and later add a Web interface. Correctness, limits,
invariants, forbidden APIs, and complexity evidence must remain identical across both interfaces.
The Judge also compiles and runs learner code, which creates an operating-system process boundary.

## Decision

- Build one framework-independent Judge domain core.
- Expose Judge use cases through an application layer.
- Implement CLI as the first interface adapter.
- Add Web only as a later adapter to the same application services.
- Keep compiler and process execution behind an infrastructure port.
- Run learner code in a child process with timeout and bounded output.
- Keep the initial product local-only; no network code-execution endpoint is allowed.

## Consequences

### Positive

- CLI and Web cannot silently diverge in verdict behavior.
- Process execution can be tested and replaced per operating system.
- Problem packages remain independent from presentation technology.
- The first vertical slice can stay small.

### Negative

- More interfaces and contracts are defined before the Web phase.
- Windows timeout and memory-limit behavior needs infrastructure-specific testing.

### Neutral / Tradeoffs

- Initial code may be slightly more verbose, but it avoids rebuilding the Judge when Web work starts.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| Put Judge logic directly in CLI commands | Fastest prototype | Hard to test and reuse; Web would duplicate behavior | Conflicts with the selected CLI-to-Web path |
| Build Web first | Immediate visual interface | Delays the correctness core and adds framework decisions early | Learning and judging behavior are the current priority |
| One shared Judge core with adapters | Reusable and testable | Requires explicit boundaries | Best fit for staged delivery |

## Implementation Notes

- The first slice should load one problem, compile one submission, run bounded tests, and return a structured report.
- Do not choose a Web framework or database during the first slice.
- Define stable metadata and verdict contracts before adding many problems.

## Rollback Plan

- Keep the domain contracts and replace adapters independently.
- If the layering proves excessive before public contracts exist, collapse only the application wrapper while preserving runner isolation.
