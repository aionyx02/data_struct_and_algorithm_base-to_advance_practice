---
type: testing_policy
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_when_debugging
owner: project
---

# Testing Strategy

## Current Required Checks

```powershell
npm run lint
npm run security:scan
npm test
npm run team:guard
npm run docs:refresh
npm run docs:ready
```

## Required C++ Checks

```powershell
cmake --preset dev
cmake --build --preset dev
ctest --preset dev
```

When using a Visual Studio multi-config build directory instead of the preset, pass a configuration:
`ctest --test-dir build -C Debug --output-on-failure`.

## Test Layers

| Layer | Purpose |
|---|---|
| Documentation guards | Frontmatter, links, placeholders, secrets, task ownership, generated indexes |
| Domain unit tests | Metadata parsing, verdict rules, invariant checks, complexity classification |
| Infrastructure tests | Compiler invocation, timeout, output capture, temporary file cleanup |
| Problem contract tests | Starter code, oracle, generator, harness, metadata compatibility |
| Differential tests | Compare learner structure against a trusted simple model |
| Stress tests | Randomized operation sequences with reproducible seeds |
| Benchmark tests | Measure `n`, `2n`, and `4n` growth and operation counts |
| End-to-end tests | Compile and judge known AC, WA, TLE, RE, API, and invariant-breaking submissions |
| Web adapter tests | Verify loopback API data, progress mapping, compile bounds, static allowlist, CSP, and method rejection |

`npm test` starts the local Web adapter on an ephemeral loopback port with an
isolated fixture. Browser verification additionally covers search, navigation,
review filtering, mouse/keyboard pane resizing, successful and failed real
compiler feedback, and desktop/mobile behavior against the real 132-problem catalog.

The first seventeen foundation problems support seeded differential stress tests:

```powershell
.\build\mingw-debug\algo.exe stress F04-circular-queue `
  tests\fixtures\submissions\circular_queue_correct.cpp `
  --seed 20260613
```

Each default run executes 100 generated cases with 100 operations per case. A failure reports
the master seed, per-case seed, replay command, and complete generated input.

## Regression Policy

1. Reproduce the failure or document why reproduction is impossible.
2. Add the smallest test that would have caught it.
3. Preserve the failing seed or minimized operation sequence.
4. Record detailed root cause in the member session log.
5. Update durable docs only when behavior or contracts changed.

## Sanitizers

- Use AddressSanitizer and UndefinedBehaviorSanitizer where the selected Windows toolchain supports them.
- Do not report sanitizer coverage when the compiler cannot provide it.
- Include use-after-free, double-delete, out-of-bounds, invalid iterator, and integer overflow cases where practical.

## Security Validation

- Run `npm run security:scan` for workflow, shell, process execution, or path-handling changes.
- Test paths containing spaces and non-ASCII characters.
- Test timeout cleanup and bounded output capture before accepting the process runner.
