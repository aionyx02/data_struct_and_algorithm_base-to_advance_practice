---
type: data_contracts
status: active
priority: p1
updated: 2026-06-13
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
| ProgressRecord | Attempts, best result, hint level, and review dates | Progress repository |

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

## Progress Schema Version 1

The canonical local store is `.judge/progress.json`:

```json
{
  "schema_version": 1,
  "records": [
    {
      "problem_id": "F03-stack-array",
      "attempts": 2,
      "accepted_attempts": 1,
      "best_time_ms": 42,
      "last_verdict": "WA"
    }
  ]
}
```

Records are sorted by `problem_id`; `best_time_ms` is `null` until the first
AC. Unknown fields, duplicate problem IDs, invalid counts, malformed JSON, and
unsupported schema versions are rejected explicitly.

## Migration Rules

- Metadata and progress records carry a schema version once persistence is implemented.
- Breaking schema changes require migration tests and an ADR.
- Unknown fields should be preserved or rejected explicitly; never silently reinterpret them.

## Cache Rules

- Cached binaries are keyed by source content, compiler identity, flags, and harness version.
- A stale or unverifiable cache entry is ignored.
- Cache deletion must remain safe and workspace-bounded.
