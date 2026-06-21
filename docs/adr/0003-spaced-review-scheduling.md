---
type: adr
status: accepted
priority: p1
updated: 2026-06-21
context_policy: on_demand
owner: project
---

# ADR-0003: Spaced Review Scheduling

## Status

Accepted

## Context

Schema 1 records attempts but cannot answer what to practice next. The roadmap
requires simple 7-day and 30-day review spacing without coupling scheduling to
the future Web UI or relying on platform-specific timezone databases.

## Decision

Upgrade progress to schema 2 with `review_streak`, `last_attempt_day`, and
`next_review_day`, stored as UTC epoch-day integers. Schema 1 loads as streak 0
with no scheduled review and migrates on the next write.

An AC at streak 0 schedules +7 days; later ACs schedule +30 days. A non-AC
resets the streak and schedules +1 day. `algo review` lists records due on or
before the current UTC day. Scheduling remains in the progress domain so CLI
and future Web adapters share policy.

## Consequences

### Positive

- Deterministic, timezone-independent dates and straightforward migration.
- A small policy gives learners a useful next-practice queue immediately.

### Negative

- This is not a configurable or evidence-based adaptive scheduler.

### Neutral / Tradeoffs

- Additional review history can be added by a later schema migration.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| Local calendar dates | Familiar display | DST/timezone ambiguity across adapters | UTC epoch days are stable and portable |
| SM-2 style ease factors | Adaptive spacing | More state and policy complexity | The roadmap currently calls for 7/30-day scheduling |

## Security Review

- Trust boundary impact: no new path or execution boundary.
- Sensitive data impact: stores only integer schedule fields.
- Permission impact: unchanged `.judge/progress.json` write.
- Failure mode: invalid schema-2 dates/counts fail closed; schema 1 remains readable.

## Resource Impact

- Memory impact: three small fields per practiced problem.
- CPU impact: O(practiced problems) due filtering.
- I/O impact: unchanged whole-file atomic replacement.

## Rollback Plan

- Keep schema-2 fields but stop advancing them; older binaries will reject
  schema 2 rather than silently reinterpret it.
