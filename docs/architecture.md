---
type: architecture_spec
status: active
priority: p1
updated: 2026-06-14
context_policy: retrieve_only
owner: project
---

# Architecture

## System Overview

The local Judge is the single source of truth for verdicts. The `algo` CLI is the
only interface today; a future Web adapter must call the same application
services and must not implement separate judging behavior.

```text
algo CLI (src/main.cpp)            Web adapter (later)
        \                                /
         v                              v
        Application services
        JudgeService  +  StressService  +  ProgressRepository
                     |
                     v
            Judge domain core
   ProblemCatalog   ProcessRunner   CaseGenerator registry
   (discovery)      (OS adapter)    (differential oracles)
                     |
                     v
        Problem packages on disk (problems/**)
```

Problem definitions are **data on disk**, discovered at runtime. The Node scripts
under `scripts/` and the docs guards are a repository support system; they do not
participate in runtime judging.

## Repository Layout

```text
include/judge/          Public headers: domain contracts + service interfaces
  problem.hpp             Problem + ProblemMetadata value types
  judge_service.hpp       Verdict, TestResult, JudgeReport, JudgeService
  stress_service.hpp      StressReport, StressService
  progress.hpp            ProgressRecord and local repository contract
  process_runner.hpp      ProcessRequest/ProcessResult + ProcessRunner port
  catalog.hpp             ProblemCatalog
  case_generator.hpp      GeneratedCase, CaseGenerator(Registry), generateCase
src/
  main.cpp                The algo CLI: argument parsing + command dispatch
  catalog.cpp             Filesystem discovery + JSON metadata parsing/validation
  judge_service.cpp       Compile, forbidden-symbol scan, run tests, verdicts
  stress_service.cpp      Seeded differential run orchestration (~166 lines)
  progress.cpp            Versioned JSON parsing and atomic local replacement
  case_generator.cpp      Assembles the generator registry; ID-keyed dispatch
  process_runner_windows.cpp / _unsupported.cpp   ProcessRunner adapter impls
  generators/             Per-domain brute-force oracles (registry pattern)
    generator_support.hpp   Shared inline helpers (RNG, list printing)
    generator_registry.hpp  Declares each register<Domain>Generators(...)
    linear_/tree_/bst_/forest_dsu_/graph_/hashing_/advanced_generators.cpp
    segment_tree_generators.cpp   Stage A2 Segment Tree brute-force oracles
problems/<stage>/<category>/<id>/
  problem.json            Metadata contract (judge mode, limits, forbidden APIs)
  statement.md            Problem statement
  tests/*.in, *.out       Public, boundary, invariant, hidden fixtures
tests/
  fixtures/submissions/*.cpp   Correct + deliberately faulty reference programs
  run_cli_test.cmake, run_stress_test.cmake   CTest drivers
CMakeLists.txt, CMakePresets.json             Build + CTest wiring
scripts/                Repository tooling (practice CLI, docs/team guards)
```

## Layers

| Layer | Responsibility | Concrete code | Must not own |
|---|---|---|---|
| Interface | Parse args, dispatch `list`/`show`/`test`/`stress`, format output, set exit code | `src/main.cpp` | Judge rules |
| Application | Orchestrate the test and stress use cases | `JudgeService`, `StressService` | OS-specific process details |
| Progress | Persist local attempt summaries behind a reusable contract | `ProgressRepository` | Judge rules, source code, UI policy |
| Domain | Verdicts, metadata contract, generator contract, discovery rules | `judge_service.hpp`, `problem.hpp`, `case_generator.hpp`, `catalog.cpp` | CLI, Web, raw OS APIs |
| Infrastructure | Compile/run child processes, read the filesystem catalog | `ProcessRunner` impls, `ProblemCatalog` | Curriculum policy |
| Problem packages | Statement, metadata, fixtures, and the matching brute-force oracle | `problems/**`, `src/generators/**` | Global process management |

## Key Components

- **ProblemCatalog** (`catalog.cpp`) walks `problems/**/problem.json`, parses the
  JSON with a small hand-rolled parser, and validates each package: required
  metadata fields, ID character set, directory name equals ID, tests directory
  stays inside the package, and every `.in` has a matching `.out`. It currently
  accepts only `stdio` judge mode.
- **JudgeService** (`judge_service.cpp`) runs one submission against a package's
  test directory: scan source text for forbidden symbols, compile with `g++`
  (compiler configurable by environment), then for each `.in` run the binary
  through the `ProcessRunner` with the package's timeout and output cap and
  diff stdout against the `.out`.
- **ProcessRunner** (`process_runner.hpp` + platform `.cpp`) is the only OS
  boundary: it takes a `ProcessRequest` (executable, args, stdin, timeout,
  output limit) and returns a `ProcessResult` (launched, timedOut,
  outputLimitExceeded, exitCode, elapsed, captured streams). Windows has a real
  implementation; other platforms get an honest "unsupported" stub.
- **CaseGenerator registry** (`case_generator.cpp` + `src/generators/**`) maps a
  problem ID to a trusted brute-force model. Each generator returns a
  `GeneratedCase { input, expectedOutput }`. Generators are grouped into
  self-contained domain translation units, each exposing a
  `register<Domain>Generators(...)` registrar that `caseGenerators()` assembles
  once into an ID-keyed map.
- **StressService** (`stress_service.cpp`) draws per-case seeds from a master
  seed, asks the registry to generate cases into a temporary directory under
  `.judge/stress/`, reuses `JudgeService` to judge them, cleans the directory via
  RAII, and on failure reports the failing case seed plus a replay command.
- **ProgressRepository** (`progress.cpp`) strictly reads schema-1 local JSON and
  atomically replaces `.judge/progress.json` after a completed `test` command.

## Runtime Flows

- `algo list` / `algo show <id>`: `ProblemCatalog` discovers and validates
  packages; `show` prints metadata and the declared (not enforced) memory limit.
- `algo test <id> <source.cpp>`: forbidden-symbol scan → compile → run each test
  → diff output → aggregate the first non-AC test into the report verdict.
- `algo stress <id> <source.cpp> [--seed s] [--cases n]`: registry generates
  `n` differential cases (defaulting to the package's `random_tests`) → judge the
  generated directory → on failure print the case seed and a reproducible replay.
- `algo progress [id]`: read saved attempt counts, accepted counts, best AC
  time, and latest verdict. Automated `test` calls may opt out with
  `--no-progress`; stress never records learner progress.

## Verdict Vocabulary

The verdict enum (`judge_service.hpp`) is the shared report shape. Enforced
today: `AC`, `WA`, `TLE`, `RE` (launch failure, non-zero exit, or output overflow),
`CE`, and `API` (forbidden symbol). Declared but **not yet enforced**: `MLE`
(memory limits are read but not applied on the MinGW target), and the
`INV`/`CX` invariant and complexity verdicts reserved for later checks.

CLI exit codes: `0` for `AC`, `1` for any other verdict, `2` for usage or
internal errors.

## Dependency Direction

- The interface depends on application services; services depend on domain types.
- Infrastructure (`ProcessRunner`, `ProblemCatalog`) implements domain-facing
  behavior; domain code does not depend on CLI, Web, CMake, or OS APIs.
- Problem packages and generators depend only on the stable contracts below.
- A future Web adapter must invoke the same `JudgeService` / `StressService`.

## Runtime Safety Boundary

- Learner programs run as child processes, never inside the Judge process.
- Every run has a timeout and bounded output capture (defaults: 1000 ms, 64 KB).
- Temporary compile and stress artifacts stay under the project-controlled
  `.judge/` directory and are removed after use.
- The product is local-only with no network execution API; it is not a sandbox
  for untrusted remote code.
- Memory limiting on the MinGW target is unimplemented and reported honestly
  rather than silently assumed.

## Stable Contracts

- Problem metadata schema (`problem.json` fields read by `catalog.cpp`).
- Case generator contract (`GeneratedCase`, `CaseGenerator`).
- Verdict and report shapes (`Verdict`, `TestResult`, `JudgeReport`, `StressReport`).
- Progress schema version 1 and `ProgressRepository` record semantics.
- CLI command surface and exit-code behavior.

Breaking changes to these require tests, migration notes, and an ADR when the
contract is already in use.

## Extension Points

- **Add a problem**: create `problems/<stage>/<category>/<id>/` (metadata,
  statement, `tests/*`), add one brute-force generator to the matching
  `src/generators/<domain>_generators.cpp`, register it in that file's
  registrar, add reference fixtures, and wire CTest entries in `CMakeLists.txt`.
- **Add a judge mode** (`function`, `adt`): extend `catalog.cpp` validation and
  `JudgeService`; keep the `ProcessRunner` boundary unchanged.
- **Add the Web layer**: place an HTTP adapter in front of the existing
  application services without duplicating judging logic.

## Not Yet Built (Planned)

- Spaced repetition and review scheduling on top of the progress repository.
- Web dashboard and practice UI, after the CLI is stable on enough problems.
- Memory-limit enforcement adapter and the `INV`/`CX` structural and complexity
  checks described in `docs/judge-requirements.md`.

## Open Questions

- Which Windows mechanism reliably enforces memory limits for MinGW binaries?
- Should the first progress store be one JSON file or one record per problem?
- Which minimal HTTP adapter fits the Web phase?
