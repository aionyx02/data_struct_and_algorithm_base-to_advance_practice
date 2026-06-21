---
type: working_memory
status: active
priority: p0
updated: 2026-06-21
context_policy: always_retrievable
owner: project
---

# Current Project Memory

## Current Strategy

- Keep the curriculum limited to data structures: 68 foundational and 64 advanced problems.
- Build the CLI Judge before the Web interface; both must share one Judge core.
- Expand the catalog in small prerequisite-ordered chapters.

## Current Focus

- Active priority: build the product layer on the completed 132-problem catalog.
- Current phase: atomic local progress persistence is complete. `algo test`
  records versioned attempt summaries through `ProgressRepository`, and
  `algo progress [problem-id]` reads them back. Automated tests opt out with
  `--no-progress`; stress runs never modify learner progress.
- Current owner / handoff state: no active task. `TASK.PROGRESS.001` completed
  with `shawn` on 2026-06-21.

## Important Constraints

- C++20, CMake, CLion, and MinGW g++ are the primary local toolchain.
- ADT implementation problems restrict STL containers that directly solve the exercise.
- The Judge is local-only and must not be treated as a secure untrusted-code service.
- General algorithms remain outside the current curriculum.

## Next Step

- Select the next product slice: spaced review scheduling or the Web stack ADR.

## Last Validation Snapshot

- Last C++ validation: 2026-06-21.
- Last test commands: `cmake --preset dev`, `cmake --build --preset dev`, and
  `ctest --preset dev`.
- Result: all 743 CTest tests passed (~521 s), including progress repository
  schema/round-trip tests and an isolated CLI AC→WA persistence flow.
- Judge runtime self-check: `algo list` enumerates all 132 problems.
  correct submissions verdict `AC`, known-wrong `WA`, broken sources `CE`, null
  dereference `RE`, infinite loop `TLE`, and out-of-root sources are refused by
  the project-root boundary. Stress differential testing returns `AC`/`WA`.
- Progress schema 1 rejects malformed JSON, unknown fields, duplicate IDs,
  invalid counts, and unsupported versions. Writes use a sibling temporary file
  and atomic replacement; no source code or diagnostics are persisted.
- Known failing checks: none.
