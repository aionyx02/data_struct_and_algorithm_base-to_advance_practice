---
type: data_contracts
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_when_planning
owner: project
---

# Data Contracts

## Core Entities

| Entity | Meaning | Owner |
|---|---|---|
| ProblemMetadata | Stable problem identity, limits, stage, difficulty, contracts, and restrictions | Problem catalog |
| TestPlan | Ordered deterministic, random, invariant, and benchmark checks | Judge domain |
| RunResult | Exit state, elapsed time, memory evidence, and bounded diagnostics | Runner adapter |
| JudgeReport | Verdict plus per-layer evidence | Judge domain |
| ProgressRecord | Attempts, verdicts, best time, and review schedule | Progress repository |

## Verdict Contract

Supported verdicts are `AC`, `WA`, `TLE`, `MLE`, `RE`, `CE`, `API`, `INV`, and `CX`.
`CX` means complexity evidence is suspicious and requires review; it is not a mathematical proof of failure.

## Persistence Rules

- Problem metadata is version-controlled with each problem package.
- Local progress is not mixed into problem definitions.
- Progress writes must be atomic enough to avoid truncating the only record.
- Random-test failures retain the seed and minimized operation sequence when available.
- Before minimization is implemented, stress failures retain the complete generated input.
- Generated build files and temporary submissions are not committed.

## Progress Schema Version 2

The canonical local store is `.judge/progress.json`:

```json
{
  "schema_version": 2,
  "records": [
    {
      "problem_id": "F03-stack-array",
      "attempts": 2,
      "accepted_attempts": 1,
      "best_time_ms": 42,
      "last_verdict": "WA",
      "review_streak": 0,
      "last_attempt_day": 20625,
      "next_review_day": 20626
    }
  ]
}
```

Records are sorted by `problem_id`; `best_time_ms` is `null` until the first
AC. Review dates are UTC epoch-day integers. A non-AC resets `review_streak`
and schedules the next day, the first AC schedules seven days later, and later
ACs schedule 30 days later. Unknown fields, duplicate problem IDs, invalid
counts, malformed JSON, and unsupported schema versions are rejected explicitly.

## Migration Rules

- Schema 1 progress records remain readable. Their review fields default to
  streak 0 and no dates, then the next attempt writes the complete schema 2 record.
- Metadata and progress records carry a schema version once persistence is implemented.
- Breaking schema changes require migration tests and an ADR.
- Unknown fields should be preserved or rejected explicitly; never silently reinterpret them.

## Cache Rules

- Cached binaries are keyed by source content, compiler identity, flags, and harness version.
- A stale or unverifiable cache entry is ignored.
- Cache deletion must remain safe and workspace-bounded.
