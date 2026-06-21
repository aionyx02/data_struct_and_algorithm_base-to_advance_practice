---
type: project_overview
status: active
priority: p1
updated: 2026-06-21
context_policy: always_retrievable
owner: project
---

# Project Overview

## Product

`data-structures-practice` is a local C++20 learning environment for progressively studying,
implementing, testing, and reviewing data structures.

## Goals

- Build the curriculum from the material under `資料結構/`.
- Cover 68 foundational and 64 advanced data-structure problems.
- Provide a local Judge for correctness, invariants, time, memory, API restrictions, and complexity evidence.
- Keep problem definitions, Judge logic, progress tracking, and future UI independently replaceable.
- Preserve durable project context through retrieval-first documentation.

## Non-Goals

- General algorithm training is outside the current scope.
- The current phase does not include dynamic programming, number theory, geometry, or a full ICPC curriculum.
- The Web interface will not own judging rules.
- The project is not intended to execute code submitted by untrusted remote users.

## Reference Materials

- The data-structure course materials under `資料結構/` are Git LFS source assets for curriculum planning and problem sequencing.
- The algorithm material under `演算法/` is an independent reference set and is not part of the current local Judge curriculum.
- Binary source assets are not documentation entry points. Keep them when a roadmap, scope document, or task uses them to justify curriculum content; do not treat them as active docs that agents should read during normal startup.

## Stack

- Learning language: C++20.
- Build system: CMake.
- Compiler: MinGW g++ on Windows; portable C++ is preferred.
- IDE: CLion.
- Context tooling: Node.js scripts from the context-engineering template.
- Storage: repository files with versioned problem and local progress JSON.
- Initial UI: native HTML/CSS/ES modules behind a loopback-only Node.js
  read adapter; submission remains on the shared Judge boundary.

## Platform Targets

- Primary: Windows 11 with PowerShell, CLion, CMake, and MinGW g++.
- Secondary: CI environments capable of running Node 20+ documentation checks.
- Future C++ CI should use a standards-compatible compiler and CMake.

## Engineering Priorities

1. Correctness and safe process execution.
2. Clear invariant and complexity validation.
3. Low memory and CPU overhead for large generated tests.
4. Decoupled Judge, problem, progress, CLI, and Web boundaries.
5. Beginner-friendly explanations and reproducible failures.

## Current Strategy

- Finish context-engineering initialization before application code.
- Implement one CLI Judge vertical slice before creating the full problem catalog.
- Keep the curriculum limited to data structures until the user explicitly expands scope.
- Evolve the read-only Web workspace without duplicating Judge behavior.
