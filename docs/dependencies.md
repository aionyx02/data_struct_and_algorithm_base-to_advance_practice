---
type: dependency_policy
status: active
priority: p1
updated: 2026-06-21
context_policy: retrieve_when_planning
owner: project
---

# Dependencies

## Dependency Rules

- Prefer the C++ and Node.js standard libraries.
- New C++ runtime libraries require a concrete need and comparison with a standard-library solution.
- A testing library may be added only when the first Judge slice demonstrates that a custom harness is insufficient.
- Framework, database, sandbox, HTTP server, or UI dependencies require an ADR.
- Lock dependency versions when reproducibility depends on them.

## Current Dependencies

| Dependency | Purpose | Scope | Replaceable? | Notes |
|---|---|---|---|---|
| CMake 4.3+ | Configure and build future C++ targets | development | yes | Avoid version-specific features without need |
| MinGW g++ 15.2+ | Compile C++20 learner and Judge code | development | yes | Code should remain compiler-portable |
| Node.js 20+ | Run documentation guards and tests | development | yes | No third-party Node packages are required |
| npm | Invoke repository context scripts | development | yes | `package-lock.json` records the tool state |

## Rejected Or Deferred Dependencies

| Dependency | Reason | Status |
|---|---|---|
| GoogleTest | Defer until the C++ testing shape is known | deferred |
| Web framework | Native modules cover the first read-only slice; revisit when component complexity justifies it | deferred |
| Database | Local JSON storage is sufficient for the initial scope | deferred |
| Container sandbox | Local single-user Judge does not yet justify the operational cost | deferred |

## Dependency Change Checklist

- [ ] Existing dependency or standard library cannot solve the need.
- [ ] License and maintenance status are acceptable.
- [ ] Security implications are understood.
- [ ] Runtime and build costs are acceptable.
- [ ] Tests cover the new dependency path.
- [ ] Removal or replacement remains practical.
