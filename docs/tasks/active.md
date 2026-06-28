---
type: task_index
status: active
priority: p0
updated: 2026-06-28
context_policy: always_retrievable
owner: project
---

# Active Tasks

## Active Queue

- TASK.CURRICULUM.020 — Implement M-series mathematics Judge family (stage M).
  Owner: shawn. Status: in progress. Stages M1 (M01-M06), M2 (M07-M09), M3
  (M10-M15), the M4 core (M16-M21), and the M5 generating-function core
  (M22-M27), plus the M6 linear-algebra core through characteristic polynomial /
  trace-power / determinant-lemma coverage (M28-M39), complete. Targeted
  M37-M39 CTest and Node tests are green; full CTest reached 1011/1016 only
  because existing DS transients failed once and passed in isolation. Next:
  continue M6 with further linear-algebra applications if useful, or start the
  G-series algorithm track.

## Strategy

Keep this file as a compact queue. Detailed execution and command output belong in member session logs.

## Next Phase Candidates

- Add progress persistence after the first Judge slice is stable.
- Start the Web dashboard after at least 25 CLI problems are reliable.
