---
type: adr
status: accepted
priority: p1
updated: 2026-06-21
context_policy: on_demand
owner: project
---

# ADR-0002: Local Progress Persistence

## Status

Accepted

## Context

The curriculum and CLI Judge are complete, but attempts disappear when the
process exits. Progress must remain separate from version-controlled problem
packages, survive interruption without truncating the only record, and be
reusable by a future Web adapter without moving Judge rules into the UI.

## Decision

Store all local progress in `${projectRoot}/.judge/progress.json` with
`schema_version: 1` and a canonical array of per-problem records. A
`ProgressRepository` application/infrastructure boundary owns parsing,
validation, and updates. It rejects malformed, unsupported, or unknown fields.

Writes serialize the complete snapshot to a sibling temporary file, flush and
close it, then atomically replace the destination (`MoveFileExW` with replace
and write-through on Windows; filesystem rename on POSIX). `algo test` records
every completed Judge report; `algo progress [problem-id]` is read-only.

## Consequences

### Positive

- One explicit store can be backed up, inspected, migrated, or consumed by Web.
- Atomic replacement prevents a partial JSON file from becoming authoritative.
- The Judge service remains independent of persistence and interface concerns.

### Negative

- Whole-file rewrites are O(number of practiced problems).
- Concurrent writers are not coordinated in this single-user version.

### Neutral / Tradeoffs

- Spaced-review policy and hint history remain separate future schema work.

## Alternatives Considered

| Option | Pros | Cons | Reason not chosen |
|---|---|---|---|
| One file per problem | Small writes, isolated damage | More cleanup and migration complexity | 132 small files add coordination cost without current concurrency needs |
| SQLite | Transactions and queries | New dependency and deployment surface | Current data volume does not justify it |
| Append-only log | Natural attempt history | Compaction and recovery complexity | First slice needs current progress, not analytics history |

## Security Review

- Trust boundary impact: fixed project-controlled path under `.judge/`; no user-selected destination.
- Sensitive data impact: stores problem IDs, verdict counts, and timings only; never source code or diagnostics.
- Permission impact: creates `.judge/` and replaces one local file.
- Failure mode: invalid files fail closed with an actionable error; the prior file remains authoritative if replacement fails.

## Resource Impact

- Memory impact: O(practiced problems), bounded by the catalog size.
- CPU impact: negligible parsing and sorting after a Judge run.
- I/O impact: one small complete-file write per recorded attempt.

## Rollback Plan

- Remove CLI recording and the repository source, leaving the ignored progress
  file untouched for recovery. A later implementation can migrate schema 1.
