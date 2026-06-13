---
type: coding_conventions
status: active
priority: p1
updated: 2026-06-13
context_policy: retrieve_when_planning
owner: project
---

# Coding Conventions

## Naming

| Item | Convention |
|---|---|
| C++ source and header files | `snake_case.cpp`, `snake_case.hpp` |
| Documentation and problem directories | `kebab-case` |
| Types and classes | `PascalCase` |
| Functions and local variables | `camelCase` |
| Compile-time constants | `kPascalCase` |
| Private data members | trailing underscore, such as `size_` |
| Tests | Match the production unit and use a `_test.cpp` suffix |
| Problem IDs | Stable stage/topic identifier such as `F03-stack-array` |

## C++ Rules

- Use C++20 and standard-library facilities unless a lesson explicitly forbids a container or algorithm.
- Prefer RAII, value semantics, and explicit ownership.
- Use `std::unique_ptr` for exclusive ownership when manual node management is not the learning objective.
- Raw pointers are allowed in pointer and linked-structure exercises, with explicit lifecycle tests.
- Avoid macros except include guards and narrowly scoped test instrumentation.
- Avoid `using namespace std;` in headers and shared code.
- Mark non-mutating methods `const` and important return values `[[nodiscard]]` when useful.
- Keep public ADT interfaces small and representation details private.

## Problem Rules

- Statements describe required complexity and forbidden APIs explicitly.
- Starter code must not reveal the intended implementation.
- Hidden tests must include empty, singleton, boundary, duplicate, and adversarial cases.
- Random tests must print or persist a reproducible seed on failure.
- Reference solutions belong outside learner-editable starter files.

## Architectural Shape

- Keep Judge domain logic independent from CLI, Web, filesystem, and process APIs.
- Keep operating-system process control behind an adapter.
- Do not let individual problem packages modify global Judge behavior.
- Prefer structured metadata parsing over ad hoc text extraction.

## Error Handling

- CLI errors must identify the problem, stage, and corrective action.
- Preserve child-process exit code and captured diagnostics without dumping unbounded output.
- Do not swallow exceptions or silently downgrade unsupported limits.

## Logging

- Logs must not contain secrets or unrestricted source/output dumps.
- Debug logs should include problem ID, run ID, test seed, verdict, and elapsed time.
- Detailed command history belongs in session logs, not startup context.
