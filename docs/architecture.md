---
type: architecture_spec
status: active
priority: p1
updated: 2026-06-13
context_policy: retrieve_only
owner: project
---

# Architecture

## System Overview

The local Judge is the single source of truth. CLI and future Web interfaces call the same application
services and must not implement separate judging behavior.

```text
CLI now                    Web later
   \                         /
    -> Application services
             |
             v
      Judge domain core
      /       |        \
 problem   execution   validation
 catalog    adapter     policies
      \       |        /
       progress repository
```

Context-engineering documents and guard scripts are a repository support system. They do not participate
in runtime judging.

## Planned Layers

| Layer | Responsibility | Must not own |
|---|---|---|
| Interface | CLI commands and future HTTP/UI adapters | Judge rules |
| Application | Orchestrate list, test, stress, benchmark, and progress use cases | OS-specific process details |
| Domain | Verdicts, limits, metadata contracts, invariant and complexity policies | CLI, Web, filesystem APIs |
| Infrastructure | Compiler/process runner, filesystem catalog, JSON progress storage | Curriculum policy |
| Problem packages | Statement, starter code, harness, generators, oracle, metadata | Global process management |

## Planned Modules

| Module | Responsibility |
|---|---|
| `judge/core` | Verdicts, test plans, limits, reports, and policy interfaces |
| `judge/runner` | Compile and execute learner code with bounded resources |
| `judge/catalog` | Discover and validate problem packages |
| `judge/testing` | Deterministic, random, differential, invariant, and benchmark checks |
| `judge/progress` | Record attempts, results, reviews, and spaced repetition state |
| `cli` | Implement `algo` commands without embedding Judge behavior |
| `problems` | Hold curriculum content grouped by stage and topic |
| `web` | Future adapter; absent until the CLI foundation is stable |

Exact directories may change during the first vertical slice. A structural change that affects public
contracts or process boundaries requires an ADR update.

## Dependency Direction

- Interfaces depend on application contracts.
- Application services depend on domain ports.
- Infrastructure implements domain ports.
- Domain code does not depend on CLI, Web, CMake, or operating-system APIs.
- Problem packages depend only on stable harness contracts.
- Web must invoke the same application layer used by CLI.

## Runtime Safety Boundary

- Learner programs run as child processes, never inside the Judge process.
- Every run has a timeout and bounded output capture.
- Temporary artifacts stay inside a dedicated project-controlled directory.
- The initial product is local-only and has no network execution API.
- Memory limiting on Windows needs a tested adapter; unsupported enforcement must be reported honestly.

## Stable Contracts

- Problem metadata schema.
- Test case and generator contract.
- Verdict and report shapes.
- Progress record schema.
- CLI exit-code behavior.

Breaking changes to these contracts require tests, migration notes, and an ADR when already in use.

## Architecture Questions

- Which Windows mechanism will enforce memory limits reliably with MinGW-built executables?
- Should the first progress repository use one JSON file or one record per problem?
- Which minimal HTTP adapter will be used when the Web phase begins?
